#include "ganymede.h"
#include "logging.h"
#include "io.h"

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
        {.mode = IO_TX_MODE_W , .len = 4, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_R , .len = 4, .off = 0, .buf = str_r}
    };

    //c = 'A';
    LOG_INFO(SPI_MSTR, "sending: %s", (char*)str);


    //io_pin_clr(SS_PIN);
    io_spi_master_sg(SS_PIN, tx, 2, 0);
    //io_pin_set(SS_PIN);

    LOG_INFO(SPI_MSTR, "got: %s", (char*)str_r);
    gmd_delay(1000);
}



TASK(STACK_SIZE);
