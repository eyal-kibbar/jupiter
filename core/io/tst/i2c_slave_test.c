#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "utils.h"

#include <avr/io.h>

void setup()
{
    io_uart_init(9600);
    io_logging_init();

    io_i2c_slave_init(0);
}

void loop()
{
    uint8_t str[] = { 'a', 'b', 'c', '\0'};
    uint8_t str_w[] = { 'x', 'y', 'z', '\0'};
    io_tx_t tx[] = {
        {.mode = IO_TX_MODE_R, .len = 3, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_W, .len = 3, .off = 0, .buf = str_w},
    };

    LOG_INFO(I2C_SLV, "waiting");
    io_i2c_slave_sg(tx, 1, 0);

    LOG_INFO(I2C_SLV, "got0: %s", (char*)str); // expecting ABC
    LOG_INFO(I2C_SLV, "got1: %s", (char*)str_w); // expecting abc
    LOG_INFO(I2C_SLV, "---------------------");
}



TASK(STACK_SIZE);
