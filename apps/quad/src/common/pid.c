#include "pid.h"
#include "utils.h"

float pid_step(pid_config_t* cfg, pid_state_t* state, float input, float setpoint)
{
    float output;
    float error = input - setpoint;
    float d_input = (error - state->prev_error);

    state->error_integral += error;
    state->error_integral = MIN(cfg->output_limit, state->error_integral);
    state->error_integral = MAX(-cfg->output_limit, state->error_integral);

    output = (cfg->kp * error) + (cfg->ki * state->error_integral) + (cfg->kd * d_input);
    output = MIN(cfg->output_limit, output);
    output = MAX(-cfg->output_limit, output);

    state->prev_error = error;

    return output;
}
