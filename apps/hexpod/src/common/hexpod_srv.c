#include "hexpod.h"
#include "ganymede.h"
#include "logging.h"
#include "srv.h"

#include <avr/interrupt.h>




void setup()
{
    
}

void loop()
{
    int x;
	hexpod_state_t script[HEXPOD_MAX_INSTRUCTIONS];
    x = srv_recv(script, sizeof script);
    LOG_INFO(HEXPOD_SRV, "recved %d bytes");
    if (x > 0) {
        hexpod_load(script, x / sizeof(hexpod_state_t));
    }
}

TASK(STACK_SIZE);
