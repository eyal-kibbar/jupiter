#include "io_i2c_platform.h"
#include <avr/power.h>
#include <stdio.h>
#include <avr/io.h>

void io_i2c_master_platform_init()
{
    // enabling power to TWI
    power_twi_enable();

    // setting 400KHz
    TWSR = 0;
    TWBR = 12;
}

void io_i2c_slave_platform_init(uint8_t slave_addr)
{
    power_twi_enable();
    TWAR = (0 == slave_addr) ? 0x1 : slave_addr << 1;
    TWAMR = 0;
    //TWCR = _BV(TWEN);
}
