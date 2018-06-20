#include "hexpod.h"
#include "ganymede.h"
#include "logging.h"
#include "srv.h"

#include <avr/interrupt.h>

hexpod_state_t s[5];

volatile uint8_t X;
uint8_t b[5];

void setup()
{
    
}

void loop()
{
//#if 0
    int x;
    x = srv_recv(s, sizeof s);
    LOG_INFO(HEXPOD_SRV, "recved %d bytes");
    if (x > 0) {
        hexpod_load(s, x / sizeof(hexpod_state_t));
        gmd_delay(10000);
    }
//#endif
    //gmd_io_tx_t tx = {.isw=0, .len=5, .off=0, .buf=b};
    //gmd_uart_sg(&tx, 1, 0);
    //cli();
    //gmd_wfe(&X, 1000);
    //sei();
    //LOG_INFO(HEXPOD_SRV, "S");
}

TASK(STACK_SIZE);