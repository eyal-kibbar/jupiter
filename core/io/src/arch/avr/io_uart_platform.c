#include "io.h"
#include "io_uart_platform.h"


//256000
void io_uart_platform_init(uint16_t baud_rate)
{
    uint16_t baud_encoded =  (uint16_t)(((F_CPU / 16UL) / baud_rate) - 1);

#if UART_CORE_INDEX == 0
    PRR &= ~(_BV(PRUSART0));
#endif

    UART_CORE_REG(UBRR,H) = (uint8_t)(baud_encoded >> 0x8);
    UART_CORE_REG(UBRR,L) = (uint8_t)(baud_encoded & 0xFF);
    //UART_CORE_REG(UBRR,H) = 0;
    //UART_CORE_REG(UBRR,L) = 8;
    //UCSR0A = 0x2;
    UART_CORE_REG(UCSR,B) = _BV(UART_CORE_REG(TXEN,)) | _BV(UART_CORE_REG(RXEN,));
}
