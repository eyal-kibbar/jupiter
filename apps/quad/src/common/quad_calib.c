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
    io_uart_init(0, 9600);
    io_logging_init();
    io_i2c_master_init();
    servo_init();

}

void init()
{
    servo_attach(4);
    servo_attach(5);
    servo_attach(6);
    servo_attach(7);


    servo_set_mircoseconds(4, 2000);
    servo_set_mircoseconds(5, 2000);
    servo_set_mircoseconds(6, 2000);
    servo_set_mircoseconds(7, 2000);

    gmd_delay(2000);

    servo_set_mircoseconds(4, 1000);
    servo_set_mircoseconds(5, 1000);
    servo_set_mircoseconds(6, 1000);
    servo_set_mircoseconds(7, 1000);

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
    char c[4];
    uint8_t motor;
    io_tx_t tx[] = {
        { .mode = IO_TX_MODE_W, .off = 0, .len = 1, .buf = "\r"},
        { .mode = IO_TX_MODE_R, .off = 0, .len = 2, .buf = c},
        { .mode = IO_TX_MODE_W, .off = 0, .len = sizeof "COMMAND: ", .buf = "COMMAND: "},
        { .mode = IO_TX_MODE_W, .off = 0, .len = 4, .buf = c},
    };

    c[0] = '\0';
    c[1] = '\0';
    c[2] = '\n';
    c[3] = '\r';


    io_uart_sg(0, tx, ARR_SIZE(tx), 0);

    switch (c[1]) {
        case 'h': micros = 2000; break;
        case 'l': micros = 1000; break;
        case '+': micros += 50; break;
        case '-': micros -= 50; break;
        case 'v': vibration_calc(); return;
        default: LOG_INFO(QUAD_CALIB, "unknown command: '%c'", c[0]); return;
    }

    switch (c[0]) {
        case '4': motor = 4; break;
        case '5': motor = 5; break;
        case '6': motor = 6; break;
        case '7': motor = 7; break;
        default: LOG_INFO(QUAD_CALIB, "unknown command: '%c'", c[0]); return;
    }

    micros = MIN(micros, 2000);
    micros = MAX(micros, 1000);
    LOG_INFO(QUAD_CALIB, "setting %u micros", micros);
    servo_set_mircoseconds(motor, micros);
}


TASK(STACK_SIZE);
