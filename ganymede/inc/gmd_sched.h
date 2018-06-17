#ifndef GMD_SCHED_H_
#define GMD_SCHED_H_

#include <stdint.h>
#include <stddef.h>
#include "gmd_platform.h"

void gmd_sched_init();
void gmd_sched_loop();

void gmd_platform_context_create(
    sched_context_t ctx,
    void (*func)(),
    void* stack,
    size_t stack_size);

void gmd_platform_context_swap(
    sched_context_t save,
    sched_context_t load);

void gmd_platform_sleep();


#endif /* GMD_SCHED_H_ */

