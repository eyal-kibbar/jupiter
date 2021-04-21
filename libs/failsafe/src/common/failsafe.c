#include "failsafe.h"
#include "ganymede.h"

struct failsafe_s {
    uint8_t countdown_sec[8];
    uint8_t reset_sec;
    uint8_t valid_counters;
};

struct failsafe_s failsafe;


void failsafe_init(uint8_t sec)
{
    failsafe.reset_sec = sec;
}

void failsafe_reset(uint8_t counter_idx)
{
    failsafe.countdown_sec[counter_idx] = failsafe.reset_sec;
    failsafe.valid_counters |= 1 << counter_idx;
}

void failsafe_dec(uint8_t sec)
{
    uint8_t i;
    for (i=0; i < sizeof(failsafe.countdown_sec); ++i) {
        if (0 == (failsafe.valid_counters & (1<<i))) {
            continue; // counter is not valid
        }

        if (failsafe.countdown_sec[i] == 0) {
            gmd_panic();
        }

        failsafe.countdown_sec[i] -= sec;
    }
}
