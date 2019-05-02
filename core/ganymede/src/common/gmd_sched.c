#include "ganymede.h"
#include "gmd_platform.h"
#include "gmd_kern.h"
#include "gmd_sched.h"


#include <stdio.h>

#define WAITING_TICK  0x01
#define WAITING_EVENT 0x02

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
    uint8_t sleep_tick;
};

typedef struct sched_s {
    task_t scheduler;
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

    while (1) {
        gmd_sched.curr->data.loop_func();
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


        task->data.setup_func();

        task->state = TASK_STATE_IDLE;

        if (task->next == (task_t*)__stop_tasks) {
            task->next = NULL;
        }
    }
    gmd_sched.curr = &gmd_sched.scheduler;
    gmd_sched.curr->state = TASK_STATE_RUNNING;
}

void gmd_sched_loop()
{
    task_t* task;
    uint8_t maysleep;
    uint8_t curtick;

    // run all idle tasks
    for (task = gmd_sched.first; task; task = task->next) {

        if (TASK_STATE_IDLE == task->state) {
            gmd_sched.curr->state = TASK_STATE_IDLE;
            sched_task_switch(task);
        }

    }

    // wakeup all sleeping tasks
    maysleep = 1;
    curtick = gmd_curtick();

    // clear interrupt before checking for events.
    // otherwise we may end up sleeping without the event interrupt to wake us up
    platform_cli();

    for (task = gmd_sched.first; task; task = task->next) {

        if (TASK_STATE_WAITING == task->state) {

            if ( (task->waiting & WAITING_TICK  && curtick != task->sleep_tick) ||
                 (task->waiting & WAITING_EVENT && 0 != (*task->p_event & task->event_mask)) ) {

                 task->state = TASK_STATE_IDLE;
             }

        }

        // stop sleeping if an idle task is encountered
        maysleep &= !(TASK_STATE_IDLE == task->state);
    }

    if (maysleep) {
        gmd_platform_sleep();
    }
    else {
        platform_sei();
    }
}

uint16_t gmd_wfe(volatile uint8_t* p_event, uint8_t mask, uint16_t timeout_ms)
{
    uint8_t before_tick, tick, nticks;
    uint16_t us, carry_us = 0, sleeping_ms = 0;

    // check if event has already happened
    if (0 != (*p_event & mask)) {
        return 0;
    }

    gmd_sched.curr->waiting = WAITING_EVENT;
    gmd_sched.curr->p_event = p_event;
    gmd_sched.curr->event_mask = mask;

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

    } while (sleeping_ms < timeout_ms && 0 == (*p_event & mask));

    return sleeping_ms;
}

void gmd_delay(uint16_t ms)
{
    volatile uint8_t no_evt = 0;
    if (0 == ms) {
        gmd_sched.curr->state = TASK_STATE_IDLE;
        sched_task_switch(&gmd_sched.scheduler);
        return;
    }

    gmd_wfe(&no_evt, 0, ms);
}
