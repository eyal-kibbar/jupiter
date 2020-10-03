#ifndef IO_PLATFORM_H_
#define IO_PLATFORM_H_

#include <stdint.h>

#define platform_cli() cli()
#define platform_sei() sei()


enum io_analog_ref_e {
    IO_ANALOG_REF_AREF,
    IO_ANALOG_REF_AVcc,
    IO_ANALOG_REF_INTERNAL,
};

/** GPIO **/
/******************************************************************************/
void io_pin_port(uint8_t pin, uint8_t** port, uint8_t* mask);

void io_pin_output(uint8_t pin);
void io_pin_input(uint8_t pin);

void io_pin_set(uint8_t pin);
uint8_t io_pin_get(uint8_t pin);
void io_pin_clr(uint8_t pin);

void io_analog_init();
void io_analog_read(uint8_t pin, enum io_analog_ref_e ref, uint16_t* val);

#endif
