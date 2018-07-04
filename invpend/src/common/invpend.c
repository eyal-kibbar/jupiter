#include "ganymede.h"
#include "logging.h"
#include "mpu.h"
#include <math.h>


#define DELTA_T 1.25
#define Kp 1
#define Ki 0.5
#define Kd 0.5

#define ARR_SIZE(x) (sizeof(x) / sizeof(*(x)))

float window[5];
int window_head;


static float calc_theta(mpu_data_t* data)
{
    return atan2f(data->accel[0], data->accel[2]);
}

static float calc_theta_dot()
{
    int window_tail = window_head - 1;
    float delta;
    
    if (window_tail < 0) {
        window_tail = ARR_SIZE(window) - 1;
    }
    
    return (window[window_head] - window[window_tail]) / DELTA_T;
}

static float calc_theta_integral()
{
    int i;
    float sum = 0.0f;
    for (i=0; i < ARR_SIZE(window); ++i) {
        sum += window[i];
    }
    return sum;
}

static float calc_accel(float err)
{
    const float g = 9.8f;
    const float l = 0.5f;
    return (g/l) * sin(err);
}

void setup()
{
    mpu_init();
}

void loop()
{
    uint8_t val;
    float theta;
    float theta_dot;
    float theta_integral;
    float err;
    float accel;
    mpu_data_t data;
    
    // read mpu data
    if (0 == (val = mpu_get_status())) {
        gmd_delay(100);
        return;
    }
    
    mpu_read(&data);
    
    theta = calc_theta(&data);
    window[window_head++] = theta;
    if (window_head == ARR_SIZE(window)) {
        window_head = 0;
    }

    // calc err
    theta_dot = calc_theta_dot();
    theta_integral = calc_theta_integral();
    LOG_INFO(INVPEND, "theta: %6d dot: %6d",
             (int16_t)(theta * (180/M_PI)),
             (int16_t)(theta_dot * (180/M_PI)));
    err = (Kd * theta_dot) + (Kp * theta) + (Ki * theta_integral);
    
    // calc accel
    accel = calc_accel(err);
    LOG_INFO(INVPEND, "err: %6d accel: %6d",
             (int16_t)(err * (180/M_PI)),
             (int16_t)(1000 * accel));
    // set servo value
}


TASK(STACK_SIZE);
