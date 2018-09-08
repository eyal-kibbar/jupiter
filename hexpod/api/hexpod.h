#ifndef HEXPOD_H_
#define HEXPOD_H_

#include <stdint.h>

#define HEXPOD_NUM_LEGS 6
#define HEXPOD_LEG_DOF  2

#define HEXPOD_NUM_SERVOS (HEXPOD_NUM_LEGS*HEXPOD_LEG_DOF)

#pragma pack(push, 1)
typedef struct hexpod_state_s
{
    uint8_t servo_pwm[HEXPOD_NUM_SERVOS];
} hexpod_state_t;
#pragma pack(pop)


void hexpod_load(hexpod_state_t* script, uint8_t n);




#endif /* HEXPOD_H_ */
