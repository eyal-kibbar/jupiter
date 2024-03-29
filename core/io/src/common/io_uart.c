#include "io.h"
#include "io_uart_platform.h"
#include "ganymede.h"
#include <stdlib.h>

typedef struct uart_sg_s {
    io_tx_t* tx;
    uint8_t num;
    uint8_t is_done;
} uart_sg_t;

static volatile uart_sg_t* p_uart = NULL;
static volatile uint8_t uart_used = 0;

void io_uart_init_core(uint16_t baud_rate)
{
    io_uart_platform_init(baud_rate);
}


IO_UART_TX_ISR()
{
    p_uart->is_done = 1;
    uart_irq_disable();
}

IO_UART_TX_READY_ISR()
{
    uint8_t data;

    data = p_uart->tx->buf[p_uart->tx->off++];
    uart_set_data(data);

    if (p_uart->tx->off == p_uart->tx->len) {

        // check if this is the last transaction
        if (p_uart->num == 1) {
            uart_irq_disable_tx_enable_txwait();
            return;
        }

        // move to next transaction
        ++p_uart->tx;
        --p_uart->num;

        // check if the next transaction is a read
        if (p_uart->tx->mode & IO_TX_MODE_R) {
            // need to switch interrupt handling
            uart_irq_disable_tx_enable_rx();
        }
    }
}

IO_UART_RX_ISR()
{
    uint8_t data;

    data = uart_get_data();
    p_uart->tx->buf[p_uart->tx->off++] = data;

    if (p_uart->tx->off == p_uart->tx->len) {

        // check if this is the last transaction
        if (p_uart->num == 1) {
            p_uart->is_done = 1;
            uart_irq_disable();
            // TODO: test the following case:
            // duplex comm, when prev tx is transmit and this recv tx ended so
            // fast that the prev tx did not finish flushing its write buffer
            return;
        }

        // move to next transaction
        ++p_uart->tx;
        --p_uart->num;

        // check if the next transaction is a write
        if (p_uart->tx->mode & IO_TX_MODE_W) {
            // need to switch interrupt handling
            uart_irq_enable_tx_disable_rx();
        }
    }
}

void io_uart_sg_core(io_tx_t* tx, uint8_t n, uint16_t timeout_ms)
{
    uart_sg_t sg;
    uint16_t remaining_timeout_ms = timeout_ms;
    uint16_t sleep_ms;

    sg.tx = tx;
    sg.num = n;
    sg.is_done = 0;

    // wait for device to be available
    sleep_ms = gmd_wfe(&uart_used, 0xFF, 1, remaining_timeout_ms);
    if (0 != remaining_timeout_ms) {
        if (remaining_timeout_ms <= sleep_ms) {
            return;
        }
        remaining_timeout_ms -= sleep_ms;
    }

    uart_used = 1;
    p_uart = &sg;

    if (sg.tx->mode & IO_TX_MODE_R) {
        uart_irq_enable_rx();
    }
    else if (sg.tx->mode & IO_TX_MODE_W) {
        uart_irq_enable_tx();
    }

    gmd_wfe_io(&sg.is_done, 0xFF, 0, remaining_timeout_ms);
    uart_irq_disable();
    p_uart = NULL;
    uart_used = 0;
}
