#ifndef PID_H_
#define PID_H_

typedef struct pid_config_s {
    float kp;
    float kd;
    float ki;

    float output_limit;
} pid_config_t;

typedef struct pid_state_s {
    float prev_error;
    float error_integral;
} pid_state_t;

float pid_step(pid_config_t* cfg, pid_state_t* state, float input, float dt, float setpoint);

#endif /* PID_H_ */
