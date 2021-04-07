#include "failsafe.h"
#include "ganymede.h"

void setup()
{
}


void loop()
{
    failsafe_dec(1);
    gmd_delay(1000);
}



TASK(STACK_SIZE);
