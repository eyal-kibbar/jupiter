#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "io.h"
#include "quad_protocol.h"
#include "quad.h"
#include "failsafe.h"
#include <string.h>

#define SS_PIN 8
#define CE_PIN 9

void setup()
{
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();
}

void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = CE_PIN,
        .irq_pin = 0xFF,
        .channel = 76
    };
    LOG_INFO(RADIO_RX, "init nrf");
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    //nrf_test();

    gmd_wfe(&quad.state, ~0, QUAD_INIT, 0); // wait for quad to be ready
    LOG_INFO(QUAD_RECV, "recv ready");
}




uint8_t last_cmd = 0;

void loop()
{
    radio_pkt_t pkt;
    quad_response_pkt_t resp_pkt;
    uint8_t payload_len = sizeof(pkt);
    uint8_t pipe_idx = 0;
    float setpoint[3];
    float throttel;
    int rc;

    failsafe_reset(1); // reset entire system if we lost communication

    memset(&resp_pkt, 0, sizeof resp_pkt);

    // TODO: set the respose packet


    //quad_get_servo_microseconds(resp_pkt.motor);
    resp_pkt.pid_cfg_kp = quad.pid_cfg.kp;
    resp_pkt.pid_cfg_ki = quad.pid_cfg.ki;
    resp_pkt.pid_cfg_kd = quad.pid_cfg.kd;
    nrf_send_pending(1, (uint8_t*)&resp_pkt, sizeof(resp_pkt));

    //LOG_INFO(RADIO_RX, "receiving");
    if ((rc = nrf_recv((uint8_t*)&pkt, &payload_len, &pipe_idx))) {
        quad_stop(); // stop the quad if we failed to reveice a packet
        return;
    }

    if (pkt.switches & 0x1) {
        quad_start();
    }
    else {
        quad_stop();
    }

    setpoint[0] = interp(pkt.j_right_x, 0, 1023, -20, +20);
    setpoint[1] = interp(pkt.j_right_y, 0, 1023, -20, +20);
    setpoint[2] = interp(pkt.j_left_x,  0, 1023, -180, +180);

    throttel = interp(pkt.j_left_y, 0, 1024, 0, 1);

    quad_set_setpoint(setpoint, throttel);

    if (last_cmd == pkt.cmd) {
        return;
    }
    last_cmd = pkt.cmd;
    switch (pkt.cmd) {

        case QUAD_CMD_NOP: break;
        case QUAD_CMD_ADD_P: quad.pid_cfg.kp += 0.05f; break;
        case QUAD_CMD_SUB_P: quad.pid_cfg.kp -= 0.05f; break;
        case QUAD_CMD_ADD_I: quad.pid_cfg.ki += 0.01f; break;
        case QUAD_CMD_SUB_I: quad.pid_cfg.ki -= 0.01f; break;
        case QUAD_CMD_ADD_D: quad.pid_cfg.kd += 0.1f; break;
        case QUAD_CMD_SUB_D: quad.pid_cfg.kd -= 0.1f; break;


    }
}


TASK(STACK_SIZE);
