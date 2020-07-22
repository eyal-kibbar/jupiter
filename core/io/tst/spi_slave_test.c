#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include <avr/io.h>
#include "utils.h"
#define DD_SS       DDB2

void setup()
{
    io_spi_slave_init();
}
uint8_t prev = 0;
void loop_()
{
    uint8_t x;
    x = PINB & _BV(2);
    if (x != prev) {
        LOG_INFO(SPI_SLAV, "%d %2x", x, _BV(DD_SS));
        prev = x;
    }
}

void loop()
{
    uint8_t str[4] = {'a', 'b', 'c', '\0'};
    uint8_t str_s[4] = {'a', 'b', 'c', '\0'};
    io_tx_t tx[] = {
        {.mode = IO_TX_MODE_R, .len = 3, .off = 0, .buf = str},
        {.mode = IO_TX_MODE_W, .len = 3, .off = 0, .buf = str_s},
        {.mode = IO_TX_MODE_R | IO_TX_MODE_W | IO_TX_MODE_INLINE, .len = 1, .off = 0, .data = {'S'}},
    };

    //c = 'B';
    //LOG_INFO(SPI_SLAV, "sending: %c", (char)c);
    io_spi_slave_sg(tx, ARR_SIZE(tx), 0);

    LOG_INFO(SPI_SLAV, "got0: %s", (char*)str); // expecting ABC
    LOG_INFO(SPI_SLAV, "got1: %s", (char*)str_s); // expecting abc
    LOG_INFO(SPI_SLAV, "got2: %c", (char)tx[2].data[0]); // expecting 'M'
    LOG_INFO(SPI_SLAV, "---------------------");
    //gmd_delay(200);
}



TASK(STACK_SIZE);
