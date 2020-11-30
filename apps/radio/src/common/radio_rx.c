#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "radio.h"
#include "io.h"
#include "servo.h"

#define SS_PIN 10


radio_pkt_t pkt;


void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();

    servo_init();
    servo_attach(2);
}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 9,
        .irq_pin = 8,
        .channel = 76
    };
    LOG_INFO(RADIO_RX, "init nrf");
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    nrf_test();
}

uint16_t map(uint32_t v, uint16_t vmin, uint16_t vmax, uint16_t omin, uint16_t omax)
{
    return (uint16_t)(((v * (vmax - vmin)) / (omax - omin)) + omin);
}

void loop()
{
    uint8_t payload_len = sizeof(pkt);
    uint8_t pipe_idx = 0;

    //LOG_INFO(RADIO_RX, "receiving");
    nrf_recv((uint8_t*)&pkt, &payload_len, &pipe_idx);

    LOG_INFO(RADIO_RX, "got: %d, pipe %d", pkt.pot0, pipe_idx);

    servo_set_mircoseconds(2, map(pkt.pot0, 0, 1024, 500, 2000));

}


TASK(STACK_SIZE);
