#include "hexpod.h"
#include "ganymede.h"
#include "logging.h"
#include "srv.h"

#include <avr/interrupt.h>

hexpod_state_t s[5];


void setup()
{
    
}

void loop()
{
    int x;
    x = srv_recv(s, sizeof s);
    LOG_INFO(HEXPOD_SRV, "recved %d bytes");
    if (x > 0) {
        hexpod_load(s, x / sizeof(hexpod_state_t));
    }
}

TASK(STACK_SIZE);
