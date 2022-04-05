#ifndef AIRPLANE_PROTOCOL_H_
#define AIRPLANE_PROTOCOL_H_



#include "utils.h"

struct radio_pkt_s {
    uint16_t pot_left;
    uint16_t pot_right;
    uint16_t j_right_x;
    uint16_t j_right_y;
    uint16_t j_left_x;
    uint16_t j_left_y;
    uint8_t buttons;
    uint8_t switches;
} __attribute__((packed));

typedef struct radio_pkt_s radio_pkt_t;

COMPILER_CHECK(sizeof(radio_pkt_t) <= 32);


struct airplane_response_pkt_s {
    float angles[3];
    int16_t motor[4];
} __attribute__((packed));

typedef struct airplane_response_pkt_s airplane_response_pkt_t;

typedef struct quad_response_pkt_s quad_response_pkt_t;

COMPILER_CHECK(sizeof(radio_pkt_t) <= 32);
COMPILER_CHECK(sizeof(airplane_response_pkt_t) <= 32);


#endif /* AIRPLANE_PROTOCOL_H_ */
