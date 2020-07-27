#include "hwio.h"
#include "ganymede.h"
#include "utils.h"
#include "logging.h"
#include "io.h"

void regwrite(uint8_t slave_addr, uint8_t addr, void* buf, uint8_t n)
{
    io_tx_t tx[] = {
        { .mode = IO_TX_MODE_W, .buf = &addr, .len = 1, .off = 0 },
        { .mode = IO_TX_MODE_W, .buf = buf,   .len = n, .off = 0 }
    };
    io_i2c_tx_begin(slave_addr);
    io_i2c_master_sg(tx, ARR_SIZE(tx), 0);
    io_i2c_tx_end();
}

void regread(uint8_t slave_addr, uint8_t addr, void* buf, uint8_t n)
{
    io_tx_t tx[] = {
        { .mode = IO_TX_MODE_W, .buf = &addr, .len = 1, .off = 0 },
        { .mode = IO_TX_MODE_R, .buf = buf,   .len = n, .off = 0 }
    };
    io_i2c_tx_begin(slave_addr);
    io_i2c_master_sg(tx, ARR_SIZE(tx), 0);
    io_i2c_tx_end();
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
