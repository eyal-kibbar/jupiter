#include "servo.h"
#include "ganymede.h"
#include "logging.h"

static int dir = 5;
static uint8_t k = 50;

static void setup()
{
    
    servo_init();
    
    servo_attach(2);
    servo_attach(3);
    //servo_attach(4);


}

static void loop()
{
    LOG_INFO(SERVO, "loop %d", k);
    
    if (k <= 50) {
        dir = 10;
        k = 50;
    }
    
    if (k >= 120) {
        k = 120;
        dir = -10;
    }
    
    k += dir;
    
    
    
    
    
    servo_set(2, k);
    servo_set(3, 50 + 70 - (k-50));
    
    gmd_delay(1000);
}


TASK(STACK_SIZE);



