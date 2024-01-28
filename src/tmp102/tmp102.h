#ifndef TMP102_H
#define TMP102_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include "i2c.h"

#define TMP102_ADDR 0x48
#define TEMP_REG 0x00
#define CONFIG_REG 0x01
#define T_LOW_REG 0x02
#define T_HIGH_REG 0x03

int tmp102_init(void);
void tmp102_wakeup(void);
uint32_t tmp102_readTempC(uint8_t *buf);
uint8_t tmp102_readRegister(bool registerNumber, uint8_t reg);



#endif // TMP102_H
