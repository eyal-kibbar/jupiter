#include "gmd_uart.h"

#include <stdint.h>
#include <avr/io.h>

#ifndef DRV_UART_BAUDRATE
#error "DRV_UART_BAUDRATE must be set"
#endif

#define _BAUDRATE ((uint16_t)((F_CPU / (DRV_UART_BAUDRATE * 16UL)) - 1))


void gmd_uart_platform_init()
{
    UBRR0H = (uint8_t)(_BAUDRATE >> 0x8);
    UBRR0L = (uint8_t)(_BAUDRATE & 0xFF);

    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}
