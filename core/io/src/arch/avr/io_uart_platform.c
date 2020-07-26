#include "io.h"
#include "io_uart_platform.h"

void io_uart_platform_init(uint16_t baud_rate)
{
    uint16_t baud_encoded =  (uint16_t)(((F_CPU / 16UL) / baud_rate) - 1);

    UBRR0H = (uint8_t)(baud_encoded >> 0x8);
    UBRR0L = (uint8_t)(baud_encoded & 0xFF);

    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}
