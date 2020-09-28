#ifndef IO_UART_PLATFORM_H_
#define IO_UART_PLATFORM_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PASTER(x,y,z) x ## y ## z
#define EVALUATOR(x,y,z)  PASTER(x,y,z)
#define UART_CORE_REG(name, suffix) EVALUATOR(name, UART_CORE_INDEX, suffix)

#define uart_irq_enable_tx() do { (UART_CORE_REG(UCSR,B)) |= _BV(UART_CORE_REG(UDRIE,)); } while (0)
#define uart_irq_enable_rx() do { (UART_CORE_REG(UCSR,B)) |= _BV(UART_CORE_REG(RXCIE,)); } while (0)

#define uart_irq_disable_tx() do { UART_CORE_REG(UCSR,B) &= ~_BV(UART_CORE_REG(UDRIE,)); } while (0)
#define uart_irq_disable_rx() do { UART_CORE_REG(UCSR,B) &= ~_BV(UART_CORE_REG(RXCIE,)); } while (0)

#define uart_irq_disable()    do { UART_CORE_REG(UCSR,B) &= ~(_BV(UART_CORE_REG(UDRIE,)) | _BV(UART_CORE_REG(RXCIE,))); } while (0)
#define uart_irq_disable_tx_enable_rx() do { UART_CORE_REG(UCSR,B) = UART_CORE_REG(UCSR,B) & ~_BV(UART_CORE_REG(UDRIE,)) | _BV(UART_CORE_REG(RXCIE,)); } while (0)
#define uart_irq_enable_tx_disable_rx() do { UART_CORE_REG(UCSR,B) = UART_CORE_REG(UCSR,B) & ~_BV(UART_CORE_REG(RXCIE,)) | _BV(UART_CORE_REG(UDRIE,)); } while (0)

#define uart_set_data(data) do { (UART_CORE_REG(UDR,)) = (uint8_t)data; } while (0)
#define uart_get_data() ((uint8_t)(UART_CORE_REG(UDR,)))

#define uart_is_ready_rx() (!!((UART_CORE_REG(UCSR,A)) & _BV(UART_CORE_REG(RXC,))))
#define uart_is_ready_tx() (!!((UART_CORE_REG(UCSR,A)) & _BV(UART_CORE_REG(UDRE,))))


#ifndef USART0_UDRE_vect
#define USART0_UDRE_vect USART_UDRE_vect
#endif

#ifndef USART0_RX_vect
#define USART0_RX_vect USART_RX_vect
#endif

#define IO_UART_TX_ISR() ISR(EVALUATOR(USART, UART_CORE_INDEX, _UDRE_vect))
#define IO_UART_RX_ISR() ISR(EVALUATOR(USART, UART_CORE_INDEX, _RX_vect))


void io_uart_platform_init(uint16_t baud_rate);

#endif
