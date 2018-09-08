#ifndef MPU_H_
#define MPU_H_


typedef struct mpu_data_s
{
	// $rad / sec^2$
	float accel[3];
	
	// $rad/sec$
	float gyro[3];
	
	float temperature;
} mpu_data_t;

void mpu_init();

uint8_t mpu_get_status();

void mpu_read(mpu_data_t* data);

#endif /* MPU_H */
