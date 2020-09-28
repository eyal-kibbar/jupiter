#include "io.h"



extern void io_uart_init_core0(uint16_t baud_rate);
extern void io_uart_init_core1(uint16_t baud_rate);

extern void io_uart_sg_core0(io_tx_t* tx, uint8_t n, uint16_t timeout_ms);
extern void io_uart_sg_core1(io_tx_t* tx, uint8_t n, uint16_t timeout_ms);


void io_uart_init(uint8_t core_idx, uint16_t baud_rate)
{
    switch(core_idx) {
        default:
        case 0: io_uart_init_core0(baud_rate); break;
        case 1: io_uart_init_core1(baud_rate); break;
    }
}

void io_uart_sg(uint8_t core_idx, io_tx_t* tx, uint8_t n, uint16_t timeout_ms)
{
    switch(core_idx) {
        default:
        case 0: io_uart_sg_core0(tx, n, timeout_ms); break;
        case 1: io_uart_sg_core1(tx, n, timeout_ms); break;
    }
}
