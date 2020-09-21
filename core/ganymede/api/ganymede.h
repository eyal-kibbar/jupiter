#ifndef GANYMEDE_H_
#define GANYMEDE_H_

#define GMD_MAGIC 0xBEEF

#include <stdint.h>
#include <stddef.h>

typedef uint8_t gmd_i2c_dev_addr_t;

typedef struct gmd_io_tx_s {
    uint8_t  isw;
    uint8_t  len;
    uint8_t  off;
    uint8_t* buf;
} gmd_io_tx_t;

struct gmd_task_data_s {
    uint16_t magic;         /**<< magic number for identifying a task struct */
    uint16_t stack_size;    /**<< task stack size */
    void (*setup_func)();   /**<< setup function for internal chip state. interrupts and async io not available */
    void (*init_func)();    /**<< init function for external sensors that require io (spi/uart/i2c) initializing */
    void (*loop_func)();    /**<< loop function that will be called in a loop */
};

void setup() __attribute__((weak));
void init() __attribute__((weak));

#define TASK(_stack_size) \
    __attribute__((used, section("tasks"))) \
    union { uint8_t stack[_stack_size]; struct gmd_task_data_s task_data; } task = { \
        .task_data = { \
            .magic = GMD_MAGIC, \
            .stack_size = _stack_size, \
            .setup_func = setup, \
            .init_func = init, \
            .loop_func = loop, \
        } \
    }

void gmd_delay(uint16_t ms);

/**
 * wait until event changes
 * @return if timeout != 0, number of ms waited for event
 */
uint16_t gmd_wfe(volatile uint8_t* p_event, uint8_t mask, uint8_t event, uint16_t timeout_ms);

uint8_t  gmd_curtick();
uint16_t gmd_ticks2ms(uint8_t ticks, uint16_t* out_us);
uint16_t gmd_ms2ticks(uint16_t ms, uint16_t* out_us);

void gmd_panic();

#endif /* GANYMEDE_H_ */
