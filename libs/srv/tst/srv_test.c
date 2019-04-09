#include "srv.h"
#include "ganymede.h"
#include "logging.h"


char str[4];

void setup()
{
    
}

void loop()
{
    int sz;
    
    sz = srv_recv(str, 3);
    
    LOG_INFO(SRV_TEST, "recved: %d bytes: '%s'", sz, str);
    
    srv_send(str, sz);
    
}


TASK(STACK_SIZE);


