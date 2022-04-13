#include "ganymede.h"
#include "io.h"
#include "io_platform.h"
#include "logging.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define LIGHT_DURATION_MS (30*1000)

#define PIN_LIGHT_SENSOR 0
#define PIN_RELAY 8

volatile uint8_t movement_detected;

void setup()
{
#ifdef DEBUG
    io_uart_init(0, 57600);
    io_logging_init();
#endif

    io_analog_init();

    io_pin_input(3);
    io_pin_output(PIN_RELAY);

    io_pin_clr(PIN_RELAY);
    //io_pin_set(SS_PIN);

    EICRA = _BV(ISC11) | _BV(ISC10); // raising edge user interrupt 1
    EIMSK = _BV(INT1); // enable interrupt 1
}


void init()
{

}

ISR(INT1_vect)
{
    movement_detected = 1;
}



void loop()
{
    uint8_t pin = 0;
    uint16_t light = 0;

    // wait for movement interrupt
    gmd_wfe(&movement_detected, 0xFF, 0, 0);
    movement_detected = 0;

    // check it is dark outside
    io_analog_read(PIN_LIGHT_SENSOR, IO_ANALOG_REF_AVcc, &light);
    if (light > LIGHT_SENSOR_THRESH) {
        return; // no need to turn on light
    }

    // turn on light
    io_pin_set(PIN_RELAY);


    do {
        movement_detected = 0;
        gmd_wfe(&movement_detected, 0xFF, 0, LIGHT_DURATION_MS); // enable light for 30 seconds
        LOG_INFO(SMART_LIGHT, "movement detected %d", movement_detected);
        // reset timer in case we reached the end and we still see movement
        // or if the movement stopped, and then continued
    } while (io_pin_get(3) || movement_detected);

    // turn off light
    io_pin_clr(PIN_RELAY);
    //pin = io_pin_get(3);


    LOG_INFO(SMART_LIGHT, "%04d %04d", pin, light);
    //gmd_delay(200);
}


TASK(STACK_SIZE);
