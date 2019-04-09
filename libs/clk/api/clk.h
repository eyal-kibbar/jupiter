#ifndef CLK_H_
#define CLK_H_

#include <stdint.h>

typedef struct clk_time_s {
    uint16_t day;
    uint16_t millisecond;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t year;
} clk_time_t;

void clk_uptime(clk_time_t* out);

#endif /* CLK_H_ */
