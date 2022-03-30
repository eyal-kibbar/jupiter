#include "io_i2c_platform.h"


void io_i2c_master_platform_init()
{
    // enabling power to TWI
    PRR &= ~(_BV(PRTWI));
    // setting 400KHz
    TWSR = 0;
    TWBR = 12;
}

void io_i2c_slave_platform_init(uint8_t slave_addr)
{
    TWAR = (0 == slave_addr) ? 0x1 : slave_addr << 1;
    TWAMR = 0;
    //TWCR = _BV(TWEN);
}
