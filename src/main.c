#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/i2c.h>


#include "remote.h"
#include "adc.h"

//#include <zephyr/net/mqtt.h> not today...

#define I2C_DEV_NAME DT_LABEL(DT_ALIAS(i2c0))

#define PCF8563_I2C_ADDR 0x51

#define BT_UUID_BAS BT_UUID_DECLARE_16(BT_UUID_BAS_VAL)


#define I2C_DEV I2C_DT_SPEC_GET_ON_I2C(i2c0)


#define LED0_NODE DT_ALIAS(led0)

#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios,
							      {0});
static uint16_t adc_val = 0;
static uint32_t led_state_counter = 0;


struct device *i2c_dev;

int  init_i2c(void)
{
    i2c_dev = device_get_binding("i2c0");
    if (!i2c_dev) {
        //printk("I2C: Device driver not found.\n");
        return 1;
    }
	return 0;
    //printk("I2C: Device driver found.\n");
}


int read_rtc(uint8_t reg, uint8_t *data, size_t len)
{
    struct i2c_msg msgs[2];

    msgs[0].buf = &reg;
    msgs[0].len = sizeof(reg);
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = data;
    msgs[1].len = len;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    return i2c_transfer(i2c_dev, &msgs[0], 2, PCF8563_I2C_ADDR);
}




// nrf_drv_i2s_config_t config = NRF_DRV_I2S_DEFAULT_CONFIG; 
int main(void)
{
	int ret = 0;
	ret += gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	//ret += initialize_adc();

    //ret += init_i2c();

	ret += bluetooth_init();

	if(ret > 0){

	//gpio_pin_toggle_dt(&led);

	} else {
		while(true) 
		{
			//adc_val = get_adc_val();
			gpio_pin_toggle_dt(&led);
			led_state_counter++;
			k_sleep(K_SECONDS(1));

		}

	}

    return 0;
}