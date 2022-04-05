#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "airplane_protocol.h"
#include "io.h"
#include "utils.h"

#include <math.h>

#define SS_PIN 6
#define LED_PIN 9

static const uint8_t switches_map[] = {7, 4};
static const uint8_t buttons_map[] = {2, 3, 8};

void setup()
{
    uint8_t i;

    io_uart_init(0, 57600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();

    for (i=0; i < ARR_SIZE(switches_map); ++i) {
        io_pin_input(switches_map[i]);
        io_pin_set(switches_map[i]); // pullup resister
    }

    for (i=0; i < ARR_SIZE(buttons_map); ++i) {
        io_pin_input(buttons_map[i]);
        io_pin_set(buttons_map[i]); // pullup resister
    }

    io_pin_output(LED_PIN);
    io_pin_clr(LED_PIN);
    io_analog_init();
}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 5,
        .irq_pin = 0xFF, // no IRQ
        .channel = 76
    };

    nrf_init(&cfg);


    nrf_send_open_pipe("abcde", 5);
    nrf_send_set();
    nrf_test();
}

void loop()
{
    uint8_t i;
    int rc;
    radio_pkt_t pkt;
    airplane_response_pkt_t resp_pkt;
    uint8_t resp_pipe_idx;
    uint8_t resp_pkt_size;

    // read all inputs
    //io_analog_read(7, IO_ANALOG_REF_AVcc, &pkt->pot_left);
    //io_analog_read(6, IO_ANALOG_REF_AVcc, &pkt->pot_right);
    io_analog_read(0, IO_ANALOG_REF_AVcc, &pkt.j_left_y);
    io_analog_read(1, IO_ANALOG_REF_AVcc, &pkt.j_left_x);
    io_analog_read(2, IO_ANALOG_REF_AVcc, &pkt.j_right_x);
    io_analog_read(3, IO_ANALOG_REF_AVcc, &pkt.j_right_y);

    for (i=0; i < ARR_SIZE(switches_map); ++i) {
        uint8_t switch_status = io_pin_get(switches_map[i]);
        pkt.switches &= ~(1 << i);
        pkt.switches |= (!!switch_status) << i;
    }

    for (i=0; i < ARR_SIZE(buttons_map); ++i) {
        uint8_t button_status = io_pin_get(buttons_map[i]);
        pkt.buttons &= ~(1 << i);
        pkt.buttons |= (!!button_status) << i;
    }

    //LOG_INFO(AIRPLANE_REMOTE, "sending switches: %02x left: %d right: %d ", pkt.switches, pkt.j_left_y, pkt.j_right_y);
    if ((rc = nrf_send((uint8_t*)&pkt, sizeof(pkt)))) {
        io_pin_clr(LED_PIN);
        return;
    }

    // send succeeded + got an ACK packet
    io_pin_set(LED_PIN);


    // receive the quad respose packet

    resp_pkt_size = sizeof resp_pkt;
    if (0 == nrf_recv((uint8_t*)&resp_pkt, &resp_pkt_size, &resp_pipe_idx)) {
        LOG_INFO(AIRPLANE_REMOTE, "got %d bytes, pipe %d:%.02f,%.02f,%.02f,%04d,%04d,%04d,%04d",
            resp_pkt_size, resp_pipe_idx,
            resp_pkt.angles[0], resp_pkt.angles[1], resp_pkt.angles[2],
            resp_pkt.motor[0], resp_pkt.motor[1], resp_pkt.motor[2], resp_pkt.motor[3]);
    }


}


TASK(STACK_SIZE);
