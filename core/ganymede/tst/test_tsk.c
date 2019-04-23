#include "ganymede.h"
#include "logging.h"

void setup()
{

}

void loop()
{
    LOG_INFO(test, "testing 123");
    gmd_delay(1000);
}


TASK(STACK_SIZE);
