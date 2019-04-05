#include "ganymede.h"
#include "moisture.h"
#include "logging.h"


void setup()
{
}

void loop()
{
    LOG_INFO(moisture, "testing");
    gmd_delay(1000);
}



TASK(STACK_SIZE);
