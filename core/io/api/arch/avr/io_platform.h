#ifndef IO_PLATFORM_H_
#define IO_PLATFORM_H_

#include <stdint.h>

#define platform_cli() cli()
#define platform_sei() sei()


void io_pin_output(uint8_t pin);
void io_pin_input(uint8_t pin);

void io_pin_set(uint8_t pin);
void io_pin_clr(uint8_t pin);

#endif
