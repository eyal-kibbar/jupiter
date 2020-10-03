#include "nRF24L01.h"
#include "nrf_regs.h"
#include "nrf_cmd.h"
#include "ganymede.h"
#include "logging.h"
#include "io.h"
#include "utils.h"
#include <string.h>


#define NRF_ADDR_LEN 5
#define NRF_PAYLOAD_LEN 32


typedef struct nrf_s {
    uint8_t  csn_pin;
    uint8_t  ce_pin;
    uint8_t* irq_port;
    uint8_t  irq_mask;
} nrf_t;


nrf_t nrf;



uint8_t nrf_reg_read(uint8_t addr, uint8_t* val)
{
    uint8_t cmd[2];

    io_tx_t tx = {
        .mode = IO_TX_MODE_W | IO_TX_MODE_R,
        .len = 2,
        .off = 0,
        .buf = cmd
    };

    cmd[0] = NRF_CMD_R_REGISTER | addr;
    cmd[1] = NRF_CMD_NOP;

    io_spi_tx_begin(nrf.csn_pin);
    io_spi_master_sg(&tx, 1, 0);
    io_spi_tx_end();
    *val = cmd[1];
    return cmd[0];
}

uint8_t nrf_reg_write(uint8_t addr, uint8_t val)
{
    uint8_t cmd[2];

    io_tx_t tx = {
        .mode = IO_TX_MODE_W | IO_TX_MODE_R,
        .len = 2,
        .off = 0,
        .buf = cmd
    };

    cmd[0] = NRF_CMD_W_REGISTER | addr;
    cmd[1] = val;

    io_spi_tx_begin(nrf.csn_pin);
    io_spi_master_sg(&tx, 1, 0);
    io_spi_tx_end();
    return cmd[0]; // status register
}



void nrf_reg_write_buff(uint8_t addr, uint8_t* buff, uint8_t buff_len)
{
    uint8_t cmd = NRF_CMD_W_REGISTER | addr;

    io_tx_t tx[] = {
        {
            .mode = IO_TX_MODE_W,
            .len = 1,
            .off = 0,
            .buf = &cmd
        },
        {
            .mode = IO_TX_MODE_W,
            .len = buff_len,
            .off = 0,
            .buf = buff
        }
    };

    io_spi_tx_begin(nrf.csn_pin);
    io_spi_master_sg(tx, ARR_SIZE(tx), 0);
    io_spi_tx_end();
}

static uint8_t nrf_send_cmd(uint8_t cmd)
{
    io_tx_t tx = {
        .mode = IO_TX_MODE_W | IO_TX_MODE_R,
        .len = 1,
        .off = 0,
        .buf = &cmd
    };

    io_spi_tx_begin(nrf.csn_pin);
    io_spi_master_sg(&tx, 1, 0);
    io_spi_tx_end();
    return cmd;
}


void nrf_init(const nrf_cfg_t* cfg)
{


    nrf.csn_pin = cfg->csn_pin;
    nrf.ce_pin  = cfg->ce_pin;
    io_pin_port(cfg->irq_pin, &nrf.irq_port, &nrf.irq_mask);

    io_pin_input(cfg->irq_pin);
    io_pin_output(nrf.csn_pin);
    io_pin_output(nrf.ce_pin);

    io_pin_set(nrf.csn_pin); // reset SPI communication
    io_pin_clr(nrf.ce_pin); // chip enabled pin is set before TX/RX operations

    gmd_delay(100); // let the SPI communication reset

    // setup retransmit
    nrf_reg_write(NRF_SETUP_RETR_ADDR,
        (0x1 << NRF_SETUP_RETR_ARD_SHFT) | // auto retransmit delay: 500 microseconds
        (0xF << NRF_SETUP_RETR_ARC_SHFT)   // 15 retransmit attempts
    );

    // setup data rate
    nrf_reg_write(NRF_RF_SETUP_ADDR,
        (0x0 << NRF_RF_SETUP_RF_DR_LOW_SHFT) |  // 1Mbps
        (0x0 << NRF_RF_SETUP_RF_DR_HIGH_SHFT) | // 1Mbps
        (0x0 << NRF_RF_SETUP_RF_PWR_SHFT) // 18dBm
    );

    // reset state
    nrf_reg_write(NRF_STATUS_ADDR, NRF_STATUS_RX_DR_BMSK | NRF_STATUS_TX_DS_BMSK | NRF_STATUS_MAX_RT_BMSK);

    nrf_send_cmd(NRF_CMD_FLUSH_RX);
    nrf_send_cmd(NRF_CMD_FLUSH_TX);

    // set channel
    nrf_reg_write(NRF_RF_CH_ADDR, cfg->channel);


    // powerup
    nrf_reg_write(NRF_CONFIG_ADDR,
        (0x1 << NRF_CONFIG_CRCO_SHFT)   | // 2 bytes CRC
        (0x1 << NRF_CONFIG_PWR_UP_SHFT) | // power up device
        (0x1 << NRF_CONFIG_EN_CRC_SHFT)   // enable crc
    );

    gmd_delay(10);
}

