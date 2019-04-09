#include "ganymede.h"
#include "logging.h"
#include "barometer.h"

void setup()
{
	LOG_INFO(BAROMETER_TEST, "init");
	barometer_init();
}

void loop()
{
	gmd_delay(1000);
}



TASK(STACK_SIZE);
