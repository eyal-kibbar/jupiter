#include "io.h"
#include <stdio.h>

static int io_logging_uart_putchar(char c, FILE *stream)
{
    static uint8_t log_buffer[LOG_BUFFER_SIZE];
    static uint8_t log_buffer_idx = 0;

    log_buffer[log_buffer_idx++] = (uint8_t)c;

    // flush
    if (c == '\r' || LOG_BUFFER_SIZE == log_buffer_idx) {
        io_tx_t tx = {
            .mode = IO_TX_MODE_W,
            .buf = (uint8_t*)log_buffer,
            .len = log_buffer_idx,
            .off = 0
        };
        io_uart_sg(LOG_UART_CORE_IDX, &tx, 1, 0);
        log_buffer_idx = 0;
    }
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
