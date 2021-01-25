#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "radio.h"
#include "io.h"
#include "servo.h"

#define SS_PIN 2


radio_pkt_t pkt;


void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();

    servo_init();

}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 3,
        .irq_pin = 0xFF, // irq disconnected
        .channel = 76
    };
    LOG_INFO(RADIO_RX, "init nrf");
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    nrf_test();

    servo_attach(4);
    servo_attach(5);
    servo_attach(6);
    servo_attach(7);

}

uint16_t map(uint32_t v, uint16_t vmin, uint16_t vmax, uint16_t omin, uint16_t omax)
{
    return (uint16_t)(((v * (omax - omin)) / (vmax - vmin)) + omin);
}

void loop()
{
    uint8_t payload_len = sizeof(pkt);
    uint8_t pipe_idx = 0;

    //LOG_INFO(RADIO_RX, "receiving");
    nrf_recv((uint8_t*)&pkt, &payload_len, &pipe_idx);

    /*
    LOG_INFO(RADIO_RX, "got: %04dx%04d %04dx%04d, %02x pipe %d",
        pkt.j_left_x, pkt.j_left_y,
        pkt.j_right_x, pkt.j_right_y,
        pkt.switches,
        pipe_idx);

        */
    LOG_INFO(RADIO_RX, "%04d(%4d) x %04d(%4d)   %04d(%4d) x %04d(%4d)",
        map(pkt.j_right_x, 0, 1024, 500, 2250), pkt.j_right_x,
        map(pkt.j_right_y, 0, 1024, 500, 2250), pkt.j_right_y,
        map(pkt.j_left_x, 0, 1024, 500, 2250), pkt.j_left_x,
        map(pkt.j_left_y, 0, 1024, 500, 2250), pkt.j_left_y);

    servo_set_mircoseconds(4, map(pkt.j_right_x, 0, 1024, 500, 2250));
    servo_set_mircoseconds(5, map(pkt.j_right_y, 0, 1024, 500, 2250));
    servo_set_mircoseconds(6, map(pkt.j_left_x, 0, 1024, 500, 2250));
    servo_set_mircoseconds(7, map(pkt.j_left_y, 0, 1024, 500, 2250));

}


TASK(STACK_SIZE);
