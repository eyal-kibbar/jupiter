#include "io.h"



extern void io_uart_init_core(uint16_t baud_rate);
extern void io_uart_sg_core(io_tx_t* tx, uint8_t n, uint16_t timeout_ms);


void io_uart_init(uint8_t core_idx, uint16_t baud_rate)
{
    io_uart_init_core(baud_rate);
}

void io_uart_sg(uint8_t core_idx, io_tx_t* tx, uint8_t n, uint16_t timeout_ms)
{
    io_uart_sg_core(tx, n, timeout_ms);
}
