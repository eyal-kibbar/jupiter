#include "ganymede.h"
#include "logging.h"

#include "mpu6050_regs.h"
#include "mpu.h"

#include <stdio.h>
#include <string.h>
#include <math.h>


#define htons(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
#define ntohs htons

#define MPU_TEMPERATURE_OFFSET (-540)
#define MPU_TEMPERATURE_SCALE (340)

#define ARR_SIZE(x) (sizeof(x) / sizeof(*(x)))

struct mpu_reg_set_s {
    uint8_t addr;
    uint8_t val;
};

static struct mpu_reg_set_s mpu_init_cfg[] = {
    {MPU_PWR_MGMT_1_ADDR,   (1 << MPU_PWR_MGMT_1_RESET_SHFT)},     // reset
    {MPU_GYRO_CONFIG_ADDR,  (0 << MPU_GYRO_CONFIG_FS_SEL_SHFT)},   // set gyro to +-250 d/s
    {MPU_ACCEL_CONFIG_ADDR, (0 << MPU_ACCEL_CONFIG_AFS_SEL_SHFT)}, // set accel to +-2g
    
    {MPU_PWR_MGMT_1_ADDR, 0
      | (1 << MPU_PWR_MGMT_1_CYCLE_SHFT)
      /*| (1 << MPU_PWR_MGMT_1_TEMP_DIS_SHFT)*/
    }, // wake
    
    
    {MPU_PWR_MGMT_2_ADDR, 0
      | (0 << MPU_PWR_MGMT_2_STBY_XG_SHFT)
      | (0 << MPU_PWR_MGMT_2_STBY_YG_SHFT)
      | (0 << MPU_PWR_MGMT_2_STBY_ZG_SHFT)
      | (1 << MPU_PWR_MGMT_2_LP_WAKE_CTRL_SHFT) // sampling rate 5 Hz
    
    },
    
    {MPU_INT_PIN_CFG_ADDR, 0
      | (1 << MPU_INT_PIN_CFG_LATCH_INT_EN_SHFT)
      //| (1 << MPU_INT_PIN_CFG_INT_OPEN_SHFT)
      //| (1 << MPU_INT_PIN_CFG_INT_LEVEL_SHFT)
    },
    
    {MPU_INT_ENABLE_ADDR, 0
      | (1 << MPU_INT_ENABLE_DATA_RDY_EN_SHFT)
    }, // enable interrupt
};

static void mpu_regwrite(uint8_t addr, uint8_t val)
{
    uint8_t buf[2];
    gmd_io_tx_t tx = { .isw = 1, .buf = buf, .len = 2, .off = 0 };
    
    buf[0] = addr;
    buf[1] = val;
    
    LOG_INFO(MPU, "reg %02x: %02x", addr, val);
    gmd_i2c_sg(MPU_ADDR, &tx, 1, 0);
}

static uint8_t mpu_regread(uint8_t addr)
{
    uint8_t val;
    
    gmd_io_tx_t tx[2] = {
        { .isw = 1, .buf = &addr, .len = 1, .off = 0 },
        { .isw = 0, .buf = &val,  .len = 1, .off = 0 }
    };
    
    gmd_i2c_sg(MPU_ADDR, tx, 2, 0);
    
    return val;
}

int16_t mpu_get_temperature()
{
    uint8_t addr = MPU_TEMP_OUT_ADDR;
    int16_t val;
    float temp;
    
    gmd_io_tx_t tx[2] = {
        { .isw = 1, .buf = &addr, .len = 1, .off = 0 },
        { .isw = 0, .buf = (uint8_t*)&val,  .len = 2, .off = 0 }
    };
    
    gmd_i2c_sg(MPU_ADDR, tx, 2, 0);
    
    val = ntohs(val);
    
    LOG_INFO(MPU, "raw temperature (%02x): %04x", addr, val);
    
    val -= MPU_TEMPERATURE_OFFSET; // offset
    temp = 36.53f + (val - MPU_TEMPERATURE_OFFSET) / ((float)MPU_TEMPERATURE_SCALE);

    return (int16_t)(temp*1000);
}

void mpu_get_accel(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t addr = MPU_ACCEL_XOUT_ADDR;
    int16_t buff[3];
    int16_t val;
    
    gmd_io_tx_t tx[2] = {
        { .isw = 1, .buf = &addr, .len = 1, .off = 0 },
        { .isw = 0, .buf = (uint8_t*)buff, .len = 6, .off = 0 }
    };
    
    gmd_i2c_sg(MPU_ADDR, tx, 2, 0);
    
    val = buff[0];
    val = ntohs(val);
    *x = (int16_t)roundf(1000 * (val / 16384.0f));
    
    val = buff[1];
    val = ntohs(val);
    *y = (int16_t)roundf(1000 * (val / 16384.0f));
    
    val = buff[2];
    val = ntohs(val);
    *z = (int16_t)roundf(1000 * (val / 16384.0f));
}


void mpu_get_gyro(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t addr = MPU_GYRO_XOUT_ADDR;
    int16_t buff[3];
    int16_t val;
    
    gmd_io_tx_t tx[2] = {
        { .isw = 1, .buf = &addr, .len = 1, .off = 0 },
        { .isw = 0, .buf = (uint8_t*)buff, .len = 6, .off = 0 }
    };
    
    gmd_i2c_sg(MPU_ADDR, tx, 2, 0);
    
    val = buff[0];
    val = ntohs(val);
    *x = (int16_t)(1000 * (val / 131.0));
    
    val = buff[1];
    val = ntohs(val);
    *y = (int16_t)(1000 * (val / 131.0));
    
    val = buff[2];
    val = ntohs(val);
    *z = (int16_t)(1000 * (val / 131.0));
}

void mpu_init()
{
    uint8_t i;
    uint8_t mpu_addr = 0;
    
    for (i=0; i < ARR_SIZE(mpu_init_cfg); ++i) {
        mpu_regwrite(mpu_init_cfg[i].addr, mpu_init_cfg[i].val);
    }
    
    // sanity
    mpu_addr = mpu_regread(MPU_WHO_AM_I_ADDR);
    
    if (MPU_ADDR != mpu_addr) {
        LOG_ERROR(MPU, "invalid addr: %x", mpu_addr);
        gmd_panic();
    }

    // wait for first interrupt
    for (i=0; 0 == mpu_get_status(); ++i);
    
    LOG_INFO(MPU, "done setting mpu: %d, %x", i, mpu_addr);
    
}

uint8_t mpu_get_status()
{
    return mpu_regread(MPU_INT_STATUS_ADDR);
}

