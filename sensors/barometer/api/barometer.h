#ifndef BAROMETER_H_
#define BAROMETER_H_

#include <stdint.h>


void barometer_init();

uint8_t barometer_is_data_ready();


void barometer_pressure_start();
void barometer_temperature_start();

void barometer_pressure_get(float* out);
void barometer_temperature_get(float* out);


#endif /* BAROMETER_H_ */
