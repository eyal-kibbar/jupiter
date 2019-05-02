#include "ganymede.h"
#include "clk.h"
#include "libclk.h"


#if CLK_RESOLUTION_MS > 60000
#error "clock resultion must be under 1 minute"
#endif

typedef struct clk_s {
    uint16_t extra_us;
    uint16_t carry_us;
    uint16_t tick_us;

    uint8_t extra_ms;
    uint8_t carry_ms;
    uint8_t tick_ms;

    uint8_t nticks;
    uint8_t curtick;
} clk_t;

static clk_t clk;


void setup()
{
    uint16_t ms = 0;


    for (clk.nticks = 0;
         ms < CLK_RESOLUTION_MS;
         ms = gmd_ticks2ms(++clk.nticks, &clk.extra_us));

    ms -= CLK_RESOLUTION_MS;
    clk.extra_ms = (uint8_t)ms;
    clk.tick_ms = gmd_ticks2ms(1, &clk.tick_us);

    clk.curtick = gmd_curtick();
}

void loop()
{
    uint8_t next_tick;
    uint8_t ticks;

    // start with the previous tick
    next_tick = clk.curtick;

    // add the number of ticks for a full resolution
    next_tick += clk.nticks;

    // add the carry ms and us
    clk.carry_us += clk.extra_us;
    if (1000 <= clk.carry_us) {
        clk.carry_us -= 1000;
        ++clk.carry_ms;
    }
    clk.carry_ms += clk.extra_ms;

    // check if we carried enough time to add a tick
    if (clk.tick_ms < clk.carry_ms ||
        (clk.tick_ms < clk.carry_ms && clk.tick_us <= clk.carry_us)) {

        clk.carry_ms -= clk.tick_ms;
        if (clk.carry_us < clk.tick_us) {
            --clk.carry_ms;
            clk.carry_us += 1000;
        }

        clk.carry_us -= clk.tick_us;

        --next_tick;
    }
    clk.curtick = next_tick;

    clk_tick(CLK_RESOLUTION_MS);

    ticks = next_tick - gmd_curtick();
    gmd_delay(gmd_ticks2ms(ticks, NULL));
}


TASK(STACK_SIZE);
