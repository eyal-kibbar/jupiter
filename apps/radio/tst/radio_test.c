#include "ganymede.h"
#include "io.h"
#include "logging.h"

#include <avr/io.h>

#define POT_MAX 1000
#define POT_MIN 24

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    io_analog_init();
}

void init()
{
    LOG_INFO(RADIO, "init");
}

uint16_t v;

uint16_t map(uint16_t v, uint16_t vmin, uint16_t vmax, uint16_t omin, uint16_t omax)
{
    return (v - vmin) * ((omax - omin) / (vmax - vmin)) + omin;
}

void loop()
{
    uint16_t val = 0;

    io_analog_read(0, IO_ANALOG_REF_AVcc, &val);
    //if (POT_MIN < val && val < POT_MAX) {
        //v = map(val, 0, 1024, 0, 255);
    //}
    LOG_INFO(RADIO, "val0: %d", val);
    //gmd_delay(20);
}



TASK(STACK_SIZE);
