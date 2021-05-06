#include "ganymede.h"
#include "logging.h"
#include "servo.h"
#include "io.h"
#include "utils.h"
#include "mpu.h"
#include <math.h>

static uint16_t micros;

void setup()
{
    io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
    servo_init();

}

void init()
{
    servo_attach(10);

    mpu_init();

    LOG_INFO(QUAD_CALIB, "Enter command:");
    LOG_INFO(QUAD_CALIB, " h - set micros = 2000");
    LOG_INFO(QUAD_CALIB, " l - set micros = 1000");
    LOG_INFO(QUAD_CALIB, " + - add 50 micros to current value");
    LOG_INFO(QUAD_CALIB, " - - subtract 50 micros to current value");
    LOG_INFO(QUAD_CALIB, " v - toggle print vibrations command");

}

static float val[200];
void vibration_calc()
{
    uint8_t i;
    float sum = 0;
    float mean = 0;
    float std = 0;
    mpu_data_t data;

    LOG_INFO(QUAD_CALIB, "taking acc samples");
    // calculate variance of the accelaration vector:

    for (i=0; i < ARR_SIZE(val); ++i) {
        mpu_read(&data);
        val[i] = sqrt(pow(data.accel[0], 2) + pow(data.accel[1], 2) + pow(data.accel[2], 2));
        sum += val[i];
    }
    LOG_INFO(QUAD_CALIB, "calculating");
    mean = sum / ARR_SIZE(val);
    for (i=0; i < ARR_SIZE(val); ++i) {
        std += pow(val[i] - mean, 2);
    }
    std = sqrt(std);

    LOG_INFO(QUAD_CALIB, "%.06f", std);


}

void loop()
{
    char c[3];
    io_tx_t tx[] = {
        { .mode = IO_TX_MODE_W, .off = 0, .len = 1, .buf = "\r"},
        { .mode = IO_TX_MODE_R, .off = 0, .len = 1, .buf = c},
        { .mode = IO_TX_MODE_W, .off = 0, .len = sizeof "COMMAND: ", .buf = "COMMAND: "},
        { .mode = IO_TX_MODE_W, .off = 0, .len = 3, .buf = c},
    };

    c[0] = '\0';
    c[1] = '\n';
    c[2] = '\r';


    io_uart_sg(0, tx, ARR_SIZE(tx), 0);

    switch (c[0]) {
        case 'h': micros = 2000; break;
        case 'l': micros = 1000; break;
        case '+': micros += 50; break;
        case '-': micros -= 50; break;
        case 'v': vibration_calc(); return;
        default: LOG_INFO(QUAD_CALIB, "unknown command: '%c'", c[0]); return;
    }

    micros = MIN(micros, 2000);
    micros = MAX(micros, 1000);
    LOG_INFO(QUAD_CALIB, "setting %u micros", micros);
    servo_set_mircoseconds(10, micros);
}


TASK(STACK_SIZE);
