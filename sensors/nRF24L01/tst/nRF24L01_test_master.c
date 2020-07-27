#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"

#define SS_PIN 7

void setup()
{
    io_uart_init(9600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();
}


void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 4,
        .irq_pin = 5
    };
    nrf_init(&cfg);

    nrf_send_open_pipe("abcde", 5);
    nrf_send_set();
}

void loop()
{
    static int idx = 0;

    ++idx;

    nrf_send((uint8_t*)&idx, sizeof(idx));

    gmd_delay(1000);

}



TASK(STACK_SIZE);
