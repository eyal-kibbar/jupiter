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

void failsafe_dec(uint8_t sec)
{
    if (failsafe.countdown_sec <= sec) {
        gmd_panic();
    }

    failsafe.countdown_sec -= sec;
}
