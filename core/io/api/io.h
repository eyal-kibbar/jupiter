#ifndef IO_H_
#define IO_H_

#include "io_platform.h"


#define IO_TX_MODE_W      0x1
#define IO_TX_MODE_R      0x2

typedef struct io_tx_s {
    uint8_t  mode;
    uint8_t  len;
    uint8_t  off;
    uint8_t* buf;
} io_tx_t;

void io_logging_init();


/** SPI **/
/******************************************************************************/
void io_spi_master_init();
void io_spi_tx_begin(uint8_t slave_select_pin);
void io_spi_tx_end();
void io_spi_master_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms);

void io_spi_slave_init();
void io_spi_slave_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms);

/** UART **/
/******************************************************************************/
void io_uart_init(uint16_t baud_rate);
void io_uart_sg(io_tx_t* tx, uint8_t n, uint16_t timeout_ms);

/** I2C **/
/******************************************************************************/
void io_i2c_master_init();
void io_i2c_tx_begin(uint8_t slave_addr);
void io_i2c_tx_end();
void io_i2c_master_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms);

void io_i2c_slave_init(uint8_t slave_addr);
void io_i2c_slave_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms);

#endif /* IO_H_ */
