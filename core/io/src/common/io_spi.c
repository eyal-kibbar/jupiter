#include "io_spi_platform.h"
#include "io_platform.h"
#include "io.h"
#include "ganymede.h"
#include "logging.h"

typedef struct spi_sg_s {
    io_tx_t* tx;
    uint8_t num;
    uint8_t slave_select_pin;
    volatile uint8_t is_used;
    volatile uint8_t is_done;
} spi_sg_t;

spi_sg_t spi_sg;

void io_spi_master_init()
{
    io_spi_master_platform_init();
}

void io_spi_slave_init()
{
    io_spi_slave_platform_init();
}

static void do_spi_isr()
{
    uint8_t data;

    if (spi_sg.is_done) {
        return;
    }

    data = spi_get_data();


    if (spi_sg.tx->mode & IO_TX_MODE_R) {
        spi_sg.tx->buf[spi_sg.tx->off] = data;
    }

    ++spi_sg.tx->off;

    if (spi_sg.tx->len == spi_sg.tx->off) {

        // check to see if we are in the last transaction
        if (1 == spi_sg.num) {
            spi_sg.is_done = 1;
            return;
        }

        // move to next transaction
        ++spi_sg.tx;
        --spi_sg.num;
    }

    if (spi_sg.tx->mode & IO_TX_MODE_W) {
        data = spi_sg.tx->buf[spi_sg.tx->off];
    }
    else {
        data = 0;
    }

    spi_set_data(data);

}

IO_SPI_ISR()
{
    do_spi_isr();
}

void io_spi_tx_begin(uint8_t slave_select_pin)
{
    // select slave
    platform_cli();

    // TODO: handle is_used

    spi_sg.slave_select_pin = slave_select_pin;
    spi_sg.is_used = 1;
    io_pin_clr(slave_select_pin);
}
void io_spi_tx_end()
{
    spi_sg.is_used = 0;
    io_pin_set(spi_sg.slave_select_pin);
    platform_sei();
}


static void io_spi_tx_init(io_tx_t *tx, uint8_t n)
{
    for (; 0 < n; --n, ++tx) {
        if (tx->mode & IO_TX_MODE_INLINE) {
            tx->buf = tx->data;
        }
    }
}

void io_spi_master_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms)
{
    uint8_t data = 0;


    if (0 == n) {
        return;
    }

    io_spi_tx_init(tx, n);

    // initialize transactions struct
    spi_sg.tx = tx;
    spi_sg.num = n;
    spi_sg.is_done = 0;


    // do the first write
    if (spi_sg.tx->mode & IO_TX_MODE_W) {
        data = spi_sg.tx->buf[spi_sg.tx->off];
    }

    spi_set_data(data);

    // wait until all transactions are complete
    gmd_wfe(&spi_sg.is_done, 0xFF, 0, timeout_ms);
}

void io_spi_slave_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms)
{

    uint16_t remaining_timeout_ms = timeout_ms;
    uint16_t slept_ms;
    uint8_t data = 0;

    if (0 == n) {
        return;
    }

    io_spi_tx_init(tx, n);

    platform_cli();

    // initialize transactions struct
    spi_sg.tx = tx;
    spi_sg.num = n;
    spi_sg.is_done = 0;
    spi_sg.is_used = 1;

    // do the first write
    if (spi_sg.tx->mode & IO_TX_MODE_W) {
        data = spi_sg.tx->buf[spi_sg.tx->off];
    }

    spi_set_data(data);

    // wait for select slave to be low
    slept_ms = gmd_wfe(&PIN_SPI, _BV(DD_SS), _BV(DD_SS), remaining_timeout_ms);
    if (0 != timeout_ms) {
        if (remaining_timeout_ms <= slept_ms) {
            return;
        }

        remaining_timeout_ms -= slept_ms;
    }

    // wait until master resets the connection by setting SS to high
    gmd_wfe(&PIN_SPI, _BV(DD_SS), 0, remaining_timeout_ms);

    spi_sg.is_used = 0;

    platform_sei();

}
