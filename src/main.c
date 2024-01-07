
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
// #include <zephyr/drivers/rtc.h>

#include "i2c.h"
#include "common.h"
#include "remote.h"
#include "adc.h"

//#include <zephyr/net/mqtt.h> not today...


#define BT_UUID_BAS BT_UUID_DECLARE_16(BT_UUID_BAS_VAL)

#define LED0_NODE DT_ALIAS(led0)

#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)


static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios,
							      {0});
static uint16_t adc_val = 0;
static uint32_t led_state_counter = 0;


// const struct device *pcf = DEVICE_DT_GET_ANY();



int main(void)
{
	int ret = 0;
	ret += gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	//ret += initialize_adc();

    ret += init_i2c();

	ret += bluetooth_init();

	if(ret > 0){

	//gpio_pin_toggle_dt(&led);

	} else {

		//PCF8563_Init();
		//PCF8563_Set_Time(23,59,58);//Time
		// PCF8563_Set_Alarm(0,0);//Alarm
		// PCF8563_Set_Days(2021,9,28);//Days
		// PCF8563_Set_Timer(1,10);//Timer 10S


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