void nrf_test()
{
    uint8_t val, status;
    uint8_t addr;


    for (addr = 0; addr <= 0x17; ++addr) {
        val = 0xFF;
        status = 0xFF;
        status = nrf_reg_read(addr, &val);
        LOG_INFO(NRF, "addr 0x%02x: 0x%02x status: 0x%02x", addr, val, status);
        gmd_delay(10);
    }



}

void nrf_send_open_pipe(uint8_t* addr, uint8_t addr_len)
{
    uint8_t full_addr[NRF_ADDR_LEN];

    memset(full_addr, 0, sizeof(full_addr));
    memcpy(full_addr, addr, MIN(addr_len, sizeof(full_addr)));


    // set pipe0 address for ACKs
    nrf_reg_write_buff(NRF_RX_ADDR_P0_ADDR, full_addr, sizeof(full_addr));

    // set transmission address TX_ADDR
    nrf_reg_write_buff(NRF_TX_ADDR_ADDR, full_addr, sizeof(full_addr));

    // set payload length
    nrf_reg_write(NRF_RX_PW_P0_ADDR, NRF_PAYLOAD_LEN);

    // auto ack is set by default
}

void nrf_recv_open_pipe(uint8_t pipe_idx, uint8_t* addr, uint8_t addr_len)
{
    uint8_t full_addr[NRF_ADDR_LEN];
    uint8_t full_addr_len;
    uint8_t reg_rxaddr;

    memset(full_addr, 0, sizeof(full_addr));
    memcpy(full_addr, addr, MIN(addr_len, sizeof(full_addr)));


    // set pipe address (only pipe0 and pipe1 support full address length)
    full_addr_len = pipe_idx < 2 ? sizeof(full_addr) : 1;
    nrf_reg_write_buff(NRF_RX_ADDR_P0_ADDR + pipe_idx, full_addr, full_addr_len);

    // set rx payload length
    nrf_reg_write(NRF_RX_PW_P0_ADDR + pipe_idx, NRF_PAYLOAD_LEN);

    // enable rx pipe
    nrf_reg_read(NRF_EN_RXADDR_ADDR, &reg_rxaddr);
    nrf_reg_write(NRF_EN_RXADDR_ADDR, reg_rxaddr | (1 << pipe_idx));

    // auto ack is set by default

}

void nrf_recv_set()
{
    int8_t reg_config;


    nrf_reg_read(NRF_CONFIG_ADDR, &reg_config);
    reg_config |= (1 << NRF_CONFIG_PRIM_RX_SHFT);
    nrf_reg_write(NRF_CONFIG_ADDR, reg_config);


    io_pin_set(nrf.ce_pin); // initialize RX
}

void nrf_send_set()
{
    uint8_t reg_config;

    io_pin_clr(nrf.ce_pin);


    nrf_reg_read(NRF_CONFIG_ADDR, &reg_config);
    reg_config &= ~(1 << NRF_CONFIG_PRIM_RX_SHFT);
    nrf_reg_write(NRF_CONFIG_ADDR, reg_config);

}

