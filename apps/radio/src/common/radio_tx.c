#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "radio.h"
#include "io.h"


#define SS_PIN 10


radio_pkt_t pkt;


void setup()
{
    uint8_t i;

    io_uart_init(0, 9600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();

    for (i=2; i <= 7; ++i) {
        io_pin_input(i);
        io_pin_set(i); // pullup resister
    }

    io_analog_init();
}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 9,
        .irq_pin = 8,
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
    // read all inputs
    io_analog_read(0, IO_ANALOG_REF_AVcc, &pkt.pot0);
    io_analog_read(1, IO_ANALOG_REF_AVcc, &pkt.pot1);
    io_analog_read(2, IO_ANALOG_REF_AVcc, &pkt.j_right_x);
    io_analog_read(3, IO_ANALOG_REF_AVcc, &pkt.j_right_y);

    for (i=2; i <=7; ++i) {
        if (io_pin_get(i)) {
            pkt.buttons |= 1 << (i-2);
        }
        else {
            pkt.buttons &= ~(1 << (i-2));
        }
    }
    LOG_INFO(RADIO_TX, "sending pot0: %d", pkt.pot0);
    nrf_send((uint8_t*)&pkt, sizeof(pkt));

    //gmd_delay(10);
}


TASK(STACK_SIZE);
