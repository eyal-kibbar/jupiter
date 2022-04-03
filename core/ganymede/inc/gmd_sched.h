#ifndef GMD_SCHED_H_
#define GMD_SCHED_H_

#include <stdint.h>
#include <stddef.h>
#include "gmd_platform.h"


typedef struct sched_context_s* sched_context_t;

void gmd_sched_init();


void gmd_sched_start();

void gmd_platform_context_create(
    sched_context_t ctx,
    void (*func)(),
    void* stack,
    size_t stack_size);

void gmd_platform_context_load(
    sched_context_t load);

void gmd_platform_context_swap(
    sched_context_t save,
    sched_context_t load);

void gmd_platform_sleep(uint8_t flags);


#endif /* GMD_SCHED_H_ */
