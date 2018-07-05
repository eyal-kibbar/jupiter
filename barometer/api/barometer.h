#ifndef BAROMETER_H_
#define BAROMETER_H_

typedef struct barometer_s {
    float height_m;
    
} barometer_t;


void barometer_init();

void barometer_get(barometer_t* out);


#endif /* BAROMETER_H_ */
