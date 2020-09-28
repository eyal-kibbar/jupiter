#include "ganymede.h"
#include "io.h"
#include "utils.h"
#include "logging.h"


void setup()
{
    io_uart_init(0, 9600);
    io_uart_init(1, 9600);
    io_logging_init();
}

void init()
{
    LOG_INFO(UART, "initializing");
}

void loop()
{
    char* c = "A\n\r";
    io_tx_t tx[] = {
        {
            .mode = IO_TX_MODE_R,
            .off = 0,
            .len = 1,
            .buf = c
        },
        {
            .mode = IO_TX_MODE_W,
            .off = 0,
            .len = 3,
            .buf = c
        },
    };

    io_uart_sg(0, &tx[0], 2, 0);

    LOG_INFO(UART, "testing 123");
}



TASK(STACK_SIZE);
