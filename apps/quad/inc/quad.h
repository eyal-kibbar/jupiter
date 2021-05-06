#ifndef QUAD_H_
#define QUAD_H_


void quad_start();
void quad_stop();
void quad_set_setpoint(float rpy[3], float throtte);

void quad_angles_get(float rpy[3]);
void quad_get_servo_microseconds(int16_t motor[4]);


#endif /* QUAD_H_ */
