#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "radio.h"
#include "io.h"
#include "utils.h"

#include <math.h>

#define SS_PIN 6
#define LED_PIN 9

radio_pkt_t pkts;

static const uint8_t switches_map[] = {7, 4};
static const uint8_t buttons_map[] = {2, 3, 8};

void setup()
{
    uint8_t i;

    io_uart_init(0, 9600);
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
    radio_pkt_t *pkt = &pkts;
    uint32_t std;

    // read all inputs
    //io_analog_read(0, IO_ANALOG_REF_AVcc, &pkt->pot0);
    //io_analog_read(1, IO_ANALOG_REF_AVcc, &pkt->pot1);
    io_analog_read(0, IO_ANALOG_REF_AVcc, &pkt->j_left_y);
    io_analog_read(1, IO_ANALOG_REF_AVcc, &pkt->j_left_x);
    io_analog_read(2, IO_ANALOG_REF_AVcc, &pkt->j_right_x);
    io_analog_read(3, IO_ANALOG_REF_AVcc, &pkt->j_right_y);

    for (i=0; i < ARR_SIZE(switches_map); ++i) {
        uint8_t switch_status = io_pin_get(switches_map[i]);
        pkt->switches &= ~(1 << i);
        pkt->switches |= (!!switch_status) << i;
    }

    for (i=0; i < ARR_SIZE(buttons_map); ++i) {
        uint8_t button_status = io_pin_get(buttons_map[i]);
        pkt->buttons &= ~(1 << i);
        pkt->buttons |= (!!button_status) << i;
    }


    if ((rc = nrf_send((uint8_t*)pkt, sizeof(*pkt)))) {
        io_pin_clr(LED_PIN);
    }
    else {
        io_pin_set(LED_PIN);
    }

    LOG_INFO(RADIO_TX, "sending switches: %02x left x: %d left y: %d ", pkt->switches, pkt->j_left_x, pkt->j_left_y);


}


TASK(STACK_SIZE);
