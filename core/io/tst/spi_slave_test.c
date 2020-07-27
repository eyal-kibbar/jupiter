#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include <avr/io.h>
#include "utils.h"
#define DD_SS       DDB2

void setup()
{
    io_uart_init(9600);
    io_logging_init();
    io_spi_slave_init();
}

void loop()
{
    uint8_t str[4] = {'a', 'b', 'c', '\0'};
    uint8_t str_s[4] = {'a', 'b', 'c', '\0'};
    io_tx_t tx[] = {
        {.mode = IO_TX_MODE_R, .len = 3, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_W, .len = 3, .off = 0, .buf = str_s},
    };

    //c = 'B';
    //LOG_INFO(SPI_SLAV, "sending: %c", (char)c);
    io_spi_slave_sg(tx, ARR_SIZE(tx), 0);

    LOG_INFO(SPI_SLAV, "got0: %s", (char*)str); // expecting ABC
    LOG_INFO(SPI_SLAV, "got1: %s", (char*)str_s); // expecting abc
    LOG_INFO(SPI_SLAV, "---------------------");
    //gmd_delay(200);
}



TASK(STACK_SIZE);
