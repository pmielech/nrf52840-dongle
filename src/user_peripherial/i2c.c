#include "i2c.h"
#include "remote.h"


static const struct device *i2c0_device = DEVICE_DT_GET(I2C0_NODE);

static uint8_t i2c_buffer[2];




int init_i2c(void)
{
    if(!device_is_ready(i2c0_device)){
		return 1;
	}

	return 0;
}

uint8_t I2C_readBytes_u(uint8_t *buf, uint8_t bytes_n, uint8_t reg, uint16_t addr){

    i2c_read(i2c0_device, buf, bytes_n, addr);

    return 0;
}


uint8_t I2C_writeByte_u(uint8_t value, uint8_t reg, uint16_t addr){

    return i2c_reg_write_byte(i2c0_device, addr, reg, value);
    
}

uint8_t I2C_readByte_u(uint8_t reg, uint16_t addr){

    uint8_t value = 0;
    if(i2c_reg_read_byte(i2c0_device, addr, reg, &value) != 0){
    }
    return value;

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

