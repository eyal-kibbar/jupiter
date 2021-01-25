#include "servo.h"
#include "ganymede.h"
#include "io.h"
#include "logging.h"

#define MAX_MICROSECONDS 2250
#define MIN_MICROSECONDS 500
#define SETP_MICROSECONDS 50


uint16_t micros[4] = {500, 1000, 1500, 2000};
int16_t dirs[4] = { 1, -1, 1, -1};
uint8_t pins[4] = {4, 5, 6, 7};

const int used_pins = 4;

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();

}

void init()
{
    int i;
    servo_init();
    for (i=0; i < used_pins; ++i) {
        servo_attach(pins[i]);
    }
}

void loop()
{
    int i;
    for (i=0; i < used_pins; ++i) {
        micros[i] += dirs[i] * SETP_MICROSECONDS;
        if (micros[i] < MIN_MICROSECONDS) {
            micros[i] = MIN_MICROSECONDS;
            dirs[i] = 1;
        }
        else if (micros[i] > MAX_MICROSECONDS) {
            micros[i] = MAX_MICROSECONDS;
            dirs[i] = -1;
        }

        LOG_INFO(SERVO, "microseconds %d", micros[i]);
        servo_set_mircoseconds(pins[i], micros[i]);

    }

    gmd_delay(100);
    //LOG_INFO(SERVO, "microseconds1 %d microseconds2: %d", microseconds1, microseconds2);
    //gmd_delay(10);
}


TASK(STACK_SIZE);
