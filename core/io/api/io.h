#ifndef IO_H_
#define IO_H_

#include "ganymede.h"


#define IO_TX_MODE_W 0x1
#define IO_TX_MODE_R 0x2


typedef struct io_tx_s {
    uint8_t  mode;
    uint8_t  len;
    uint8_t  off;
    uint8_t* buf;
} io_tx_t;

void io_spi_master_init();
void io_spi_slave_init();

void io_spi_master_sg(uint8_t slave_select_pin, io_tx_t *tx, uint8_t n, uint16_t timeout_ms);
void io_spi_slave_sg(io_tx_t *tx, uint8_t n, uint16_t timeout_ms);


#endif /* IO_H_ */
