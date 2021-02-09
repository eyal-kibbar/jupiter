#include "failsafe.h"
#include "ganymede.h"

struct failsafe_s {
    uint8_t countdown_sec;
    uint8_t reset_sec;
};

struct failsafe_s failsafe;


void failsafe_init(uint8_t sec)
{
    failsafe.reset_sec = sec;
    failsafe_reset();
}

void failsafe_reset()
{
    failsafe.countdown_sec = failsafe.reset_sec;
}

void loop()
{
    if (0 == failsafe.countdown_sec) {
        gmd_panic();
    }

    --failsafe.countdown_sec;
    gmd_delay(1000);
}



TASK(STACK_SIZE);
