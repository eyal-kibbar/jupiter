#ifndef HWIO_H_
#define HWIO_H_

#include <stdint.h>

void regwrite(uint8_t slave_addr, uint8_t addr, void* buf, uint8_t n);
void regread(uint8_t slave_addr, uint8_t addr, void* buf, uint8_t n);


void regwrite8(uint8_t slave_addr, uint8_t addr, uint8_t val);
uint8_t regread8(uint8_t slave_addr, uint8_t addr);

void regwrite16(uint8_t slave_addr, uint8_t addr, uint16_t val);
uint16_t regread16(uint8_t slave_addr, uint8_t addr);

#endif /* HWIO_H_ */