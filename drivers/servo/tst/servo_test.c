#include "servo.h"
#include "ganymede.h"
#include "io.h"
#include "logging.h"

#define MAX_MICROSECONDS 2250
#define MIN_MICROSECONDS 500
#define SETP_MICROSECONDS 20

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    servo_init();
}

void init()
{
    servo_attach(4);
    servo_attach(5);
}

uint16_t microseconds1 = 0;
uint16_t microseconds2 = ~0;
int16_t dir1;
int16_t dir2;

void loop()
{
    microseconds1 += dir1*SETP_MICROSECONDS;

    if (microseconds1 < MIN_MICROSECONDS) {
        microseconds1 = MIN_MICROSECONDS;
        dir1 = 1;
    }
    else if (MAX_MICROSECONDS < microseconds1) {
        microseconds1 = MAX_MICROSECONDS;
        dir1 = -1;
    }

    microseconds2 += dir2*SETP_MICROSECONDS;

    if (microseconds2 < MIN_MICROSECONDS) {
        microseconds2 = MIN_MICROSECONDS;
        dir2 = 1;
    }
    else if (MAX_MICROSECONDS < microseconds2) {
        microseconds2 = MAX_MICROSECONDS;
        dir2 = -1;
    }


    LOG_INFO(SERVO, "microseconds1 %d microseconds2: %d", microseconds1, microseconds2);
    servo_set_mircoseconds(4, microseconds1);
    servo_set_mircoseconds(5, microseconds2);

    //gmd_delay(10);
}


TASK(STACK_SIZE);
