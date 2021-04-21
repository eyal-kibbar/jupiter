#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"
#include "quad_protocol.h"
#include "quad.h"
#include "failsafe.h"

#define SS_PIN 2
#define CE_PIN 3

void setup()
{
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();
}

void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = CE_PIN,
        .irq_pin = 0xFF, // irq disconnected
        .channel = 76
    };
    LOG_INFO(RADIO_RX, "init nrf");
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    nrf_test();
}

float map(float v, float vmin, float vmax, float omin, float omax)
{
    return ((v * (omax - omin)) / (vmax - vmin)) + omin;
}

void loop()
{
    radio_pkt_t pkt;
    uint8_t payload_len = sizeof(pkt);
    uint8_t pipe_idx = 0;
    float setpoint[3];
    float throttel;
    int rc;

    failsafe_reset(1); // reset entire system if we lost communication

    //LOG_INFO(RADIO_RX, "receiving");
    if ((rc = nrf_recv((uint8_t*)&pkt, &payload_len, &pipe_idx))) {
        quad_stop(); // stop the quad if we failed to reveice a packet
    }

    if (pkt.switches & 0x1) {
        quad_start();
    }
    else {
        quad_stop();
    }

    setpoint[0] = map(pkt.j_right_x, 0, 1023, -45, +45);
    setpoint[1] = map(pkt.j_right_y, 0, 1023, -45, +45);
    setpoint[2] = map(pkt.j_left_x,  0, 1023, -180, +180);

    throttel = map(pkt.j_left_y, 0, 1024, 0, 1);

    quad_set_setpoint(setpoint, throttel);
}


TASK(STACK_SIZE);
