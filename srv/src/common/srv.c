#include "srv.h"
#include "ganymede.h"
#include "logging.h"
#include "utils.h"
#include <string.h>



static void srv_io_sg(gmd_io_tx_t* tx, uint16_t timeout_ms)
{
    gmd_uart_sg(tx, 1, timeout_ms);
}

uint8_t srv_recv(void* pkt, uint8_t max_size)
{
    uint8_t* pkt_buf = (uint8_t*)pkt;
    uint8_t n = 0;
    uint8_t i;
    uint8_t is_valid = 1;
    uint8_t buf[sizeof(SRV_DELIMITER)-1];
    gmd_io_tx_t tx = {.isw=0, .buf=buf, .len=sizeof(buf), .off=0 };
    
    
    do {
        srv_io_sg(&tx, 0);

        // check for delimiter
        if (0 == memcmp(SRV_DELIMITER, buf, sizeof(buf))) {
            
            // delimiter found !
            if (is_valid) {
                return n;
            }
            
            // flush it
            tx.off = 0;
            is_valid = 1;
            continue;
        }

        // flush at least 1 byte
        i=0;
        do {
            ++i;
            
            if (!is_valid) {
                continue;
            }
            
            if (n < max_size) {
                pkt_buf[n++] = buf[i-1];
                continue;
            }
            
            // pkt is full but no delimiter found
            n = 0;
            is_valid = 0;
            
        } while (i < ARR_SIZE(buf) &&
                 SRV_DELIMITER[0] != buf[i]);

        // reset buffer to the begining of the delimiter
        memmove(buf, buf + i, tx.off - i);
        tx.off -= i;
    } while (1);
}


void srv_send(void* pkt, uint8_t size)
{
    gmd_io_tx_t pkt_tx = 
    { .isw = 1, .buf = pkt, .len = size, .off = 0 };
    
    srv_io_sg(&pkt_tx, 0);
}



