#ifndef GMD_PLATFORM_H_
#define GMD_PLATFORM_H_

#include <ucontext.h>

#define platform_cli()
#define platform_sei()
#define platform_isei() 1

#define GMD_SCHED_CONTEXT_SIZE (sizeof(ucontext_t))

//#define likely(x)       __builtin_expect((x),1)
//#define unlikely(x)     __builtin_expect((x),0)




#endif /* GMD_PLATFORM_H_ */
