#include "io_spi_platform.h"




void io_spi_master_platform_init()
{
    DDR_SPI = _BV(DD_MOSI) | _BV(DD_SCK) | _BV(DD_SS);
    SPCR = _BV(SPIE) | _BV(SPE) | _BV(MSTR)
            //| _BV(SPR1) | _BV(SPR0) // clock rate = 16 MHz / 128
            ; // clock rate = 16 MHz / 4
}

void io_spi_slave_platform_init()
{
    DDR_SPI |= _BV(DD_MISO);
    SPCR = _BV(SPIE) | _BV(SPE);
}
