#include "ganymede.h"
#include "gmd_platform.h"
#include "gmd_sched.h"
#include "gmd_kern.h"


#include <setjmp.h>

#include <avr/sleep.h>
#include <avr/power.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <avr/wdt.h>

struct sched_context_s {
    uint8_t  regs[16];
    uint16_t bp;
    uint16_t sp;
    uint8_t sreg;
    uint16_t pc;
} __attribute__((packed));


static volatile uint8_t gmd_timer_tick;


/**
 * sched
 * ========================================
 **/

void gmd_platform_context_create(
    sched_context_t ctx,
    void (*func)(),
    void* stack,
    size_t stack_size)
{

    setjmp(*(jmp_buf*)ctx);
    ctx->bp = 0;
    ctx->sp = (uint16_t)stack + stack_size - sizeof(uint16_t);
    ctx->pc = (uint16_t)func;
}

void gmd_platform_context_load(
    sched_context_t load)
{
    longjmp(*(jmp_buf*)load, 1);
}

void gmd_platform_context_swap(
    sched_context_t save,
    sched_context_t load)
{
    if (0 == setjmp(*(jmp_buf*)save)) {
        longjmp(*(jmp_buf*)load, 1);
    }
}

/**
 * sleep
 * ========================================
 **/

void gmd_platform_sleep(uint8_t flags)
{
    cli();

    // disable ADC when sleeping
    //ADCSRA = 0;
    //set_sleep_mode(SLEEP_MODE_IDLE);


    if (flags & (GMD_SLEEP_F_IO)) {
        set_sleep_mode(SLEEP_MODE_IDLE);
    }
    else if (flags & GMD_SLEEP_F_CLK) {
        set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    }
    else {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    }

    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}

/**
 * timer
 * ========================================
 **/

static void gmd_platform_timer_init()
{
    PRR &= ~(_BV(PRTIM2));
    ASSR = _BV(AS2); // set clock to function as realtime hardware clock (RTC)
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20); // set clock with 1024 prescalar
    TIMSK2 = _BV(TOIE2); // enable overflow interrupt
    // clock src is 16MHz,
    // we prescale it with 1024, that gives us a counter decrement rate of ~16KHz
    // for every 256 counter decrements we get a tick
    // that gives us tick rate of ~61Hz, or 1 tick every 16.384 ms
}

uint8_t gmd_curtick()
{
    return gmd_timer_tick;
}

uint16_t gmd_ticks2ms(uint8_t ticks, uint16_t* out_us)
{
    uint16_t ms, us, x;
    uint16_t ticks16;

    us = 0;
    ticks16 = (uint16_t)ticks;
    ms = ticks16 * 16;

    // avoid overflow
    if (170 < ticks16) {
        ms += 65;  // (170 * 384) / 1000;
        us = 280; // (170 * 384) % 1000;
        ticks16 -= 170;
    }

    x = ticks16 * 384;
    ms += x / 1000;
    us += x - ((x/1000) * 1000);

    if (NULL != out_us) {
        *out_us = us;
    }

    return ms;
}

uint16_t gmd_ms2ticks(uint16_t ms, uint16_t* out_us)
{
    return 0;
}

// using timer 2
ISR(TIMER2_OVF_vect)
{
    ++gmd_timer_tick;
}

/**
 * Watchdog
 * ========================================
 **/

#define		wdr()	asm("wdr"::)

/* stack size calculation does not support naked functions
__attribute__((naked))
__attribute__((section(".init3")))
 void watchdog_reset(void)
{
    wdr();
    wdt_disable();
}
*/

void watchdog_reset(void) {}

void gmd_wdg_reset()
{
    wdr();
}

void gmd_platform_init()
{
    wdr();
#ifdef GMD_WATCHDOG_ENABLED
    wdt_enable(WDTO_1S);
#endif
    //WDTCSR = _BV(WDE) | _BV(WDP3);
    PRR = ~0; // shut down all systems for power saving
    gmd_platform_timer_init();
}
