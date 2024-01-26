
#include "tmp102.h"



struct device *tmp102_dev;



int tmp102_init(void)
{
    I2C_writeByte_u(0x0, TEMP_REG, TMP102_ADDR);
    return 0;
}


uint8_t tmp102_readRegister(bool registerNumber, uint8_t reg)
{
  uint8_t registerByte[2];
  I2C_readBytes_u(registerByte, 2, reg, TMP102_ADDR);
  return registerByte[registerNumber];
}

void tmp102_wakeup(void)
{
  uint8_t registerByte; 
  registerByte = readRegister(0, CONFIG_REG);
  registerByte &= 0xFE; // Clear SD (bit 0 of first byte)
  I2C_writeByte_u(registerByte, CONFIG_REG, TMP102_ADDR);
}


uint32_t tmp102_readTempC(uint8_t *buf)
{
    uint8_t registerByte[2]; 
    int16_t digitalTemp;     

    I2C_readBytes_u(registerByte, 2, TEMP_REG, TMP102_ADDR);

    *buf = registerByte[0];
    *(buf+1) = registerByte[1];


    if (registerByte[0] == 0xFF && registerByte[1] == 0xFF)
    {
        return 0xFFFFFFFF; // Sensor disconnected
    }

    // Bit 0 of second byte will always be 0 in 12-bit readings and 1 in 13-bit
    if (registerByte[1] & 0x01) // 13 bit mode
    {
        // Combine bytes to create a signed int
        digitalTemp = ((registerByte[0]) << 5) | (registerByte[1] >> 3);
        // Temperature data can be + or -, if it should be negative,
        // convert 13 bit to 16 bit and use the 2s compliment.
        if (digitalTemp > 0xFFF)
        {
            digitalTemp |= 0xE000;
        }
    }
    else // 12 bit mode
    {
        // Combine bytes to create a signed int
        digitalTemp = ((registerByte[0]) << 4) | (registerByte[1] >> 4);
        // Temperature data can be + or -, if it should be negative,
        // convert 12 bit to 16 bit and use the 2s compliment.
        if (digitalTemp > 0x7FF)
        {
            digitalTemp |= 0xF000;
        }
    }

    return 0;
}
