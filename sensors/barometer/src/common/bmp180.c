#include "barometer.h"
#include "ganymede.h"
#include "logging.h"
#include "hwio.h"
#include "bmp180_regs.h"
#include "utils.h"
#if !defined(BMP180_OSS) || BMP180_OSS<0 || BMP180_OSS>3
    #error "set bmp180 mode: BMP180_OSS [0-3]"
#endif

typedef struct reg_set_s {
    uint8_t addr;
    uint8_t val;
} reg_set_t;

struct bmp180_calib_s {
    int16_t  AC1;
    int16_t  AC2;
    int16_t  AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t  B1;
    int16_t  B2;
    int16_t  MB;
    int16_t  MC;
    int16_t  MD;
};


static struct bmp180_calib_s bmp180_calib;

void barometer_init()
{
    uint8_t chip_id;
    uint16_t* calib;
    uint8_t i;
    LOG_INFO(BMP, "init");

    // reset
    LOG_INFO(BMP, "reset");
    regwrite8(BMP_ADDR, BMP_SOFT_RESET_ADDR, BMP_SOFT_RESET);

    gmd_delay(10);

    // calidate chip is up and working
    chip_id = regread8(BMP_ADDR, BMP_CHIP_ID_ADDR);

    if (BMP_CHIP_ID != chip_id) {
        LOG_ERROR(BMP, "invalid chip id: %x", chip_id);
        gmd_panic();
    }

    LOG_INFO(BMP, "chip_id: %x", chip_id);


    // read calibration data
    regread(BMP_ADDR, BMP_CALIB_ADDR, &bmp180_calib, sizeof(bmp180_calib));
    // fix endian
    for (calib = (uint16_t*)&bmp180_calib, i=0; i < sizeof(bmp180_calib) / sizeof(*calib); ++i) {
        calib[i] = ntohs(calib[i]);
    }

    LOG_INFO(BMP, "AC1: %d", bmp180_calib.AC1);
    LOG_INFO(BMP, "AC2: %d", bmp180_calib.AC2);
    LOG_INFO(BMP, "AC3: %d", bmp180_calib.AC3);
    LOG_INFO(BMP, "AC4: %u", bmp180_calib.AC4);
    LOG_INFO(BMP, "AC5: %u", bmp180_calib.AC5);
    LOG_INFO(BMP, "AC6: %u", bmp180_calib.AC6);

    LOG_INFO(BMP, "B1:  %d", bmp180_calib.B1);
    LOG_INFO(BMP, "B2:  %d", bmp180_calib.B2);
    LOG_INFO(BMP, "MB:  %d", bmp180_calib.MB);
    LOG_INFO(BMP, "MC:  %d", bmp180_calib.MC);
    LOG_INFO(BMP, "MD:  %d", bmp180_calib.MD);

    LOG_INFO(BMP, "initialized");
}

void barometer_pressure_start()
{
    regwrite8(BMP_ADDR, BMP_CTRL_MEAS_ADDR,
        (0x14       << BMP_CTRL_MEAS_MEASUREMENT_CONTROL_SHFT) |
        (0x1        << BMP_CTRL_MEAS_SCO_SHFT) |
        (BMP180_OSS << BMP_CTRL_MEAS_OSS_SHFT));
}

void barometer_temperature_start()
{
    uint8_t res;

    regwrite8(BMP_ADDR, 0xF4,
        (0x1E       << BMP_CTRL_MEAS_MEASUREMENT_CONTROL_SHFT) |
        (0x1        << BMP_CTRL_MEAS_SCO_SHFT));

    //gmd_delay(10);
    res = regread8(BMP_ADDR, BMP_CTRL_MEAS_ADDR);
    LOG_INFO(BMP, "meas: %02x", res);
}

void barometer_temperature_get(float* out)
{
    uint16_t UT;

    while (!barometer_is_data_ready()) {
        LOG_INFO(BMP, "waiting for data");
        gmd_delay(10);
    }

    UT = regread16(BMP_ADDR, BMP_OUT_ADDR);
    UT = ntohs(UT);
    //UP >>= 8-BMP180_OSS;

    LOG_INFO(BMP, "UT: %04x", UT);
}


uint8_t barometer_is_data_ready()
{
    return !(regread8(BMP_ADDR, BMP_CTRL_MEAS_ADDR) & BMP_CTRL_MEAS_SCO_BMSK);
}

void barometer_pressure_get(float* out)
{
    uint8_t raw_pressure[3];
    int32_t UP;

    while (!barometer_is_data_ready()) {
        LOG_INFO(BMP, "waiting for data");
        gmd_delay(10);
    }

    regread(BMP_ADDR, BMP_OUT_ADDR, raw_pressure, 3);
    UP = raw_pressure[0];
    UP <<= 16;
    UP |= raw_pressure[1];
    UP <<= 8;
    UP |= raw_pressure[2];
    //UP >>= 8-BMP180_OSS;

    LOG_INFO(BMP, "UP: %d %d %d", raw_pressure[0], raw_pressure[1], raw_pressure[2]);
}
