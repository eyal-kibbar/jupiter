#include "io.h"
#include "io_i2c_platform.h"
#include "ganymede.h"
#include "logging.h"

#define I2C_SLAVE_STATE_ONGOING 0
#define I2C_SLAVE_STATE_DONE 1
#define I2C_SLAVE_STATE_ERR 2

typedef struct io_i2c_s {
    io_tx_t *tx;
    uint8_t num;
    uint8_t state;
} io_i2c_t;

io_i2c_t i2c;

static volatile io_i2c_t* p_i2c;

static void io_i2c_next_tx()
{
    ++p_i2c->tx;
    --p_i2c->num;

    if (0 == p_i2c->num) {
        p_i2c->state = I2C_SLAVE_STATE_DONE;
    }
}


#include "io_uart_platform.h"
char p[5];
static void io_i2c_isr()
{

    uint8_t data;
    uint8_t status;

    io_pin_set(10);

    if (NULL == p_i2c || p_i2c->state) {
        return;
    }


    status = TWSR;
    p_i2c->state = 1;


    {


        sprintf(p, "%02x", status);

        while (!uart_is_ready_tx());
        uart_set_data(p[0]);
        while (!uart_is_ready_tx());
        uart_set_data(p[1]);
    }

    //twi_get_status(&status);
    return;

    switch (status) {

        case TW_SR_GCALL_ACK:
        case TW_SR_SLA_ACK:

            // write command has been received:
            // make sure we are ready for an incoming write tx
            // if not, send a nack
            twi_send_ack();
            break;

        case TW_SR_ARB_LOST_GCALL_ACK:
        case TW_SR_ARB_LOST_SLA_ACK:
            twi_send_nack();
            break;

        case TW_SR_GCALL_DATA_ACK:
        case TW_SR_DATA_ACK:

            // make sure we are ready for an incoming write tx
            twi_get_data(&data);
            //p_i2c->tx->buf[p_i2c->tx->off] = data;
            ++p_i2c->tx->off;
            if (p_i2c->tx->off == p_i2c->tx->len) {
                io_i2c_next_tx();
            }
            break;

        case TW_SR_GCALL_DATA_NACK:
        case TW_SR_DATA_NACK:
            twi_send_ack();
            p_i2c->state = I2C_SLAVE_STATE_ERR;
            break;

        case TW_SR_STOP:
            twi_send_ack();
            p_i2c->state = I2C_SLAVE_STATE_ERR;

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


void io_i2c_slave_init(uint8_t slave_addr)
{
    io_i2c_slave_platform_init(slave_addr);
    io_pin_output(10);
}

void io_i2c_slave_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms)
{
    //io_i2c_t i2c;
    //io_pin_set(10);
    i2c.tx = tx;
    i2c.num = n;
    i2c.state = I2C_SLAVE_STATE_ONGOING;

    //platform_cli();
    p_i2c = &i2c;
    //twi_enable_auto_ack();

    LOG_INFO(I2C, "waiting");
    while (1) {
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);

        while ((TWCR & _BV(TWINT)) == 0) {
            //LOG_INFO(I2C, "status: %02x %02x", TWSR, TWCR);
        }
        LOG_INFO(I2C, "status1: %02x", TWSR);
        if (TWSR == 0) {
            TWCR = _BV(TWSTO) | _BV(TWINT) | _BV(TWEN);
        }
        /* code */
    }

    //io_pin_clr(10);
    gmd_wfe(&i2c.state, 0xFF, I2C_SLAVE_STATE_ONGOING, 0);
    //gmd_delay(3000);
    //io_pin_clr(10);

    twi_disable_auto_ack();
    p_i2c = NULL;
    platform_sei();

    //LOG_INFO(I2C_SLV, "status: %02x", status);
}
