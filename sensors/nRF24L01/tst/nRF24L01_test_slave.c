#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"

#define SS_PIN 2


int sent_idx = 100;
void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();
}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 3,
        .irq_pin = 0xFF, //8,
        .channel = 76
    };
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    nrf_test();
    LOG_INFO(NTF_SLV, "waiting");
}

void loop()
{
    int idx = 0;

    uint8_t payload_len = sizeof(idx);
    uint8_t pipe_idx = 0;


    nrf_send_pending(1, (uint8_t*)&sent_idx, sizeof(sent_idx));
    nrf_recv((uint8_t*)&idx, &payload_len, &pipe_idx);

    LOG_INFO(NRF_SLV, "got: %d, pipe %d", idx, pipe_idx);

    ++sent_idx;
    //nrf_test();

    //while (1);

}



TASK(STACK_SIZE);
