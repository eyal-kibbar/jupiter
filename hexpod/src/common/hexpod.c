#include "hexpod.h"
#include "ganymede.h"
#include "logging.h"
#include "servo.h"
#include <string.h>

#define MIN(a, b) ( ((a) < (b)) ? (a) : (b) )

typedef struct hexpod_s {
    hexpod_state_t script[HEXPOD_MAX_INSTRUCTIONS];
    uint8_t curr;
    uint8_t num_instructions;
} hexpod_t;

static const uint8_t hexpod_servoidx2pin[HEXPOD_NUM_SERVOS] =
    {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

static hexpod_t hexpod;

static void hexpod_init()
{
    int i;
    
    servo_init();

    hexpod_load(NULL, 0);
    
    for (i=0; i < HEXPOD_NUM_SERVOS; ++i) {
        servo_attach(hexpod_servoidx2pin[i]);
    }
}

static void hexpod_step()
{
    uint8_t i;
    hexpod_state_t* s;
    
    if (0 == hexpod.num_instructions) {
        return;
    }
    
    LOG_INFO(HEXPOD, "running step %d", hexpod.curr);
    
    s = &hexpod.script[hexpod.curr++];
    
    if (hexpod.num_instructions == hexpod.curr) {
        hexpod.curr = 0;
    }
    
    for (i=0; i < HEXPOD_NUM_SERVOS; ++i) {
        servo_set(hexpod_servoidx2pin[i], s->servo_pwm[i]);
    }
}

void hexpod_load(hexpod_state_t* script, uint8_t n)
{
    hexpod.curr = 0;

    LOG_INFO(HEXPOD, "loading %d instructions", n);
    
    n = MIN(n, HEXPOD_MAX_INSTRUCTIONS);
    if (0 == n) {
        memset(&hexpod, 0, sizeof hexpod);
    }
    else {
        memcpy(hexpod.script, script, sizeof(*script) * n);
        hexpod.num_instructions = n;
        hexpod.curr = 0;
    }
}


void setup()
{
    hexpod_init();
}

void loop()
{
    hexpod_step();
    gmd_delay(HEXPOD_SERVO_DELAY_MS);
}

TASK(STACK_SIZE);
