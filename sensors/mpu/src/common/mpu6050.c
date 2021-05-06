#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "utils.h"

#include "mpu6050_regs.h"
#include "mpu.h"

#include <stdio.h>
#include <string.h>
#include <math.h>


#define MPU_TEMPERATURE_OFFSET (0)
#define MPU_TEMPERATURE_SCALE (340)

#define MPU_CALIBRATION_FACTOR 200

struct mpu_reg_set_s {
    uint8_t addr;
    uint8_t val;
};

struct mpu_calib_s {
    //float gyro_err[3];
    int16_t gyro_err[3];
};


struct mpu_calib_s mpu_calib;

#if MPU_GYRO_FULL_SCALE_RANGE == 250
    #define MPU_GYRO_CONFIG_FS_SEL_VAL 0
    #define MPU_GYRO_COEF (1/131.0f)
#elif MPU_GYRO_FULL_SCALE_RANGE == 500
    #define MPU_GYRO_CONFIG_FS_SEL_VAL 1
    #define MPU_GYRO_COEF (1/65.5f)
#elif MPU_GYRO_FULL_SCALE_RANGE == 1000
    #define MPU_GYRO_CONFIG_FS_SEL_VAL 2
    #define MPU_GYRO_COEF (1/32.8f)
#elif MPU_GYRO_FULL_SCALE_RANGE == 2000
    #define MPU_GYRO_CONFIG_FS_SEL_VAL 3
    #define MPU_GYRO_COEF (1/16.4f)
#else
    #error "MPU_GYRO_FULL_SCALE_RANGE has invalid value"
#endif


#if MPU_ACCEL_FULL_SCALE_RANGE == 2
    #define MPU_ACCEL_CONFIG_FS_SEL_VAL 0
    #define MPU_ACCEL_COEF (1/16384.0f)
#elif MPU_ACCEL_FULL_SCALE_RANGE == 4
    #define MPU_ACCEL_CONFIG_FS_SEL_VAL 1
    #define MPU_ACCEL_COEF (1/8192.0f)
#elif MPU_ACCEL_FULL_SCALE_RANGE == 8
    #define MPU_ACCEL_CONFIG_FS_SEL_VAL 2
    #define MPU_ACCEL_COEF (1/4096.0f)
#elif MPU_ACCEL_FULL_SCALE_RANGE == 16
    #define MPU_ACCEL_CONFIG_FS_SEL_VAL 3
    #define MPU_ACCEL_COEF (1/2048.0f)
#else
    #error "MPU_ACCEL_FULL_SCALE_RANGE has invalid value"
#endif



static const struct mpu_reg_set_s mpu_init_cfg[] = {
    {MPU_PWR_MGMT_1_ADDR,   (1 << MPU_PWR_MGMT_1_RESET_SHFT)},     // reset
    {MPU_GYRO_CONFIG_ADDR,  (MPU_GYRO_CONFIG_FS_SEL_VAL << MPU_GYRO_CONFIG_FS_SEL_SHFT)},
    {MPU_ACCEL_CONFIG_ADDR, (MPU_ACCEL_CONFIG_FS_SEL_VAL << MPU_ACCEL_CONFIG_AFS_SEL_SHFT)}, // set accel to +-2g

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
      //| (1 << MPU_INT_PIN_CFG_LATCH_INT_EN_SHFT)
      //| (1 << MPU_INT_PIN_CFG_INT_OPEN_SHFT)
      //| (1 << MPU_INT_PIN_CFG_INT_LEVEL_SHFT)
    },

    {MPU_INT_ENABLE_ADDR, 0
       //| (1 << MPU_INT_ENABLE_DATA_RDY_EN_SHFT)
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

void mpu_raw_read(mpu_rawdata_t* rawdata)
{
    uint8_t addr = MPU_ACCEL_XOUT_ADDR;
    int16_t buff[7];
    int16_t val;
    int i;

    // mpu registers: 6 accel regs, 2 temperature regs, 6 gyro regs
    io_tx_t tx[2] = {
        { .mode = IO_TX_MODE_W, .buf = &addr,           .len = 1,            .off = 0 },
        { .mode = IO_TX_MODE_R, .buf = (uint8_t*)buff,  .len = sizeof(buff), .off = 0 }
    };

    io_i2c_tx_begin(MPU_ADDR);
    io_i2c_master_sg(tx, 2, 0);
    io_i2c_tx_end();

    memset(rawdata, 0, sizeof *rawdata);

    for (i=0; i < 3; ++i) {
        val = buff[0 + i];
        rawdata->accel[i] = (int16_t)ntohs(val);
    }

    val = buff[3];
    rawdata->temperature = (int16_t)ntohs(val);

    for (i=0; i < 3; ++i) {
        val = buff[4 + i];
        rawdata->gyro[i] = (int16_t)ntohs(val);
        rawdata->gyro[i] -= mpu_calib.gyro_err[i];
    }
}

void mpu_raw_parse(const mpu_rawdata_t* rawdata, mpu_data_t* data)
{
    uint8_t i;
    for (i=0; i < ARR_SIZE(data->accel); ++i) {
        data->accel[i] = rawdata->accel[i] * MPU_ACCEL_COEF;
    }

    data->temperature = 36.53f + (rawdata->temperature - MPU_TEMPERATURE_OFFSET) / ((float)MPU_TEMPERATURE_SCALE);

    for (i=0; i < ARR_SIZE(data->gyro); ++i) {
        data->gyro[i] = rawdata->gyro[i] * MPU_GYRO_COEF;
        //data->gyro[i] = (rawdata->gyro[i]-mpu_calib.gyro_err[i]) * MPU_GYRO_COEF;
    }
}

void mpu_read(mpu_data_t* data)
{
    mpu_rawdata_t rawdata;
    mpu_raw_read(&rawdata);
    mpu_raw_parse(&rawdata, data);
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
    //LOG_INFO(MPU, "status: %02x", val);
    //mpu_regwrite(MPU_INT_STATUS_ADDR, 0);
    return val;
}

void mpu_calibrate()
{
    uint8_t i, j;
    mpu_rawdata_t rawdata;
    int32_t gyro_err[ARR_SIZE(rawdata.gyro)];

    LOG_INFO(MPU, "calibrating");

    memset(&mpu_calib, 0, sizeof(mpu_calib));
    memset(gyro_err, 0, sizeof(gyro_err));

    for (j=0; j < MPU_CALIBRATION_FACTOR; ++j) {

        // wait for data to be ready
        for (i=0; 0 == mpu_get_status(); ++i);

        // acumulate error, assuming the sensor is not moving
        mpu_raw_read(&rawdata);
        for (i=0; i < ARR_SIZE(gyro_err); ++i) {
            gyro_err[i] += (int32_t)rawdata.gyro[i];
        }
    }

    // calculate error average over all the collected samples
    for (i=0; i < ARR_SIZE(gyro_err); ++i) {
        //mpu_calib.gyro_err[i] = ((float)gyro_err[i]) / MPU_CALIBRATION_FACTOR;
        mpu_calib.gyro_err[i] = (int16_t)(gyro_err[i] / MPU_CALIBRATION_FACTOR);
    }

    LOG_INFO(MPU, "calibration done: %d %d %d",
        gyro_err[0],
        gyro_err[1],
        gyro_err[2]);

}
