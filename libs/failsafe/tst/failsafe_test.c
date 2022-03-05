#include "ganymede.h"
#include "failsafe.h"
#include "logging.h"
#include "io.h"

int i = 0;

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    //io_analog_init();
    failsafe_init(5, gmd_panic);
}

void loop()
{
    ++i;
    LOG_INFO(TEST, "testing %d", i);
    gmd_delay(1000);
    if (i < 10) {
        failsafe_reset(0);
    }
}



TASK(STACK_SIZE);
