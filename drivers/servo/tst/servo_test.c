#include "servo.h"
#include "ganymede.h"
#include "io.h"
#include "logging.h"

#define MAX_MICROSECONDS 2250
#define MIN_MICROSECONDS 500
#define SETP_MICROSECONDS 16

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    servo_init();

    servo_attach(2);
    servo_attach(3);
    //servo_attach(4);


}

uint16_t microseconds;
int16_t dir;

void loop()
{
    microseconds += dir*SETP_MICROSECONDS;

    if (microseconds < MIN_MICROSECONDS) {
        microseconds = MIN_MICROSECONDS;
        dir = 1;
    }
    else if (MAX_MICROSECONDS < microseconds) {
        microseconds = MAX_MICROSECONDS;
        dir = -1;
    }

    LOG_INFO(SERVO, "microseconds %d", microseconds);
    servo_set_mircoseconds(2, microseconds);

    gmd_delay(500);
}


TASK(STACK_SIZE);
