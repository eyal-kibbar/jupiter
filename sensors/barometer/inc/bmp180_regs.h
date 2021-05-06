#ifndef BMP180_REGS_H_
#define BMP180_REGS_H_

#define BMP_ADDR 0x77

#define BMP_CHIP_ID_ADDR    0xD0
#define BMP_CHIP_ID         0x55

#define BMP_SOFT_RESET_ADDR 0xE0
#define BMP_SOFT_RESET      0xB6

#define BMP_CALIB_ADDR      0xAA

#define BMP_CTRL_MEAS_ADDR  0xF4
#define BMP_CTRL_MEAS_MEASUREMENT_CONTROL_SHFT 0x00
#define BMP_CTRL_MEAS_MEASUREMENT_CONTROL_BMSK 0x1F
#define BMP_CTRL_MEAS_SCO_SHFT 0x5
#define BMP_CTRL_MEAS_SCO_BMSK 0x20
#define BMP_CTRL_MEAS_OSS_SHFT 0x6
#define BMP_CTRL_MEAS_OSS_BMSK 0xC0

#define BMP_OUT_ADDR 0xF6

#endif /* BMP180_REGS_H_ */
