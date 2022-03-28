#ifndef QUAD_H_
#define QUAD_H_

#include "pid.h"

enum quad_state_e {
    QUAD_INIT,
    QUAD_READY,
    QUAD_BALANCING
};

struct quad_s {
    volatile uint8_t state;

    float setpoint[3];
    float throtte;

    int16_t motor[4];

    pid_config_t pid_cfg;
    pid_config_t pid_yaw_cfg;

    pid_state_t pid_roll_state;
    pid_state_t pid_pitch_state;
    pid_state_t pid_yaw_state;
};

extern struct quad_s quad;

void quad_start();
void quad_stop();
void quad_set_setpoint(float rpy[3], float throtte);

void quad_angles_get(float rpy[3], float* dt);
void quad_get_servo_microseconds(int16_t motor[4]);


#endif /* QUAD_H_ */
