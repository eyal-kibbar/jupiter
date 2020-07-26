#include "ganymede.h"
#include "io.h"
#include "utils.h"


void setup()
{
    io_uart_init(9600);
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

    io_uart_sg(&tx[0], 2, 0);
    //io_uart_sg(&tx[1], 1, 0);
    //gmd_delay(1000);
}



TASK(STACK_SIZE);
