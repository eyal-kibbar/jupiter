#include "servo.h"
#include "ganymede.h"
#include "logging.h"

static int dir = 1;
static uint8_t k = 0;

static void setup()
{
    
    servo_init();
    
    servo_attach(6);
    servo_attach(7);
    servo_attach(8);


}

static void loop()
{
    LOG_INFO(SERVO, "loop %d", k);
    
    //gmd_uart_write("aa\r\n",4);
    k += dir*16;
    
    if (k == 0) { 
        dir *= -1;
    }
    
    
    servo_set(7, k);
    //servo_set(8, 255-k);
    
    gmd_delay(100);
}


TASK(STACK_SIZE);



