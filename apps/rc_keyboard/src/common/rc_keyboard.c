#include "ganymede.h"
#include "io.h"
#include <avr/io.h>
#include <string.h>
#include "logging.h"

uint8_t curr_state = 0;
uint8_t pin_mask[4];
uint8_t mask;
uint8_t* port;

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
}

void init()
{
    uint8_t i;
    const uint8_t pins[] = {3, 4, 5, 6};
    // configure pins and pullups

    for (i=0; i < 4; ++i) {
        io_pin_input(pins[i]); // set input
        io_pin_set(pins[i]);   // set pullup
        io_pin_port(pins[i], &port, &pin_mask[i]);
        mask |= pin_mask[i];
    }

    curr_state = mask;
}

void presskey(uint8_t keycode)
{
    uint8_t buf[8];

    uint8_t state;
    io_tx_t tx = {
        .mode = IO_TX_MODE_W,
        .len  = sizeof(buf),
        .off  = 0,
        .buf  = buf
    };


    memset(buf, 0, sizeof(buf));
    buf[2] = keycode;

    io_uart_sg(0, &tx, 1, 0);

    //LOG_INFO(RC, "presskey %x", keycode);
}

void releasekey()
{
    uint8_t release_cmd[8] = {0};
    io_tx_t tx = {
        .mode = IO_TX_MODE_W,
        .len  = sizeof(release_cmd),
        .off  = 0,
        .buf  = release_cmd,
    };

    io_uart_sg(0, &tx, 1, 0);

    //LOG_INFO(RC, "release");
}


void loop()
{
    uint8_t i;
    //LOG_INFO(RC, "PIND: %x DDRD: %x PORTD: %x MCUCR: %x", PIND & 0x8, DDRD, PORTD, MCUCR & 0x10);
    //gmd_delay(1000);
    gmd_wfe(port, mask, curr_state, 0);
    curr_state = (*port) & mask;
    //LOG_INFO(RC, "curr_state: %x", curr_state);

    for (i=0; i < 4; ++i) {
        if (0 == (curr_state & pin_mask[i])) {
            break;
        }
    }

    switch (i) {
        case 0: presskey(0x80); break;
        case 1: presskey(0x2C); break;
        case 2: presskey(0x50); break;
        case 3: presskey(0x81); break;
        default: releasekey(); break;
    }

    //LOG_INFO(RC, "i=%d", i);
}

TASK(STACK_SIZE);
