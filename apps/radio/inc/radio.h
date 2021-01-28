#ifndef RADIO_H_
#define RADIO_H_

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

#endif /* RADIO_H_ */
