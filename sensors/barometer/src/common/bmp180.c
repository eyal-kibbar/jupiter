#include "barometer.h"
#include "ganymede.h"
#include "logging.h"
#include "hwio.h"
#include "bmp180_regs.h"

typedef struct reg_set_s {
    uint8_t addr;
    uint8_t val;
} reg_set_t;

reg_set_t bmp180_init_cfg[] = {
    
    
};

void barometer_init()
{
    uint8_t chip_id;
    chip_id = regread8(BMP_ADDR, BMP_CHIP_ID_ADDR);
    
    LOG_INFO(BMP, "chip_id: %x", chip_id);
}


void barometer_get(barometer_t* out);


