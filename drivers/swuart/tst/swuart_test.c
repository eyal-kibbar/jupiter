#include "ganymede.h"
#include "logging.h"
#include "swuart.h"

void setup()
{
}

void loop()
{
    LOG_INFO(swuart, "testing 123");
    gmd_delay(500);
}



TASK(STACK_SIZE);
