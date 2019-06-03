#include "ganymede.h"
#include "logging.h"
#include "gmd_i2c.h"
#include "gmd_platform.h"
#include "gmd_i2c_platform.h"


typedef struct gmd_i2c_slave_s {
    uint8_t (*on_read)(void);
    void (*on_write)(uint8_t);

} gmd_i2c_slave_t;

static gmd_i2c_slave_t gmd_i2c_slave;

void gmd_i2c_platform_init()
{

}

void gmd_i2c_slave_init(gmd_i2c_dev_addr_t slave_addr, uint8_t (*on_read)(void), void (*on_write)(uint8_t))
{
    LOG_INFO(i2c_slave, "init");
    TWAR = slave_addr << 1;
    //TWAMR = 0xFE;
    TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);

    while (1) {
        LOG_INFO(X, "%02x", TWCR);
        gmd_delay(1000);
    }
}


static void gmd_i2c_slave_isr()
{
    LOG_INFO(i2c_slave, "imhere");
}

ISR(TWI_vect)
{
    gmd_i2c_slave_isr();
}
