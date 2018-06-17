#ifndef GMD_UART_PLATFORM_H_
#define GMD_UART_PLATFORM_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define uart_irq_enable_tx() do { UCSR0B |= _BV(UDRIE0); } while (0)
#define uart_irq_enable_rx() do { UCSR0B |= _BV(RXCIE0); } while (0)

#define uart_irq_disable_tx() do { UCSR0B &= ~_BV(UDRIE0); } while (0)
#define uart_irq_disable_rx() do { UCSR0B &= ~_BV(RXCIE0); } while (0)

#define uart_set_data(data) do { UDR0 = (uint8_t)data; } while (0)
#define uart_get_data() ((uint8_t)UDR0)

#define uart_is_ready_rx() (!!(UCSR0A & _BV(RXC0)))
#define uart_is_ready_tx() (!!(UCSR0A & _BV(UDRE0)))

#define GMD_UART_TX_ISR() ISR(USART_UDRE_vect)
#define GMD_UART_RX_ISR() ISR(USART_RX_vect)


#endif /* GMD_UART_PLATFORM_H_ */
