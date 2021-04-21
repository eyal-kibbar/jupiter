#include "ganymede.h"
#include "logging.h"
#include "barometer.h"
#include "io.h"
#include "logging.h"

void setup()
{
	io_uart_init(0, 57600);
    io_logging_init();
    io_i2c_master_init();
}

void init()
{
	LOG_INFO(BAROMETER_TEST, "init");
	barometer_init();
}
void loop()
{
	float p;
	barometer_temperature_start();
	barometer_temperature_get(&p);

	gmd_delay(1000);
}



TASK(STACK_SIZE);
