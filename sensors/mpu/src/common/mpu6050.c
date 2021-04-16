#include "ganymede.h"
#include "logging.h"
#include "io.h"

#include "mpu6050_regs.h"
#include "mpu.h"

#include <stdio.h>
#include <string.h>
#include <math.h>


#define htons(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
#define ntohs htons

#define MPU_TEMPERATURE_OFFSET (0)
#define MPU_TEMPERATURE_SCALE (340)

#define MPU_CALIBRATION_FACTOR 200

#define ARR_SIZE(x) (sizeof(x) / sizeof(*(x)))

struct mpu_reg_set_s {
    uint8_t addr;
    uint8_t val;
};

struct mpu_calib_s {
    float gyro_err[3];
};


struct mpu_calib_s mpu_calib;

static struct mpu_reg_set_s mpu_init_cfg[] = {
    {MPU_PWR_MGMT_1_ADDR,   (1 << MPU_PWR_MGMT_1_RESET_SHFT)},     // reset
    {MPU_GYRO_CONFIG_ADDR,  (0 << MPU_GYRO_CONFIG_FS_SEL_SHFT)},   // set gyro to +-250 d/s
    {MPU_ACCEL_CONFIG_ADDR, (0 << MPU_ACCEL_CONFIG_AFS_SEL_SHFT)}, // set accel to +-2g

    {MPU_PWR_MGMT_1_ADDR, 0
      /*| (1 << MPU_PWR_MGMT_1_CYCLE_SHFT) // sleep between samples*/
      /*| (1 << MPU_PWR_MGMT_1_TEMP_DIS_SHFT)*/
    }, // wake


    {MPU_PWR_MGMT_2_ADDR, 0
      | (0 << MPU_PWR_MGMT_2_STBY_XG_SHFT)
      | (0 << MPU_PWR_MGMT_2_STBY_YG_SHFT)
      | (0 << MPU_PWR_MGMT_2_STBY_ZG_SHFT)
      | (0 << MPU_PWR_MGMT_2_LP_WAKE_CTRL_SHFT) // sampling rate 1.25 Hz

    },

    {MPU_CONFIG_ADDR, 0
      | (0x4 << MPU_CONFIG_EXT_SYNC_SET_SHFT) // set gyro Z as the clock source
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
    io_tx_t tx = { .mode = IO_TX_MODE_W, .buf = buf, .len = 2, .off = 0 };

    buf[0] = addr;
    buf[1] = val;

    LOG_INFO(MPU, "reg %02x: %02x", addr, val);
    io_i2c_tx_begin(MPU_ADDR);
    io_i2c_master_sg(&tx, 1, 0);
    io_i2c_tx_end();
}

static uint8_t mpu_regread(uint8_t addr)
{
    uint8_t val;

    io_tx_t tx[2] = {
        { .mode = IO_TX_MODE_W, .buf = &addr, .len = 1, .off = 0 },
        { .mode = IO_TX_MODE_R, .buf = &val,  .len = 1, .off = 0 }
    };

    io_i2c_tx_begin(MPU_ADDR);
    io_i2c_master_sg(tx, 2, 0);
    io_i2c_tx_end();
    return val;
}

void mpu_read(mpu_data_t* data)
{
    uint8_t addr = MPU_ACCEL_XOUT_ADDR;
    int16_t buff[7];
    int16_t val;
    int i;

    // mpu registers: 6 accel regs, 2 temperature regs, 6 gyro regs
    io_tx_t tx[2] = {
        { .mode = IO_TX_MODE_W, .buf = &addr, .len = 1, .off = 0 },
        { .mode = IO_TX_MODE_R, .buf = (uint8_t*)buff,  .len = sizeof(buff), .off = 0 }
    };

    io_i2c_tx_begin(MPU_ADDR);
    io_i2c_master_sg(tx, 2, 0);
    io_i2c_tx_end();

    memset(data, 0, sizeof *data);

    for (i=0; i < 3; ++i) {
        val = (int16_t)ntohs(buff[0 + i]);
        data->accel[i] = val / 16384.0f; // assumes accel full-scale range of +- 2g
    }

    val = (int16_t)ntohs(buff[3]);
    data->temperature = 36.53f + (val - MPU_TEMPERATURE_OFFSET) / ((float)MPU_TEMPERATURE_SCALE);

    for (i=0; i < 3; ++i) {
        val = (int16_t)ntohs(buff[4 + i]);
        data->gyro[i] = val / 131.0f; // assumes gyro full-scale range of +- 250 d/s
        data->gyro[i] -= mpu_calib.gyro_err[i];
    }

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

    if (MPU_WHO_AM_I_POR != mpu_addr) {
        LOG_ERROR(MPU, "invalid addr: %x", mpu_addr);
        gmd_panic();
    }

    // wait for first interrupt
    for (i=0; 0 == mpu_get_status(); ++i);

    LOG_INFO(MPU, "done setting mpu: %d, %x", i, mpu_addr);

}

uint8_t mpu_get_status()
{
    uint8_t val;
    val = mpu_regread(MPU_INT_STATUS_ADDR);
    //mpu_regwrite(MPU_INT_STATUS_ADDR, 0);
    return val;
}

void mpu_calibrate()
{
    uint8_t i, j;
    struct mpu_calib_s calib;
    mpu_data_t data;

    memset(&mpu_calib, 0, sizeof(mpu_calib));
    memset(&calib, 0, sizeof(calib));

    for (j=0; j < MPU_CALIBRATION_FACTOR; ++j) {

        // wait for data to be ready
        for (i=0; 0 == mpu_get_status(); ++i);

        // acumulate error, assuming the sensor is not moving
        mpu_read(&data);
        for (i=0; i < ARR_SIZE(calib.gyro_err); ++i) {
            calib.gyro_err[i] += data.gyro[i];
        }
    }

    // calculate error average over all the collected samples
    for (i=0; i < ARR_SIZE(calib.gyro_err); ++i) {
        mpu_calib.gyro_err[i] = calib.gyro_err[i] / MPU_CALIBRATION_FACTOR;
    }
}
