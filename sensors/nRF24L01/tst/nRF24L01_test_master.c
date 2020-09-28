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
        .irq_pin = 5,
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
    static int idx = 0;

    //while(1);
    ++idx;

    LOG_INFO(NRF_MSTR, "sending %d", idx);
    nrf_send((uint8_t*)&idx, sizeof(idx));
    //nrf_test();
    gmd_delay(3000);

}



TASK(STACK_SIZE);
