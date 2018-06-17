#ifndef MPU_H_
#define MPU_H_


void mpu_init();


int16_t mpu_get_temperature();

uint8_t mpu_get_status();

void mpu_get_gyro(int16_t *x, int16_t *y, int16_t *z);
void mpu_get_accel(int16_t *x, int16_t *y, int16_t *z);
    


#endif /* MPU_H */
