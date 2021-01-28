#include "ganymede.h"
#include "nRF24L01.h"
#include "logging.h"
#include "radio.h"
#include "io.h"
#include "servo.h"
#include <avr/eeprom.h>


#define SS_PIN 2
#define EEPROM_CALIB_ADDR ((void*)0x10)

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2000


#define CALIB_MODE_RIGHT_J 0x1
#define CALIB_MODE_LEFT_J  0x2


radio_pkt_t pkt;
uint8_t calib_mode;


struct radio_rx_calib_s {
    int16_t j_right_x_bais;
    int16_t j_right_y_bais;
    int16_t j_left_x_bais;
    int16_t j_left_y_bais;
};

struct radio_rx_calib_s calib;

void setup()
{
    io_uart_init(0, 9600);
    io_logging_init();
    io_pin_output(SS_PIN);
    io_pin_set(SS_PIN);
    io_spi_master_init();

    servo_init();

}

static void calib_read()
{
    while (!eeprom_is_ready()) {
        gmd_delay(10);
    };
    eeprom_read_block(&calib, EEPROM_CALIB_ADDR, sizeof(calib));
}

static void calib_write()
{
    while (!eeprom_is_ready()) {
        gmd_delay(10);
    };
    eeprom_write_block(&calib, EEPROM_CALIB_ADDR, sizeof(calib));
    gmd_delay(1000);
}

void init()
{
    nrf_cfg_t cfg = {
        .csn_pin = SS_PIN,
        .ce_pin = 3,
        .irq_pin = 0xFF, // irq disconnected
        .channel = 76
    };
    LOG_INFO(RADIO_RX, "init nrf");
    nrf_init(&cfg);

    nrf_recv_open_pipe(1, "abcde", 5);
    nrf_recv_set();
    nrf_test();

    servo_attach(4);
    servo_attach(5);
    servo_attach(6);
    servo_attach(7);

    calib_read();
}

int16_t map(uint32_t v, uint16_t vmin, uint16_t vmax, uint16_t omin, uint16_t omax)
{
    return (int16_t)(((v * (omax - omin)) / (vmax - vmin)) + omin);
}

uint16_t get_servo_us(int16_t v, int16_t c)
{
    const uint16_t orig_center = (SERVO_MIN_US + SERVO_MAX_US) / 2;
    uint16_t new_center = orig_center + c;

    v = map(v, 0, 1023, SERVO_MIN_US, SERVO_MAX_US);


    v += c;

    if (c < 0) {
        v = MAX(v, SERVO_MIN_US);
        v = MIN(v, 2*new_center - SERVO_MIN_US);
    }
    else if (c >= 0) {
        v = MAX(v, 2*new_center - SERVO_MAX_US);
        v = MIN(v, SERVO_MAX_US);
    }



    return v;
}

uint16_t get_pot_us(uint8_t pin)
{

}

void loop()
{
    uint8_t payload_len = sizeof(pkt);
    uint8_t pipe_idx = 0;
    uint16_t j_right_x_us;
    uint16_t j_right_y_us;
    uint16_t j_left_x_us;
    uint16_t j_left_y_us;

    //LOG_INFO(RADIO_RX, "receiving");
    nrf_recv((uint8_t*)&pkt, &payload_len, &pipe_idx);


    if ( !(CALIB_MODE_RIGHT_J & calib_mode) && (0x1 & pkt.switches)) {
        calib_mode |= CALIB_MODE_RIGHT_J;
        calib.j_right_x_bais = get_servo_us(pkt.j_right_x, 0);
        calib.j_right_y_bais = get_servo_us(pkt.j_right_y, 0);
        LOG_INFO(RADIO_RX, "entering right calibration mode: %d %d", calib.j_right_x_bais, calib.j_right_y_bais);
    }
    else if ((CALIB_MODE_RIGHT_J & calib_mode) && !(0x1 & pkt.switches)) {
        calib_mode &= ~CALIB_MODE_RIGHT_J;
        calib.j_right_x_bais -= get_servo_us(pkt.j_right_x, 0);
        calib.j_right_y_bais -= get_servo_us(pkt.j_right_y, 0);
        LOG_INFO(RADIO_RX, "exiting right calibration mode: %d %d", calib.j_right_x_bais, calib.j_right_y_bais);
    }

    if ( !(CALIB_MODE_LEFT_J & calib_mode) && (0x2 & pkt.switches)) {
        calib_mode |= CALIB_MODE_LEFT_J;
        calib.j_left_x_bais = get_servo_us(pkt.j_left_x, 0);
        calib.j_left_y_bais = get_servo_us(pkt.j_left_y, 0);
        LOG_INFO(RADIO_RX, "entering left calibration mode: %d %d", calib.j_right_x_bais, calib.j_right_y_bais);
    }
    else if ((CALIB_MODE_LEFT_J & calib_mode) && !(0x2 & pkt.switches)) {
        calib_mode &= ~CALIB_MODE_LEFT_J;
        calib.j_left_x_bais -= get_servo_us(pkt.j_left_x, 0);
        calib.j_left_y_bais -= get_servo_us(pkt.j_left_y, 0);
        LOG_INFO(RADIO_RX, "exiting left calibration mode: %d %d", calib.j_right_x_bais, calib.j_right_y_bais);
    }

    if ((0 == calib_mode) && !(0x1 & pkt.buttons)) {
        LOG_INFO(RADIO_RX, "saving calib");
        calib_write();
        LOG_INFO(RADIO_RX, "done");
    }

    /*
    LOG_INFO(RADIO_RX, "got: %04dx%04d %04dx%04d, %02x pipe %d",
        pkt.j_left_x, pkt.j_left_y,
        pkt.j_right_x, pkt.j_right_y,
        pkt.switches,
        pipe_idx);

        */
    j_right_x_us = get_servo_us(pkt.j_right_x, calib.j_right_x_bais);
    j_right_y_us = get_servo_us(pkt.j_right_y, calib.j_right_y_bais);
    j_left_x_us = get_servo_us(pkt.j_left_x, calib.j_left_x_bais);
    j_left_y_us = get_servo_us(pkt.j_left_y, calib.j_left_y_bais);


    /*
    LOG_INFO(RADIO_RX, "%04d(%4d) x %04d(%4d)   %04d(%4d) x %04d(%4d) %04dx%04d %02x %02x",
        j_right_x_us, pkt.j_right_x,
        j_right_y_us, pkt.j_right_y,
        map(pkt.j_left_x, 0, 1024, 500, 2000), pkt.j_left_x,
        map(pkt.j_left_y, 0, 1024, 500, 2000), pkt.j_left_y,
        pkt.pot_left, pkt.pot_right, pkt.buttons, pkt.switches);
    */



    if (0 == (CALIB_MODE_RIGHT_J & calib_mode)) {
        LOG_INFO(RADIO_RX, "right: 4d %4d %4d %x", j_right_x_us, calib.j_right_x_bais, pkt.j_right_x, pkt.buttons);
        servo_set_mircoseconds(4, j_right_x_us);
        servo_set_mircoseconds(5, j_right_y_us);
    }

    if (0 == (CALIB_MODE_LEFT_J & calib_mode)) {
        //LOG_INFO(RADIO_RX, "left: %4d %4d %4d %x", j_left_x_us, calib.j_left_x_bais, pkt.j_left_x, pkt.buttons);
        servo_set_mircoseconds(6, j_left_x_us);
        servo_set_mircoseconds(7, j_left_y_us);
    }

}


TASK(STACK_SIZE);
