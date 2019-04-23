#include "ganymede.h"
#include "gmd_platform.h"
#include "gmd_sched.h"
#include "gmd_kern.h"

#include <stdio.h>
#include <ucontext.h>

struct sched_context_s {
    ucontext_t uctx;
};


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
    getcontext(&ctx->uctx);
    ctx->uctx.uc_stack.ss_sp = stack;
    ctx->uctx.uc_stack.ss_size = stack_size;
    ctx->uctx.uc_stack.ss_flags = 0;
    ctx->uctx.uc_link = 0;
    makecontext(&ctx->uctx, func, 0);
}

void gmd_platform_context_swap(
    sched_context_t save,
    sched_context_t load)
{
    swapcontext(&save->uctx, &load->uctx);
}

/**
 * sleep
 * ========================================
 **/

void gmd_platform_sleep()
{

}

/**
 * timer
 * ========================================
 **/

uint8_t gmd_curtick()
{
    return 0;
}

uint16_t gmd_ticks2ms(uint8_t ticks, uint16_t* out_us)
{
    *out_us = 0;
    return 0;
}

uint16_t gmd_ms2ticks(uint16_t ms, uint16_t* out_us)
{
    return 0;
}


/**
 * log
 * ========================================
 **/
void gmd_log_init()
{
}


void gmd_platform_init()
{
    // TODO: create a timer thread
    // TODO: create an interrupt thread
}
