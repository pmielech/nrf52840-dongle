
#include "../common.h"
#include <zephyr/drivers/i2c.h>

#define I2C0_NODE DT_NODELABEL(i2c0)



int init_i2c(void);
uint8_t I2C_writeByte_u(uint8_t value, uint8_t reg, uint16_t addr);
uint8_t I2C_readByte_u(uint8_t reg, uint16_t addr);
uint8_t I2C_readBytes_u(uint8_t *buf, uint8_t bytes_n, uint8_t reg, uint16_t addr);

