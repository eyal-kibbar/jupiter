#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "utils.h"

#include <avr/io.h>

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();

    io_i2c_master_init();
}

void loop()
{
    uint8_t str[] = { 'A', 'B', 'C', '\0'};
    uint8_t str_r[] = { 'X', 'Y', 'Z', '\0'};
    io_tx_t tx[] = {
        {.mode = IO_TX_MODE_W, .len = 3, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_R, .len = 3, .off = 0, .buf = str_r},
    };

    //c = 'A';
    LOG_INFO(SPI_MSTR, "sending: %s", (char*)str);


    io_i2c_tx_begin(0);
    io_i2c_master_sg(tx, 1, 0);
    io_i2c_tx_end();

    LOG_INFO(I2C_MSTR, "got0: %s", (char*)str); // expecting ABC
    LOG_INFO(I2C_MSTR, "got1: %s", (char*)str_r); // expecting abc
    LOG_INFO(I2C_MSTR, "---------------------");
    gmd_delay(1000);
}



TASK(STACK_SIZE);
