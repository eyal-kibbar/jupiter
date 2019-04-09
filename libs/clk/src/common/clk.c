#include "clk.h"
#include "libclk.h"

static clk_time_t clk;

void clk_uptime(clk_time_t* out)
{
    *out = clk;
}

void clk_tick(uint16_t ms)
{
    uint16_t s;
    clk.millisecond += ms;
    if (clk.millisecond < 1000) {
        return;
    }

    s = clk.millisecond / 1000;
    clk.millisecond -= s*1000;
    clk.second += s;
    if (clk.second < 60) {
        return;
    }

    s = clk.second / 60;
    clk.second -= s*60;
    clk.minute += s;
    if (clk.minute < 60) {
        return;
    }

    s = clk.minute / 60;
    clk.minute -= s*60;
    clk.hour += s;
    if (clk.hour < 24) {
        return;
    }

    s = clk.hour / 24;
    clk.hour -= s*24;
    clk.day += s;
    if (clk.day < 365) {
        return;
    }

    s = clk.day / 365;
    clk.day -= s*365;
    clk.year += s;
}
