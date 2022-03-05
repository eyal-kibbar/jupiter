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

void clk_diff(const clk_time_t* a, const clk_time_t* b, clk_time_t* diff)
{
    uint8_t carry;

    if (b->millisecond < a->millisecond) {
        carry = 1;
        diff->millisecond = 1000+b->millisecond - a->millisecond;
    }
    else {
        carry = 0;
        diff->millisecond = b->millisecond - a->millisecond;
    }


    if (b->second < (a->second+carry)) {
        diff->second = 60+b->second - a->second - carry;
        carry = 1;
    }
    else {
        diff->second = b->second - a->second - carry;
        carry = 0;
    }


    if (b->minute < (a->minute+carry)) {
        diff->minute = 60+b->minute - a->minute - carry;
        carry = 1;
    }
    else {
        diff->minute = b->minute - a->minute - carry;
        carry = 0;
    }


    if (b->hour < (a->hour+carry)) {
        diff->hour = 24+b->hour - a->hour - carry;
        carry = 1;
    }
    else {
        diff->hour = b->hour - a->hour - carry;
        carry = 0;
    }


    if (b->day < (a->day+carry)) {
        diff->day = 365+b->day - a->day - carry;
        carry = 1;
    }
    else {
        diff->day = b->day - a->day - carry;
        carry = 0;
    }


    diff->year = b->year - a->year - carry;

}
