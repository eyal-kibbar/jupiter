#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"
#include "airplane_protocol.h"
#include "airplane.h"
#include "failsafe.h"
#include <string.h>
#include "arrow.h"


#define SS_PIN 8
#define CE_PIN 9

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
        .irq_pin = 0xFF,
        .channel = 76
    };
    LOG_INFO(RADIO_RX, "init nrf");
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    //nrf_test();
}

void loop()
{
    radio_pkt_t pkt;
    airplane_response_pkt_t resp_pkt;
    uint8_t payload_len = sizeof(pkt);
    uint8_t pipe_idx = 0;
    int rc;

    //failsafe_reset(1); // reset entire system if we lost communication

    memset(&resp_pkt, 0, sizeof resp_pkt);

    // set the respose packet
    nrf_send_pending(1, (uint8_t*)&resp_pkt, sizeof(resp_pkt));

    //LOG_INFO(RADIO_RX, "receiving");
    if ((rc = nrf_recv((uint8_t*)&pkt, &payload_len, &pipe_idx))) {
        // TODO: failed to receive packet
        return;
    }

    arrow_set(pkt.j_right_x, pkt.j_right_y, pkt.j_left_y);
    arrow_autobalance(!!(pkt.switches & 0x1));
}


TASK(STACK_SIZE);
