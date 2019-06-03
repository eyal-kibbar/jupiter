#include "ganymede.h"
#include "logging.h"
#include "gmd_i2c.h"
#include "gmd_platform.h"
#include "gmd_i2c_platform.h"

typedef struct gmd_i2c_sg_s {
    gmd_i2c_dev_addr_t slave_addr;
    gmd_io_tx_t* tx;
    uint8_t num;
} gmd_i2c_sg_t;


static volatile uint8_t i2c_available;
static volatile uint8_t i2c_is_done;
static gmd_i2c_sg_t gmd_i2c;


void gmd_i2c_platform_init()
{
#ifndef DRV_I2C_PRESCALER
#define DRV_I2C_PRESCALER 1
#endif

    // set the SCL frequency
#if   DRV_I2C_PRESCALER == 1
    TWSR = 0;
#elif DRV_I2C_PRESCALER == 4
    TWSR = _BV(TWPS0);
#elif DRV_I2C_PRESCALER == 16
    TWSR = _BV(TWPS1);
#elif DRV_I2C_PRESCALER == 64
    TWSR = _BV(TWPS1) | _BV(TWPS0);
#else
    #error "Invalid value of DRV_I2C_PRESCALER"
#endif

#ifndef DRV_I2C_BITRATE
#define DRV_I2C_BITRATE 0
#endif

#if DRV_I2C_BITRATE > 255
    #error "Invalid value of DRV_I2C_BITRATE"
#endif

    //TWBR = DRV_I2C_BITRATE;
    TWBR = 72;
    LOG_INFO(I2C_MASTER, "init i2c");
    i2c_available = 1;
}

static void gmd_i2c_dbg_print_tx()
{
    uint8_t i;
    LOG_ERROR(I2C, "TX: %s", gmd_i2c.tx->isw ? "W" : "R");
    for (i=0; i < gmd_i2c.tx->len; ++i) {
        LOG_ERROR(I2C, " %02x: %02x", i, gmd_i2c.tx->buf[i]);
    }
}


static void gmd_i2c_next_tx()
{
    LOG_INFO(I2C, "TX ended");

    ++gmd_i2c.tx;
    --gmd_i2c.num;

    if (0 != gmd_i2c.num) {
        LOG_INFO(I2C, "starting next TX, %d more to go", gmd_i2c.num);
        twi_send_start();
    }
    else {
        LOG_INFO(I2C, "finished TX batch");
        twi_send_stop();
        i2c_is_done = 1;
    }
}


static void gmd_i2c_isr()
{
    uint8_t status;

    twi_get_status(&status);
    LOG_INFO(I2C, "status: %02x", status);

    switch (status) {
        case TW_START:
        case TW_REP_START:
            if (gmd_i2c.tx->isw) {
                LOG_INFO(I2C, "sending W");
                twi_send_write(gmd_i2c.slave_addr);
            }
            else {
                LOG_INFO(I2C, "sending R");
                twi_send_read(gmd_i2c.slave_addr);
            }
            break;

        case TW_MT_SLA_NACK:
            LOG_INFO(I2C, "resend start");
            twi_send_start();
            break;

        case TW_MT_DATA_ACK:
            ++gmd_i2c.tx->off;
            // fall through

        case TW_MT_SLA_ACK:
            // assert tx->isw == 1

            // check if this w transaction has ended
            if (gmd_i2c.tx->off == gmd_i2c.tx->len) {
                gmd_i2c_next_tx();
            }
            else { // keep on writing
                LOG_INFO(I2C, "writing byte %02x", gmd_i2c.tx->buf[gmd_i2c.tx->off]);
                twi_set_data(gmd_i2c.tx->buf[gmd_i2c.tx->off]);
                twi_send();
            }
            break;


        case TW_MR_DATA_ACK:
            twi_get_data(&gmd_i2c.tx->buf[gmd_i2c.tx->off]);
            LOG_INFO(I2C, "reading byte %02x", gmd_i2c.tx->buf[gmd_i2c.tx->off]);
            ++gmd_i2c.tx->off;
            // fall through

        case TW_MR_SLA_ACK:
            // assert tx->isw == 0
            if (1 == (gmd_i2c.tx->len-gmd_i2c.tx->off)) {
                LOG_INFO(I2C, "last byte");
                twi_send();
            }
            else {
                LOG_INFO(I2C, "%d more bytes to go", gmd_i2c.tx->len);
                twi_send_ack();
            }
            break;

        case TW_MR_DATA_NACK:
            twi_get_data(&gmd_i2c.tx->buf[gmd_i2c.tx->off]);
            LOG_INFO(I2C,  "reading last byte: %02x", gmd_i2c.tx->buf[gmd_i2c.tx->off]);
            ++gmd_i2c.tx->off;
            gmd_i2c_next_tx();
            break;

        default:
            LOG_ERROR(I2C, "unexpected status: %x", status);
            gmd_i2c_dbg_print_tx();
            //gmd_panic();
            break;
    }
}

ISR(TWI_vect)
{
    gmd_i2c_isr();
}

static void gmd_i2c_sg_noisr(gmd_i2c_dev_addr_t slave_addr, gmd_io_tx_t* tx, uint8_t n)
{
    i2c_is_done = 0;

    gmd_i2c.slave_addr = slave_addr;
    gmd_i2c.tx = tx;
    gmd_i2c.num = n;

    LOG_INFO(I2C, "sending start");
    twi_send_start();

    do {
        // wait for i2c to be ready
        while (!twi_is_ready());

        gmd_i2c_isr();

    } while (0 == i2c_is_done);
}

void gmd_i2c_sg(gmd_i2c_dev_addr_t slave_addr, gmd_io_tx_t* tx, uint8_t n, uint16_t timeout_ms)
{
    uint16_t sleep_ms;
     // wait until i2c is available

    if (!platform_isei()) {
        gmd_i2c_sg_noisr(slave_addr, tx, n);
        return;
    }

    platform_cli();

    LOG_INFO(I2C, "waiting for bus: %d", i2c_available);
    while (!i2c_available) {
        sleep_ms = gmd_wfe(&i2c_available, 0xFF, timeout_ms);

        // check for timeout
        if (0 != timeout_ms) {
            if (timeout_ms <= sleep_ms) {
                return;
            }

            timeout_ms -= sleep_ms;
        }
    }
    i2c_available = 0;
    i2c_is_done = 0;
    platform_sei();

    gmd_i2c.slave_addr = slave_addr;
    gmd_i2c.tx = tx;
    gmd_i2c.num = n;

    // enable i2c ready interrupt
    LOG_INFO(I2C, "sending start %02x", TWCR);
    twi_send_start();

    // wait until operation is done
    LOG_INFO(I2C, "waiting for transaction to finish");
    gmd_wfe(&i2c_is_done, 0xFF, timeout_ms);
    i2c_available = 1;
}
