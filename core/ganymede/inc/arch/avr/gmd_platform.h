#ifndef GMD_PLATFORM_H_
#define GMD_PLATFORM_H_

#include <avr/interrupt.h>
#include <setjmp.h>

#define platform_cli() cli()
#define platform_sei() sei()
#define platform_isei() (SREG & 0x80)

#define GMD_SCHED_CONTEXT_SIZE (sizeof(jmp_buf))

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)


typedef struct sched_context_s* sched_context_t;

#endif /* GMD_PLATFORM_H_ */

