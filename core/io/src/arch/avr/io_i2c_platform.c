#include "io_i2c_platform.h"


void io_i2c_master_platform_init()
{
#if   IO_I2C_PRESCALER == 1
    TWSR = 0;
#elif IO_I2C_PRESCALER == 4
    TWSR = _BV(TWPS0);
#elif IO_I2C_PRESCALER == 16
    TWSR = _BV(TWPS1);
#elif IO_I2C_PRESCALER == 64
    TWSR = _BV(TWPS1) | _BV(TWPS0);
#else
    #error "Invalid value of IO_I2C_PRESCALER"
#endif

    TWBR = 72;
}

void io_i2c_slave_platform_init(uint8_t slave_addr)
{
    TWAR = (0 == slave_addr) ? 0x1 : slave_addr << 1;
    TWAMR = 0;
    //TWCR = _BV(TWEN);
}
