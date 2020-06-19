#include "ganymede.h"
#include "gmd_platform.h"
#include "gmd_uart.h"
#include "gmd_uart_platform.h"



typedef struct gmd_uart_sg_s {
    gmd_io_tx_t* tx;
    uint8_t num;
    volatile uint8_t available;
    volatile uint8_t is_done;
} gmd_uart_sg_t;


static gmd_uart_sg_t gmd_uart_tx;
static gmd_uart_sg_t gmd_uart_rx;

void gmd_uart_init()
{
    gmd_uart_platform_init();
    gmd_uart_tx.available = 1;
    gmd_uart_rx.available = 1;
}



static void gmd_uart_tx_next_tx()
{
    ++gmd_uart_tx.tx;
    --gmd_uart_tx.num;

    if (0 == gmd_uart_tx.num) {
        // flag that the operation was finished
        gmd_uart_tx.is_done = 1;
        return;
    }

    if (gmd_uart_tx.tx->isw) {
        uart_irq_enable_tx();
    }
    else {
        gmd_uart_tx.is_done = 1; // need to switch transactions
    }
}

static void gmd_uart_rx_next_tx()
{
    ++gmd_uart_rx.tx;
    --gmd_uart_rx.num;

    if (0 == gmd_uart_rx.num) {
        // flag that the operation was finished
        gmd_uart_rx.is_done = 1;
        return;
    }

    if (gmd_uart_rx.tx->isw) {
        gmd_uart_rx.is_done = 1; // need to switch transactions
    }
    else {
        uart_irq_enable_rx();
    }
}

GMD_UART_RX_ISR()
{
    uint8_t data;

    data = uart_get_data();
    gmd_uart_rx.tx->buf[gmd_uart_rx.tx->off++] = data;

    if (gmd_uart_rx.tx->off == gmd_uart_rx.tx->len) {
        uart_irq_disable_rx();
        gmd_uart_rx_next_tx();
    }
}

GMD_UART_TX_ISR()
{
    uint8_t data;

    data = gmd_uart_tx.tx->buf[gmd_uart_tx.tx->off++];
    uart_set_data(data);

    if (gmd_uart_tx.tx->off == gmd_uart_tx.tx->len) {
        uart_irq_disable_tx();
        gmd_uart_tx_next_tx();
    }
}

static void gmd_uart_sg_noisr(gmd_io_tx_t* tx, uint8_t n)
{
    uint8_t* buf;
    size_t sz;

    for (; 0 != n; ++tx, --n) {
        for (; 0 != tx->len; ++tx->buf, --tx->len) {
            if (tx->isw) {
                while (!uart_is_ready_tx());
                uart_set_data(*tx->buf);
            }
            else {
                while (!uart_is_ready_rx());
                *tx->buf = uart_get_data();
            }
        }
    }
}

static uint16_t gmd_uart_wait(gmd_uart_sg_t* uart, uint16_t timeout_ms)
{
    uint16_t sleep_ms = 0;

    while (!(uart->available)) {
        sleep_ms = gmd_wfe(&uart->available, 0xFF, 0, timeout_ms);

        // check for timeout
        if (0 != timeout_ms) {
            if (timeout_ms <= sleep_ms) {
                return 0;
            }

            timeout_ms -= sleep_ms;
        }
    }

    return timeout_ms;
}

void gmd_uart_sg(gmd_io_tx_t* tx, uint8_t n, uint16_t timeout_ms)
{
    uint16_t remaining_timeout_ms;

    // skip empty transactions
    for (;tx->len == tx->off && n > 0; ++tx, --n);

    // allow priting in non-interrupt env
    // this is the case during boot and inside other isr
    if (!platform_isei()) {
        gmd_uart_sg_noisr(tx, n);
        return;
    }

    // wait until uart is available
    platform_cli();

    while (n > 0) {

        gmd_uart_sg_t* uart = tx->isw ? &gmd_uart_tx : &gmd_uart_rx;

        remaining_timeout_ms = gmd_uart_wait(uart, timeout_ms);
        if (timeout_ms != 0 && remaining_timeout_ms == 0) {
            goto end;
        }
        timeout_ms = remaining_timeout_ms;

        // mark that we are using uart
        uart->available = 0;
        uart->is_done = 0;

        // set the data for the isr
        uart->tx = tx;
        uart->num = n;

        if (tx->isw) {
            uart_irq_enable_tx();
        }
        else {
            uart_irq_enable_rx();
        }

        // wait until operation is done
        remaining_timeout_ms = gmd_wfe(&uart->is_done, 0xFF, 0, timeout_ms);
        uart->available = 1;
        if (tx->isw) {
            uart_irq_disable_tx();
        }
        else {
            uart_irq_disable_rx();
        }
        if (timeout_ms != 0 && remaining_timeout_ms == 0) {
            goto end;
        }
        timeout_ms = remaining_timeout_ms;

        // update parameters in case we need to continue
        tx = uart->tx;
        n = uart->num;
    }

end:
    platform_sei();
}
