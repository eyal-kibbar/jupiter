#include "ganymede.h"
#include "gmd_platform.h"
#include "gmd_kern.h"
#include "gmd_sched.h"


#include <stdio.h>

#define WAITING_TICK  0x01
#define WAITING_EVENT 0x02
#define WAITING_IO    0x04

#define GMD_IS_EVENT_CHANGED(p_event, mask, event) (((*p_event) & (mask)) != ((event) & (mask)))

enum task_state_e {
    TASK_STATE_IDLE,
    TASK_STATE_RUNNING,
    TASK_STATE_WAITING,
};


typedef struct task_s task_t;

struct task_s {
    struct gmd_task_data_s data;
    task_t* next;
    enum task_state_e state;
    uint8_t ctx[GMD_SCHED_CONTEXT_SIZE];

    uint8_t waiting;

    volatile uint8_t* p_event;
    uint8_t event_mask;
    uint8_t event;
    uint8_t sleep_tick;
};

typedef struct sched_s {
    task_t scheduler;
    uint8_t sched_stack[GMD_SCHED_STACK_SIZE];
    task_t* first;
    task_t* curr;
} sched_t;

static sched_t gmd_sched;


static void sched_task_switch(task_t* next)
{
    task_t* prev = gmd_sched.curr;
    gmd_sched.curr = next;
    gmd_sched.curr->state = TASK_STATE_RUNNING;

    gmd_platform_context_swap((sched_context_t)prev->ctx, (sched_context_t)next->ctx);
}

__attribute__((noreturn))
static void task_start(void)
{
    platform_sei();

    if (NULL != gmd_sched.curr->data.init_func) {
        gmd_sched.curr->data.init_func();
    }

    while (1) {
        gmd_sched.curr->data.loop_func();
    }
}

static void gmd_sched_loop()
{
    task_t* task;
    uint8_t maysleep;
    uint8_t curtick;
    uint8_t sleep_flags;
    // run all idle tasks
    for (task = gmd_sched.first; task; task = task->next) {
        gmd_wdg_reset();
        if (TASK_STATE_IDLE == task->state) {
            gmd_sched.curr->state = TASK_STATE_IDLE;
            sched_task_switch(task);
        }

    }

    // wakeup all sleeping tasks
    maysleep = 1;
    sleep_flags = 0;
    curtick = gmd_curtick();

    // clear interrupt before checking for events.
    // otherwise we may end up sleeping without the event interrupt to wake us up
    platform_cli();

    for (task = gmd_sched.first; task; task = task->next) {

        if (TASK_STATE_WAITING == task->state) {

            if (task->waiting & WAITING_TICK) {
                sleep_flags |= GMD_SLEEP_F_CLK;

                if (curtick != task->sleep_tick) {
                    task->state = TASK_STATE_IDLE;
                }
            }

            if (task->waiting & WAITING_EVENT && GMD_IS_EVENT_CHANGED(task->p_event, task->event_mask, task->event)) {
                task->state = TASK_STATE_IDLE;
            }

            if (task->waiting & WAITING_IO) {
                sleep_flags |= GMD_SLEEP_F_IO;
            }
        }

        // stop sleeping if an idle task is encountered
        maysleep &= !(TASK_STATE_IDLE == task->state);
    }
    gmd_wdg_reset();
    if (maysleep) {
        gmd_platform_sleep(sleep_flags);
    }
    else {
        platform_sei();
    }
}

void gmd_sched_init()
{
    task_t* task;
    extern char __start_tasks[];
    extern char __stop_tasks[];

    gmd_sched.first = (task_t*)__start_tasks;

    for (task = gmd_sched.first; task; task = (task_t*)task->next) {

        if (GMD_MAGIC != task->data.magic) {
            gmd_panic();
        }

        gmd_platform_context_create((sched_context_t)task->ctx,
                task_start,
                (void*)((uint8_t*)task + sizeof(task_t)),
                (size_t)task->data.stack_size - sizeof(task_t));

        task->next = (task_t*)((uintptr_t)task + (uintptr_t)task->data.stack_size);


        if (NULL != task->data.setup_func) {
            task->data.setup_func();
        }

        task->state = TASK_STATE_IDLE;

        if (task->next == (task_t*)__stop_tasks) {
            task->next = NULL;
        }
    }

    // initialize the scheduler task
    gmd_platform_context_create((sched_context_t)gmd_sched.scheduler.ctx,
        task_start,
        (void*)gmd_sched.sched_stack,
        sizeof gmd_sched.sched_stack);
    //gmd_sched.scheduler.data.init_func = <> TODO: if sched needs additional initialization function, add it here
    gmd_sched.scheduler.data.loop_func = gmd_sched_loop;


    gmd_sched.curr = &gmd_sched.scheduler;
    gmd_sched.curr->state = TASK_STATE_IDLE;
}

void gmd_sched_start()
{
    gmd_sched.scheduler.state = TASK_STATE_RUNNING;
    gmd_platform_context_load((sched_context_t)gmd_sched.scheduler.ctx);
}

static uint16_t gmd_wfe_aux(volatile uint8_t* p_event, uint8_t mask, uint8_t event, uint16_t timeout_ms, uint8_t flags)
{
    uint8_t before_tick, tick, nticks;
    uint16_t us, carry_us = 0, sleeping_ms = 0;

    // check if event has already happened
    if ((event & mask) != (*p_event & mask)) {
        return 0;
    }

    gmd_sched.curr->waiting    = WAITING_EVENT;
    gmd_sched.curr->p_event    = p_event;
    gmd_sched.curr->event      = event;
    gmd_sched.curr->event_mask = mask;

    if (flags) {
        gmd_sched.curr->waiting |= WAITING_IO;
    }

    if (0 == timeout_ms) {
        gmd_sched.curr->state = TASK_STATE_WAITING;
        sched_task_switch(&gmd_sched.scheduler);
        return 0;
    }

    tick = gmd_curtick();
    gmd_sched.curr->waiting |= WAITING_TICK;

    do {
        before_tick = tick;

        // sleep for at least 1 tick
        gmd_sched.curr->state = TASK_STATE_WAITING;
        gmd_sched.curr->sleep_tick = tick;
        sched_task_switch(&gmd_sched.scheduler);

        tick = gmd_curtick();
        nticks = tick - before_tick;
        sleeping_ms += gmd_ticks2ms(nticks, &us);
        carry_us += us;
        if (carry_us >= 1000) {
            ++sleeping_ms;
            carry_us -= 1000;
        }

    } while (sleeping_ms < timeout_ms && !GMD_IS_EVENT_CHANGED(p_event, mask, event));


    return sleeping_ms;
}

uint16_t gmd_wfe(volatile uint8_t* p_event, uint8_t mask, uint8_t event, uint16_t timeout_ms)
{
    return gmd_wfe_aux(p_event, mask, event, timeout_ms, 0);
}

uint16_t gmd_wfe_io(volatile uint8_t* p_event, uint8_t mask, uint8_t event, uint16_t timeout_ms)
{
    return gmd_wfe_aux(p_event, mask, event, timeout_ms, 1);
}

uint16_t gmd_delay(uint16_t ms)
{
    volatile uint8_t no_evt = 0;
    if (0 == ms) {
        gmd_sched.curr->state = TASK_STATE_IDLE;
        sched_task_switch(&gmd_sched.scheduler);
        return 0;
    }

    return gmd_wfe(&no_evt, 0, 0, ms);
}
