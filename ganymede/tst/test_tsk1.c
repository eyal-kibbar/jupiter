#include "ganymede.h"
#include "logging.h"


static int i;
static char buff[50];

void setup()
{
	
}

void loop()
{
	++i;
	gmd_io_tx_t xmit = { .isw=1, .off=0, .buf=buff };
	
	xmit.len = snprintf(buff, sizeof buff, "loop: %d\n\r", i);
	
	gmd_uart_sg(&xmit, 1, 0);
	
	gmd_delay(1000);
}





TASK(STACK_SIZE);
