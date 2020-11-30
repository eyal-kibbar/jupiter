#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"

#define SS_PIN 10

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
        .ce_pin = 9,
        .irq_pin = 0xFF,//8,
        .channel = 76
    };
    nrf_init(&cfg);

    nrf_send_open_pipe("abcde", 5);
    //nrf_powerup();
    nrf_send_set();

    //nrf_test();
}

void loop()
{
    int ack_pay;
    uint8_t ack_pay_size = sizeof(ack_pay);
    uint8_t ack_pay_pipe_idx;
    static int idx = 0;

    ++idx;

    LOG_INFO(NRF_MSTR, "sending %d", idx);
    nrf_send((uint8_t*)&idx, sizeof(idx));
    if (0 == nrf_recv((uint8_t*)&ack_pay, &ack_pay_size, &ack_pay_pipe_idx)) {
        LOG_INFO(NRF_MSTR, "got %d bytes: %d, pipe: %d", ack_pay_size, ack_pay, ack_pay_pipe_idx);
    }

    gmd_delay(10000);

}



TASK(STACK_SIZE);
