#ifndef MPU_H_
#define MPU_H_


typedef struct mpu_data_s {
	// $degree / sec^2$
	float accel[3];

	// degree / sec
	float gyro[3];

	// celsius
	float temperature;
} mpu_data_t;


typedef struct mpu_rawdata_s {
	int16_t accel[3];
	int16_t temperature;
	int16_t gyro[3];
} mpu_rawdata_t;

void mpu_init();

uint8_t mpu_get_status();

void mpu_read(mpu_data_t* data);
void mpu_raw_read(mpu_rawdata_t* rawdata);
void mpu_raw_parse(const mpu_rawdata_t* rawdata, mpu_data_t* data);
void mpu_calibrate();

int mpu_pipe_read(mpu_rawdata_t* rawdata_arr, uint8_t* sz);

#endif /* MPU_H */
