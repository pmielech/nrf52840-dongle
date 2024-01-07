#include "i2c.h"

static const struct device *i2c0_device = DEVICE_DT_GET(I2C0_NODE);

static uint8_t i2c_buffer[2];

int init_i2c(void)
{
    if(!device_is_ready(i2c0_device)){
		return 1;
	}

	return 0;
}

int I2C_write_u(const uint8_t * buf, uint8_t reg, uint32_t num_bytes, uint16_t addr){

    i2c_buffer[0] = reg;

    return i2c_write(i2c0_device, i2c_buffer, num_bytes, addr);

}

int I2C_read_u(const uint8_t * buf, uint8_t reg, uint32_t num_bytes, uint16_t addr){

    return i2c_read(i2c0_device, buf, num_bytes, addr);

}


// int read_rtc(uint8_t reg, uint8_t *data, size_t len)
// {
//     struct i2c_msg msgs[2];

//     msgs[0].buf = &reg;
//     msgs[0].len = sizeof(reg);
//     msgs[0].flags = I2C_MSG_WRITE;

//     msgs[1].buf = data;
//     msgs[1].len = len;
//     msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;
//     return i2c_transfer(i2c0_device, &msgs[0], 2, PCF8563_I2C_ADDR);
// }

