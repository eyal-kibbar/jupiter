#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "utils.h"

#include <avr/io.h>

#define SS_PIN 7

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();

    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);

    io_spi_master_init();

}

void loop()
{
    uint8_t str[] = { 'A', 'B', 'C', '\0'};
    uint8_t str_r[] = { 'X', 'Y', 'Z', '\0'};
    io_tx_t tx[] = {
        {.mode = IO_TX_MODE_W, .len = 3, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_R | IO_TX_MODE_W, .len = 3, .off = 0, .buf = str_r},
    };

    //c = 'A';
    LOG_INFO(SPI_MSTR, "sending: %s", (char*)str);


    io_spi_tx_begin(SS_PIN);
    io_spi_master_sg(tx, ARR_SIZE(tx), 0);
    io_spi_tx_end();

    LOG_INFO(SPI_MSTR, "got0: %s", (char*)str); // expecting ABC
    LOG_INFO(SPI_MSTR, "got1: %s", (char*)str_r); // expecting abc
    LOG_INFO(SPI_MSTR, "---------------------");
    gmd_delay(1000);
}



TASK(STACK_SIZE);
