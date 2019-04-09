#include "hwio.h"
#include "ganymede.h"
#include "utils.h"
#include "logging.h"

void regwrite(uint8_t slave_addr, uint8_t addr, void* buf, uint8_t n)
{
    gmd_io_tx_t tx[] = {
        { .isw = 1, .buf = &addr, .len = 1, .off = 0 },
        { .isw = 1, .buf = buf,   .len = n, .off = 0 }
    };

    gmd_i2c_sg(slave_addr, tx, ARR_SIZE(tx), 0);
}

void regread(uint8_t slave_addr, uint8_t addr, void* buf, uint8_t n)
{
    gmd_io_tx_t tx[] = {
        { .isw = 1, .buf = &addr, .len = 1, .off = 0 },
        { .isw = 0, .buf = buf,   .len = n, .off = 0 }
    };
    
    gmd_i2c_sg(slave_addr, tx, ARR_SIZE(tx), 0);
}


void regwrite8(uint8_t slave_addr, uint8_t addr, uint8_t val)
{
    regwrite(slave_addr, addr, &val, sizeof(val));
}

void regwrite16(uint8_t slave_addr, uint8_t addr, uint16_t val)
{
    regwrite(slave_addr, addr, &val, sizeof(val));
}

uint8_t regread8(uint8_t slave_addr, uint8_t addr)
{
    uint8_t val;
    regread(slave_addr, addr, &val, sizeof(val));
    return val;
}

uint16_t regread16(uint8_t slave_addr, uint8_t addr)
{
    uint16_t val;
    regread(slave_addr, addr, &val, sizeof(val));
    return val;
}


