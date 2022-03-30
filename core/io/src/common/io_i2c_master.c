#include "io.h"
#include "io_i2c_platform.h"
#include "ganymede.h"
#include "logging.h"

typedef struct io_i2c_s {
    io_tx_t *tx;
    uint8_t num;
    uint8_t slave_addr;
    uint8_t is_done;
    uint8_t is_used;
} io_i2c_t;

static volatile io_i2c_t io_i2c;

static void io_i2c_next_tx()
{
    ++io_i2c.tx;
    --io_i2c.num;

    if (0 != io_i2c.num) {
        twi_send_start();
    }
    else {
        twi_send_stop();
        io_i2c.is_done = 1;
    }
}

static void io_i2c_isr()
{
    uint8_t status;

    twi_get_status(&status);

    switch (status) {
        case TW_START:
        case TW_REP_START:
            if (io_i2c.tx->mode & IO_TX_MODE_W) {
                twi_send_write(io_i2c.slave_addr);
            }
            else {
                twi_send_read(io_i2c.slave_addr);
            }
            break;

        case TW_MT_SLA_NACK:
            twi_send_start();
            break;

        case TW_MT_DATA_ACK:
            ++io_i2c.tx->off;
            // fall through

        case TW_MT_SLA_ACK:
            // assert tx->mode & IO_TX_MODE_W

            // check if this w transaction has ended
            if (io_i2c.tx->off == io_i2c.tx->len) {
                io_i2c_next_tx();
            }
            else { // keep on writing
                twi_set_data(io_i2c.tx->buf[io_i2c.tx->off]);
                twi_send();
            }
            break;


        case TW_MR_DATA_ACK:
            twi_get_data(&io_i2c.tx->buf[io_i2c.tx->off]);
            ++io_i2c.tx->off;
            // fall through

        case TW_MR_SLA_ACK:
            // assert tx->isw == 0
            if (io_i2c.tx->off < (io_i2c.tx->len-1)) {
                twi_send_ack();
            }
            else {
                twi_send(); // last byte
            }
            break;

        case TW_MR_DATA_NACK:
            twi_get_data(&io_i2c.tx->buf[io_i2c.tx->off]);
            ++io_i2c.tx->off;
            io_i2c_next_tx();
            break;

        default:
            gmd_panic();
            break;
    }
}

IO_I2C_ISR()
{
    io_i2c_isr();
}


void io_i2c_master_init()
{
    io_i2c_master_platform_init();
}

void io_i2c_tx_begin(uint8_t slave_addr)
{
    gmd_wfe(&io_i2c.is_used, 0xFF, 1, 0);

    io_i2c.is_used = 1;
    io_i2c.slave_addr = slave_addr;
}

void io_i2c_tx_end()
{
    io_i2c.is_used = 0;
}


void io_i2c_master_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms)
{
    io_i2c.tx = tx;
    io_i2c.num = n;
    io_i2c.is_done = 0;

    twi_send_start();

    gmd_wfe_io(&io_i2c.is_done, 0xFF, 0, timeout_ms);
}
