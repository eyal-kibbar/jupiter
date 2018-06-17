#ifndef GMD_UART_H_
#define GMD_UART_H_

#ifndef DRV_UART_BAUDRATE
#error "DRV_UART_BAUDRATE must be set"
#endif


void gmd_uart_init();
void gmd_uart_platform_init();


#endif /* GMD_UART_H_ */
