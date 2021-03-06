#include "ganymede.h"
#include "gmd_kern.h"
#include "gmd_sched.h"

#include <stdio.h>

void gmd_panic()
{
    printf("PANIC\n\r");
    while (1);
}


int main()
{
    gmd_platform_init();
    gmd_sched_init();
    platform_sei();

    while (1) gmd_sched_loop();

    return 0;
}
