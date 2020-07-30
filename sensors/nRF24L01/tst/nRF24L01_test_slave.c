#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"

void setup()
{
    io_uart_init(9600);
    io_logging_init();
    io_spi_slave_init();
}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = 3,
        .ce_pin = 4,
        .irq_pin = 5
    };
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
}

void loop()
{
    int idx = 0;
    uint8_t payload_len = sizeof(idx);
    uint8_t pipe_idx = 0;


    nrf_recv((uint8_t*)&idx, &payload_len, &pipe_idx);

    LOG_INFO(NRF_SLV, "got: %d, pipe %d", idx, pipe_idx);

}



TASK(STACK_SIZE);
