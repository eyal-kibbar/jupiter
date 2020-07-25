#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <stdint.h>


typedef struct nrf_cfg_s {
    uint8_t csn_pin;
    uint8_t ce_pin;
    uint8_t irq_pin;
} nrf_cfg_t;


void nrf_init(const nrf_cfg_t* cfg);

/**
 * initialize a transmission pipe
 * pipe 0 rx address is overwritten for auto ACKs
 *
 * @param [IN] addr     - transmission address
 * @param [IN] addr_len - number of bytes in transmission address. Truncated at 5 bytes
 **/
void nrf_send_open_pipe(uint8_t* addr, uint8_t addr_len);

/**
 * initialize a receive pipe
 * pipe0 and pipe1 use 5 bytes addresses
 * pipe2, pipe3, pipe4 use prefix 4 bytes of pipe0 address + first byte of provided address
 *
 * @param [IN] pipe_idx     - index of the pipe to be initialized for receive
 * @param [IN] addr         - receive address
 * @param [IN] addr_len     - number of bytes in receive address
 */
void nrf_recv_open_pipe(uint8_t pipe_idx, uint8_t* addr, uint8_t addr_len);

void nrf_recv_set();
void nrf_send_set();

void nrf_send(uint8_t* payload, uint8_t payload_len);
void nrf_recv(uint8_t* payload, uint8_t* max_payload_len, uint8_t* pipe_idx);





#endif /* NRF24L01_H_ */
