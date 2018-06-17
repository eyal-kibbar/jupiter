#include "ganymede.h"
#include "gmd_platform.h"
#include "gmd_uart.h"
#include "gmd_uart_platform.h"



typedef struct gmd_uart_sg_s {
    gmd_io_tx_t* tx;
    uint8_t num;
} gmd_uart_sg_t;

static volatile uint8_t uart_available;
static volatile uint8_t uart_is_done;
static gmd_uart_sg_t gmd_uart;

void gmd_uart_init()
{
    gmd_uart_platform_init();
    uart_available = 1;
}

static void gmd_uart_next_tx()
{
    ++gmd_uart.tx;
    --gmd_uart.num;
    
    if (0 == gmd_uart.num) {
        // flag that the operation was finished
        uart_is_done = 1;
        return;
    }
    
    if (gmd_uart.tx->isw) {
        uart_irq_enable_tx();
    }
    else {
        uart_irq_enable_rx();
    }
}

GMD_UART_RX_ISR()
{
    uint8_t data;
    
    data = uart_get_data();
    gmd_uart.tx->buf[gmd_uart.tx->off++] = data;
    
    if (gmd_uart.tx->off == gmd_uart.tx->len) {
        uart_irq_disable_rx();
        gmd_uart_next_tx();
    }
}

GMD_UART_TX_ISR()
{
    uint8_t data;
    
    data = gmd_uart.tx->buf[gmd_uart.tx->off++];
    uart_set_data(data);

    if (gmd_uart.tx->off == gmd_uart.tx->len) {
        uart_irq_disable_tx();
        gmd_uart_next_tx();
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

void gmd_uart_sg(gmd_io_tx_t* tx, uint8_t n, uint16_t timeout_ms)
{
    uint16_t sleep_ms;

    // skip empty transactions
    for (;tx->len == tx->off; ++tx, --n) {
        if (0 == n) {
            return;
        }
    }

    // allow priting in non-interrupt env
    // this is the case during boot and inside other isr
    if (!platform_isei()) {
        gmd_uart_sg_noisr(tx, n);
        return;
    }
    
    // wait until uart is available
    platform_cli();
    
    while (!uart_available) {
        sleep_ms = gmd_wfe(&uart_available, timeout_ms);
        
        // check for timeout
        if (0 != timeout_ms) {
            if (timeout_ms <= sleep_ms) {
                return;
            }
            
            timeout_ms -= sleep_ms;
        }
    }
    
    // mark that we are using uart
    uart_available = 0;
    uart_is_done = 0;
    
    // set the data for the isr
    gmd_uart.tx = tx;
    gmd_uart.num = n;
    
    // enable uart interrupt
    if (tx->isw) {
        uart_irq_enable_tx();
    }
    else {
        uart_irq_enable_rx();
    }
    
    // wait until operation is done
    gmd_wfe(&uart_is_done, timeout_ms);
    uart_available = 1;
    uart_irq_disable_tx();
    uart_irq_disable_rx();
    
    platform_sei();
}























