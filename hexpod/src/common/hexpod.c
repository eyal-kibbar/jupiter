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

static const hexpod_leg_t hexpod_leg2pin[HEXPOD_NUM_LEGS] = {
    {.v = 2, .h = 3},
    {.v = 4, .h = 5},
    {.v = 6, .h = 7},
    {.v = 8, .h = 9},
    {.v = 10, .h = 11},
    {.v = 12, .h = 13},
};

static hexpod_t hexpod;

static void hexpod_init()
{
    int i;
    
    servo_init();

    hexpod_load(NULL, 0);
    
    for (i=0; i < HEXPOD_NUM_LEGS; ++i) {
        servo_attach(hexpod_leg2pin[i].v);
        servo_attach(hexpod_leg2pin[i].h);
    }
}

static void hexpod_step()
{
    uint8_t i;
    hexpod_state_t* s = &hexpod.script[hexpod.curr++];
    
    if (hexpod.num_instructions == hexpod.curr) {
        hexpod.curr = 0;
    }
    
    LOG_INFO(HEXPOD, "running step %d", hexpod.curr);
    
    for (i=0; i < HEXPOD_NUM_LEGS; ++i) {
        const hexpod_leg_t* leg = &hexpod_leg2pin[i];
        
        servo_set(leg->v, s->leg[i].v);
        servo_set(leg->h, s->leg[i].h);
    }
}

void hexpod_load(hexpod_state_t* script, uint8_t n)
{
    hexpod.curr = 0;

    LOG_INFO(HEXPOD, "loading %d instructions", n);
    n = MIN(n, HEXPOD_MAX_INSTRUCTIONS);
    if (0 == n) {
        memset(hexpod.script, 0, sizeof *hexpod.script);
        hexpod.num_instructions = 1;
    }
    else {
        memcpy(hexpod.script, script, sizeof(*script) * n);
        hexpod.num_instructions = n;
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
