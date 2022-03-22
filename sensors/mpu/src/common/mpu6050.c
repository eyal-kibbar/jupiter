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

#define MPU_CALIBRATION_FACTOR 64

#define MPU_SAMPLING_FREQ_Hz 50
#define MPU_SAMPLING_T_SEC (1.0/MPU_SAMPLING_FREQ_Hz)

struct mpu_reg_set_s {
    uint8_t addr;
    uint8_t val;
};

struct mpu_calib_s {
    //float gyro_err[3];
    int16_t gyro_err[3];
};


struct mpu_fifo_pkt_s {

#ifdef MPU_SENSOR_ACCEL_ENABLED
	int16_t accel[3];
#endif

#ifdef MPU_SENSOR_TEMP_ENABLED
	int16_t temperature;
#endif

#ifdef MPU_SENSOR_GYRO_ENABLED
	int16_t gyro[3];
#endif

} __attribute__((packed));


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

    {MPU_PWR_MGMT_1_ADDR, 0
      /*| (1 << MPU_PWR_MGMT_1_CYCLE_SHFT) // sleep between samples*/
      /*| (1 << MPU_PWR_MGMT_1_TEMP_DIS_SHFT)*/
      | 3 << MPU_PWR_MGMT_1_CLKSEL_SHFT
    }, // wake

    {MPU_GYRO_CONFIG_ADDR,  (MPU_GYRO_CONFIG_FS_SEL_VAL << MPU_GYRO_CONFIG_FS_SEL_SHFT)},
    {MPU_ACCEL_CONFIG_ADDR, (MPU_ACCEL_CONFIG_FS_SEL_VAL << MPU_ACCEL_CONFIG_AFS_SEL_SHFT)},


    /*
    {MPU_PWR_MGMT_2_ADDR, 0
      | (0 << MPU_PWR_MGMT_2_STBY_XG_SHFT)
      | (0 << MPU_PWR_MGMT_2_STBY_YG_SHFT)
      | (0 << MPU_PWR_MGMT_2_STBY_ZG_SHFT)
      | (0 << MPU_PWR_MGMT_2_LP_WAKE_CTRL_SHFT) // sampling rate 1.25 Hz

    },
    */

    {MPU_CONFIG_ADDR, 0
      | (0x4 << MPU_CONFIG_EXT_SYNC_SET_SHFT) // set gyro Z as the clock source
      | (0x3 << MPU_CONFIG_DLPF_CFG_SHFT) // set digital low pass filter
    },

    {MPU_INT_PIN_CFG_ADDR, 0
      //| (1 << MPU_INT_PIN_CFG_LATCH_INT_EN_SHFT)
      //| (1 << MPU_INT_PIN_CFG_INT_OPEN_SHFT)
      //| (1 << MPU_INT_PIN_CFG_INT_LEVEL_SHFT)
    },

    {MPU_INT_ENABLE_ADDR, 0
       | (1 << MPU_INT_ENABLE_DATA_RDY_EN_SHFT)
    }, // enable interrupt

    {MPU_FIFO_EN_ADDR, 0
#ifdef MPU_SENSOR_GYRO_ENABLED
        | (1 << MPU_FIFO_EN_XG_FIFO_EN_SHFT)
        | (1 << MPU_FIFO_EN_YG_FIFO_EN_SHFT)
        | (1 << MPU_FIFO_EN_ZG_FIFO_EN_SHFT)
#endif

#ifdef MPU_SENSOR_ACCEL_ENABLED
        | (1 << MPU_FIFO_EN_ACCEL_FIFO_EN_SHFT)
#endif

// TODO: enable temperature if needed
    },

    {MPU_SMPRT_DIV_ADDR, (1000 / MPU_SAMPLING_FREQ_Hz) - 1}, // set sampling rate
};


