#ifndef HEXPOD_H_
#define HEXPOD_H_

#include <stdint.h>

#define HEXPOD_NUM_LEGS 6

typedef struct hexpod_leg_s
{
    uint8_t v;
    uint8_t h;
} hexpod_leg_t;

typedef struct hexpod_state_s
{
    hexpod_leg_t leg[HEXPOD_NUM_LEGS];
} hexpod_state_t;



void hexpod_load(hexpod_state_t* script, uint8_t n);




#endif /* HEXPOD_H_ */