void nrf_send(uint8_t* payload, uint8_t payload_len)
{
    uint8_t full_payload[NRF_PAYLOAD_LEN];
    uint8_t status, fifo_status;
    uint8_t cmd = NRF_CMD_W_TX_PAYLOAD;
    io_tx_t tx[] = {
        {
            .mode = IO_TX_MODE_W,
            .off = 0,
            .len = 1,
            .buf = &cmd
        },
        {
            .mode = IO_TX_MODE_W,
            .off = 0,
            .len = sizeof(full_payload),
            .buf = full_payload
        }
    };

    memset(full_payload, 0, sizeof(full_payload));
    memcpy(full_payload, payload, MIN(payload_len, sizeof(full_payload)));

    // wait for transmission to be available
    do {
        status = nrf_reg_read(NRF_FIFO_STATUS_ADDR, &fifo_status);
        //gmd_delay(10);
        LOG_INFO(NRF, "status: %x fifo_status: %x", status, fifo_status);
    } while (0 == (fifo_status & NRF_FIFO_STATUS_TX_EMPTY_BMSK));

    // write payload
    io_spi_tx_begin(nrf.csn_pin);
    io_spi_master_sg(tx, ARR_SIZE(tx), 0);
    io_spi_tx_end();

    // start transmission
    io_pin_set(nrf.ce_pin);

    // wait for transmission to complete
    do {
        status = nrf_reg_read(NRF_FIFO_STATUS_ADDR, &fifo_status);
        //gmd_delay(10);
    } while (0 == (status & (NRF_STATUS_TX_DS_BMSK|NRF_STATUS_MAX_RT_BMSK)));

    LOG_INFO(NRF, "tx status: %02x", status);

    // reset status
    io_pin_clr(nrf.ce_pin);
    nrf_reg_write(NRF_STATUS_ADDR, NRF_STATUS_TX_DS_BMSK|NRF_STATUS_MAX_RT_BMSK);
    if (status & NRF_STATUS_MAX_RT_BMSK) {
        nrf_send_cmd(NRF_CMD_FLUSH_TX);
    }
}

void nrf_recv(uint8_t* payload, uint8_t* max_payload_len, uint8_t* pipe_idx)
{
    uint8_t full_payload[NRF_PAYLOAD_LEN];
    uint8_t status, fifo_status;
    uint8_t cmd = NRF_CMD_R_RX_PAYLOAD;
    io_tx_t tx[] = {
        {
            .mode = IO_TX_MODE_W,
            .off = 0,
            .len = 1,
            .buf = &cmd
        },
        {
            .mode = IO_TX_MODE_W | IO_TX_MODE_R,
            .off = 0,
            .len = sizeof(full_payload),
            .buf = full_payload
        }
    };

    memset(full_payload, NRF_CMD_NOP, sizeof(full_payload));


    // wait for packet to be available
    do {
        // wait for interrupt, activated on low
        gmd_wfe(nrf.irq_port, nrf.irq_mask, nrf.irq_mask, 0);

        status = nrf_reg_read(NRF_FIFO_STATUS_ADDR, &fifo_status);
        LOG_INFO(NRF, "rx status: %02x fifo: %02x", status, fifo_status);
        if (0 == (fifo_status & NRF_FIFO_STATUS_RX_EMPTY_BMSK)) {
            break;
        }
        //gmd_delay(100);

    } while (1); //(fifo_status & NRF_FIFO_STATUS_RX_EMPTY_BMSK);

    // read payload
    io_spi_tx_begin(nrf.csn_pin);
    io_spi_master_sg(tx, ARR_SIZE(tx), 0);
    io_spi_tx_end();

    *pipe_idx = (status & NRF_STATUS_RX_P_NO_BMSK) >> NRF_STATUS_RX_P_NO_SHFT;

    // write payload
    *max_payload_len = MIN(*max_payload_len, sizeof(full_payload));
    memcpy(payload, full_payload, *max_payload_len);

    // clear interrupt
    status = nrf_reg_write(NRF_STATUS_ADDR, NRF_STATUS_RX_DR_BMSK);

}
