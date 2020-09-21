#ifndef IO_I2C_PLATFORM_H_
#define IO_I2C_PLATFORM_H_


#include <compat/twi.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define twi_send_start() \
    do { TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE); } while(0)

#define twi_send_stop() \
    do { TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN) | _BV(TWIE); } while(0)

#define twi_get_status(status) \
    do { (*(status)) = TWSR & ~(_BV(TWPS1) | _BV(TWPS0)); } while (0)


#define twi_get_data(data) do { *((uint8_t*)(data)) = TWDR; } while (0)
#define twi_set_data(data) do { TWDR = (data); } while (0)


#define twi_send() do { TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE); } while (0)

#define twi_send_write(slave_addr) \
    do { twi_set_data(((slave_addr) << 1) | TW_WRITE); twi_send(); } while (0)

#define twi_send_read(slave_addr) \
    do { twi_set_data(((slave_addr) << 1) | TW_READ); twi_send(); } while (0)


#define twi_send_ack() \
    do { TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE); } while (0)

#define twi_send_nack() \
        do { TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE); } while (0)

#define twi_is_ready() (TWCR & _BV(TWINT))


#define twi_enable_auto_ack()  do { TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWIE); } while (0)
#define twi_disable_auto_ack() do { TWCR = _BV(TWEN); } while (0)


#define IO_I2C_ISR() ISR(TWI_vect)


void io_i2c_master_platform_init();
void io_i2c_slave_platform_init(uint8_t slave_addr);

#endif
