#ifndef QUAD_PROTOCOL_H_
#define QUAD_PROTOCOL_H_



#include "utils.h"


enum {
    QUAD_CMD_NOP,
    QUAD_CMD_ADD_P,
    QUAD_CMD_SUB_P,
    QUAD_CMD_ADD_I,
    QUAD_CMD_SUB_I,
    QUAD_CMD_ADD_D,
    QUAD_CMD_SUB_D,
};

struct radio_pkt_s {
    uint16_t pot_left;
    uint16_t pot_right;
    uint16_t j_right_x;
    uint16_t j_right_y;
    uint16_t j_left_x;
    uint16_t j_left_y;
    uint8_t buttons;
    uint8_t switches;
    uint8_t cmd;
} __attribute__((packed));

typedef struct radio_pkt_s radio_pkt_t;

COMPILER_CHECK(sizeof(radio_pkt_t) <= 32);

struct quad_response_pkt_s {
    float angles[3];
    float dt;
    //int16_t motor[4];
    float pid_cfg_kp;
    float pid_cfg_ki;
    float pid_cfg_kd;
} __attribute__((packed));

typedef struct quad_response_pkt_s quad_response_pkt_t;

COMPILER_CHECK(sizeof(radio_pkt_t) <= 32);
COMPILER_CHECK(sizeof(quad_response_pkt_t) <= 32);


#endif /* QUAD_PROTOCOL_H_ */
