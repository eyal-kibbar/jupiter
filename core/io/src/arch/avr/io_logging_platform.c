#include "io.h"
#include <stdio.h>

static int io_logging_uart_putchar(char c, FILE *stream)
{
    io_tx_t tx = {
        .mode = IO_TX_MODE_W,
        .buf = (uint8_t*)&c,
        .len = 1,
        .off = 0
    };

    io_uart_sg(0, &tx, 1, 0);
    return 1;
}


static FILE io_logging_stream = FDEV_SETUP_STREAM(
    io_logging_uart_putchar,
    NULL,
    _FDEV_SETUP_WRITE);

void io_logging_init()
{
    stdout = &io_logging_stream;
}
