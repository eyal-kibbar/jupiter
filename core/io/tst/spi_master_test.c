#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "utils.h"

extern void io_pin_set(uint8_t);
extern void io_pin_clr(uint8_t);

#include <avr/io.h>

#define SS_PIN 7

void setup()
{
    io_spi_master_init();
    DDRD |= _BV(SS_PIN);
    io_pin_set(SS_PIN);
}

void loop_()
{
    io_pin_set(SS_PIN);
    gmd_delay(1000);
    io_pin_clr(SS_PIN);
    gmd_delay(1000);
}

void loop()
{
    uint8_t str[] = { 'A', 'B', 'C', '\0'};
    uint8_t str_r[] = { 'X', 'Y', 'Z', '\0'};
    io_tx_t tx[] = {
        {.mode = IO_TX_MODE_W, .len = 3, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_R | IO_TX_MODE_W, .len = 3, .off = 0, .buf = str_r},
        {.mode = IO_TX_MODE_R | IO_TX_MODE_W | IO_TX_MODE_INLINE, .len = 1, .off = 0, .data = {'M', 'm'}},
    };

    //c = 'A';
    LOG_INFO(SPI_MSTR, "sending: %s", (char*)str);


    io_spi_tx_begin(SS_PIN);
    io_spi_master_sg(tx, ARR_SIZE(tx), 0);
    io_spi_tx_end();

    LOG_INFO(SPI_MSTR, "got0: %s", (char*)str); // expecting ABC
    LOG_INFO(SPI_MSTR, "got1: %s", (char*)str_r); // expecting abc
    LOG_INFO(SPI_MSTR, "got2: %c", (char)tx[2].data[0]); // expecting 'S'
    LOG_INFO(SPI_MSTR, "---------------------");
    gmd_delay(1000);
}



TASK(STACK_SIZE);
