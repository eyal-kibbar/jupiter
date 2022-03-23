#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "io_platform.h"
#include "mpu.h"
#include "math.h"
#include <avr/io.h>
#include "utils.h"

char s[80];
uint16_t t;


void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
}

void init()
{
    mpu_init();
    // initialize 16bit clock with 64 prescaler
    // this gives us a 4 microseconds per tick: (16Mhz/64)^-1
    TCCR1B = _BV(CS11) | _BV(CS10);
    TCNT1 = 0;

    mpu_calibrate();
}



void loop()
{

    io_tx_t tx[] = {
        { .mode = IO_TX_MODE_W, .off = 0, .len = 2, .buf = "\r"},
        { .mode = IO_TX_MODE_W, .off = 0, .len = sizeof s, .buf = s},
    };

    float ypr[3];

    TCNT1 = 0; t = 0;
    mpu_ypr(ypr);
    t = TCNT1;

    snprintf(s, sizeof s, "(%03.02f, %03.02f, %03.02f)  %d     ", ypr[0], ypr[1], ypr[2], t*4);
    io_uart_sg(0, tx, ARR_SIZE(tx), 0);

    //LOG_INFO(MPU_TEST, "(%03.02f, %03.02f, %03.02f)", yaw, pitch, roll);
    gmd_delay(50);
}

TASK(STACK_SIZE);
