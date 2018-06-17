#include "srv.h"
#include "ganymede.h"
#include "logging.h"
#include <string.h>


static uint8_t srv_buf[SRV_BUF_SIZE];
static uint8_t srv_delimiter[] = { 'a', 'b', 'c' };
static uint8_t srv_is_flushing = 1;

static gmd_io_tx_t srv_tx =
    { .isw = 0, .buf = srv_buf, .len = sizeof(srv_buf), .off = 0 };


static void srv_io_sg(gmd_io_tx_t* tx, uint16_t timeout_ms)
{
    gmd_uart_sg(tx, 1, timeout_ms);
}

static int srv_delimiter_idx()
{
    uint8_t* dlm;
    if (NULL == (dlm = memchr(srv_tx.buf, srv_delimiter[0], srv_tx.off))) {
        return -1;
    }
    
    return (uint8_t)(dlm - srv_tx.buf);
}

static void srv_reset_buf(int idx)
{
    memmove(srv_tx.buf, srv_tx.buf + idx, srv_tx.off-idx);
    srv_tx.off -= idx;
}

static void srv_flush(gmd_io_tx_t* pkt, int idx)
{
    LOG_INFO(SRV, "flushing %d bytes", idx);
    memcpy(pkt->buf + pkt->off, srv_tx.buf, idx);
    pkt->off += idx;
}

static int srv_available(gmd_io_tx_t* pkt)
{
    return pkt->len - pkt->off;
}

static int srv_has_delimiter()
{
    return 0 == memcmp(srv_delimiter, srv_tx.buf, sizeof srv_delimiter);
}

static int srv_do_recv(gmd_io_tx_t* pkt)
{
    int idx;
    
    do {

        if (0 == srv_tx.off) {
            return 0;
        }

        if (0 == (idx = srv_delimiter_idx())) {

            if (srv_tx.off < sizeof(srv_delimiter)) {
                LOG_INFO(SRV, "not enough bytes (%d) read to check for delimtier", srv_tx.off);
                return 0;
            }

            if (srv_has_delimiter()) {
                LOG_INFO(SRV, "delimiter found, reset buffer");
                srv_reset_buf(sizeof srv_delimiter);

                if (srv_is_flushing) {
                    return 1;
                }

                LOG_INFO(SRV, "not flushing, continue looking for the next delimiter");
                srv_is_flushing = 1;
                continue;
            }

            idx = 1;
            LOG_INFO(SRV, "no delimiter, flush 1 byte");
        }

        if (idx < 0) {
            idx = srv_tx.off;
        }
        
        if (srv_available(pkt) < idx) {
            LOG_INFO(SRV, "pkt is full, but no delimiter found, drop the current packet");
            srv_is_flushing = 0;
            pkt->off = 0;
        }

        // flush only if we are not in an error state
        // where pkt max size was too small
        if (srv_is_flushing) {
            srv_flush(pkt, idx);
        }

        srv_reset_buf(idx);

    } while (1);

    return 0;
}

uint8_t srv_recv(void* pkt, uint8_t max_size)
{
    gmd_io_tx_t pkt_tx;
    
    pkt_tx.buf = pkt;
    pkt_tx.len = max_size;
    pkt_tx.off = 0;
    
    LOG_INFO(SRV, "flush any leftover bytes");
    if (srv_do_recv(&pkt_tx)) {
        return pkt_tx.off;
    }
    
    LOG_INFO(SRV, "wait indefently for 1 byte to come");
    srv_tx.len = srv_tx.off + 1;
    srv_io_sg(&srv_tx, 0);
    
    LOG_INFO(SRV, "flush the 1 byte read");
    if (srv_do_recv(&pkt_tx)) {
        return pkt_tx.off;
    }
    
    LOG_INFO(SRV, "wait for packet to be read, within the time limit");
    srv_tx.len = sizeof(srv_buf);
    srv_io_sg(&srv_tx, SRV_PKT_TIMEOUT_MS);
    
    LOG_INFO(SRV, "flush packet");
    if (srv_do_recv(&pkt_tx)) {
        return pkt_tx.off;
    }
    
    LOG_INFO(SRV, "pkt timed out. reset buffer");
    srv_is_flushing = 0;
    return 0;
}


void srv_send(void* pkt, uint8_t size)
{
    gmd_io_tx_t pkt_tx = 
    { .isw = 1, .buf = pkt, .len = size, .off = 0 };
    
    srv_io_sg(&pkt_tx, 0);
}



