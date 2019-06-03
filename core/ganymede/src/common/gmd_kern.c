#include "ganymede.h"
#include "gmd_kern.h"
#include "gmd_sched.h"
#include "gmd_uart.h"
#include "gmd_i2c.h"

#include <stdio.h>

__attribute__((weak)) void gmd_uart_init() {}
//__attribute__((weak)) void gmd_i2c_init() {}


void gmd_panic()
{
    printf("PANIC\n\r");
    while (1);
}


int main()
{
    gmd_platform_init();
    gmd_uart_init();
    gmd_log_init();
    gmd_i2c_init();
    gmd_sched_init();


    platform_sei();


    while (1) gmd_sched_loop();

    return 0;
}
