#include "ganymede.h"
#include "logging.h"


static int len;
static char buff[50];

void setup()
{
	len = snprintf(buff, sizeof buff, "got: ' '\n\r");
}

void loop()
{
	
	
	gmd_io_tx_t tx[2] = {
		{.isw=0, .off=0, .buf=&buff[6], .len=1 },
		{.isw=1, .off=0, .buf=buff, .len=10 }
	};
	
	gmd_uart_sg(tx, 2, 0);
}





TASK(STACK_SIZE);
