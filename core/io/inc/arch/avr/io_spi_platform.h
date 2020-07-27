#ifndef IO_SPI_PLATFORM_H_
#define IO_SPI_PLATFORM_H_

#include <avr/io.h>
#include <avr/interrupt.h>


#define PORT_SPI    PORTB
#define PIN_SPI     PINB
#define DDR_SPI     DDRB
#define DD_MISO     DDB4
#define DD_MOSI     DDB3
#define DD_SS       DDB2
#define DD_SCK      DDB5


#define spi_set_data(data) do { SPDR = (uint8_t)data; } while (0)
#define spi_get_data() ((uint8_t)SPDR)

void io_spi_master_platform_init();
void io_spi_slave_platform_init();


#define IO_SPI_ISR() ISR(SPI_STC_vect)

#endif