static void mpu_regwrite(uint8_t addr, uint8_t val)
{
    uint8_t buf[2];
    io_tx_t tx = { .mode = IO_TX_MODE_W, .buf = buf, .len = 2, .off = 0 };

    buf[0] = addr;
    buf[1] = val;

    //LOG_INFO(MPU, "reg %02x: %02x", addr, val);

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

static uint16_t mpu_regread16(uint8_t addr)
{
    uint16_t val;

    io_tx_t tx[2] = {
        { .mode = IO_TX_MODE_W, .buf = &addr, .len = 1, .off = 0 },
        { .mode = IO_TX_MODE_R, .buf = (uint8_t*)&val,  .len = 2, .off = 0 }
    };

    io_i2c_tx_begin(MPU_ADDR);
    io_i2c_master_sg(tx, 2, 0);
    io_i2c_tx_end();
    return val;
}

static void mpu_reset()
{
    mpu_regwrite(MPU_PWR_MGMT_1_ADDR, 1 << MPU_PWR_MGMT_1_RESET_SHFT);

    gmd_delay(100);

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
        //data->gyro[i] = rawdata->gyro[i] * MPU_GYRO_COEF;
        data->gyro[i] = (rawdata->gyro[i]-mpu_calib.gyro_err[i]) * MPU_GYRO_COEF;
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

    mpu_reset();

    //mpu_reset();



    for (i=0; i < ARR_SIZE(mpu_init_cfg); ++i) {
        mpu_regwrite(mpu_init_cfg[i].addr, mpu_init_cfg[i].val);
    }



    //mpu_regwrite(MPU_USER_CTRL_ADDR, MPU_USER_CTRL_FIFO_RESET_BMSK);
    mpu_regwrite(MPU_USER_CTRL_ADDR, MPU_USER_CTRL_FIFO_EN_BMSK | MPU_USER_CTRL_FIFO_RESET_BMSK);


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
    mpu_rawdata_t rawdata[4];
    uint8_t batch_idx, axis, sz;
    uint16_t sample_cnt;
    int32_t gyro_err[ARR_SIZE(rawdata[0].gyro)];

    LOG_INFO(MPU, "calibrating");

    //mpu_regwrite(MPU_USER_CTRL_ADDR, MPU_USER_CTRL_FIFO_RESET_BMSK);


    memset(&mpu_calib, 0, sizeof(mpu_calib));
    memset(gyro_err, 0, sizeof(gyro_err));

    for (sample_cnt=0; sample_cnt < MPU_CALIBRATION_FACTOR;) {
        sz = MIN(ARR_SIZE(rawdata), MPU_CALIBRATION_FACTOR - sample_cnt);
        mpu_pipe_read(rawdata, &sz);
        if (sz == 0) {
            //LOG_INFO(MPU, "waiting %d", sample_cnt);
            gmd_delay(50);
            continue;
        }
        for (batch_idx=0; batch_idx < sz; ++batch_idx) {
            for (axis=0; axis < ARR_SIZE(gyro_err); ++axis) {
                gyro_err[axis] += (int32_t)rawdata[batch_idx].gyro[axis];
            }
        }
        sample_cnt += (uint16_t)sz;
    }

    for (axis=0; axis < ARR_SIZE(gyro_err); ++axis) {
        mpu_calib.gyro_err[axis] = (int16_t)(gyro_err[axis] / MPU_CALIBRATION_FACTOR);
    }

    //gmd_delay(2000);
    {
        uint8_t status = mpu_get_status();
        LOG_INFO(MPU, "%02x", status);
        /*
        LOG_INFO(MPU, "calibration done: %d %d %d",
            gyro_err[0],
            gyro_err[1],
            gyro_err[2]);
            */
    }


}

static uint8_t mpu_pipe_read_aux(struct mpu_fifo_pkt_s *pkt_arr, uint8_t *sz)
{
    uint16_t fifo_sz;
    uint8_t available;
    uint8_t addr = MPU_FIFO_R_W_ADDR;


    io_tx_t tx[2] = {
        { .mode = IO_TX_MODE_W, .buf = &addr,             .len = 1, .off = 0 },
        { .mode = IO_TX_MODE_R, .buf = (uint8_t*)pkt_arr, .len = 0, .off = 0 }
    };

    fifo_sz = mpu_regread16(MPU_FIFO_COUNT_ADDR);

    fifo_sz = (int16_t)ntohs(fifo_sz);

    if (0 == fifo_sz) {
        *sz = 0;
        return 0;
    }

    available = (uint8_t)(fifo_sz / sizeof(struct mpu_fifo_pkt_s));
    *sz = MIN(available, *sz);
    tx[1].len = (*sz) * sizeof(struct mpu_fifo_pkt_s);

    io_i2c_tx_begin(MPU_ADDR);
    io_i2c_master_sg(tx, 2, 0);
    io_i2c_tx_end();

    return (available - (*sz));
}

uint8_t mpu_pipe_read(mpu_rawdata_t* rawdata_arr, uint8_t* sz)
{
    uint8_t i, available;
    // use provided buffer end. mpu_rawdata_t size is always larger or equal to mpu_fifo_pkt_s size
    COMPILER_CHECK(sizeof(struct mpu_fifo_pkt_s) <= sizeof(struct mpu_fifo_pkt_s));
    struct mpu_fifo_pkt_s* buff = (struct mpu_fifo_pkt_s*)(((uint8_t*)rawdata_arr) + (*sz) * (sizeof(mpu_rawdata_t) - sizeof(struct mpu_fifo_pkt_s)));

    available = mpu_pipe_read_aux(buff, sz);
    for (i=0; i < (*sz); ++i) {

        rawdata_arr[i].accel[0] = ntohs(buff[i].accel[0]);
        rawdata_arr[i].accel[1] = ntohs(buff[i].accel[1]);
        rawdata_arr[i].accel[2] = ntohs(buff[i].accel[2]);

        //rawdata_arr[i].temperature = ntohs(buff[i].temperature);

        rawdata_arr[i].gyro[0] = ntohs(buff[i].gyro[0]);
        rawdata_arr[i].gyro[1] = ntohs(buff[i].gyro[1]);
        rawdata_arr[i].gyro[2] = ntohs(buff[i].gyro[2]);
    }

    return available;
}

#define MPU_ANGLE_FILTER_ALPHA 0.98

float angles[3];

void mpu_ypr(float* yaw, float* pitch, float* roll)
{
    mpu_rawdata_t rawdata[4];
    mpu_data_t data;
    uint8_t batch_idx, sz;
    uint8_t available;
    uint16_t sample_cnt;
    float accel_angles[3];
    float gyro_angles[3];
    float yaw_sin;

    do {
        sz = ARR_SIZE(rawdata);
        available = mpu_pipe_read(rawdata, &sz);

        for (batch_idx=0; batch_idx < sz; ++batch_idx) {
            mpu_raw_parse(&rawdata[batch_idx], &data);

            accel_angles[0] = atan2(data.accel[1], data.accel[2]) * (180 / M_PI);
            accel_angles[1] = atan2(data.accel[0], data.accel[2]) * (180 / M_PI);

            gyro_angles[0] =  data.gyro[0] * MPU_SAMPLING_T_SEC;
            gyro_angles[1] = -data.gyro[1] * MPU_SAMPLING_T_SEC;
            gyro_angles[2] =  data.gyro[2] * MPU_SAMPLING_T_SEC;

            yaw_sin = sin(gyro_angles[2] * (M_PI/180));


            *pitch = angles[0];
            *roll = angles[1];
            angles[0] = (angles[0] + gyro_angles[0]) - ((*roll) * yaw_sin);
            angles[1] = (angles[1] + gyro_angles[1]) + ((*pitch) * yaw_sin);

            // apply complementary filter for pitch and roll
            angles[0] = (MPU_ANGLE_FILTER_ALPHA * angles[0]) + ((1-MPU_ANGLE_FILTER_ALPHA) * accel_angles[0]);
            angles[1] = (MPU_ANGLE_FILTER_ALPHA * angles[1]) + ((1-MPU_ANGLE_FILTER_ALPHA) * accel_angles[1]);
            angles[2] = angles[2] + gyro_angles[2]; // use only gyro for yaw
        }
    } while (available);

    *pitch = angles[0];
    *roll = angles[1];
    *yaw = angles[2];
}
