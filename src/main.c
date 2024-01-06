#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

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


int main(void)
{
	int ret = 0;
	ret += gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	//ret += initialize_adc();


	ret += bluetooth_init();

	while(true) 
	{
		//adc_val = get_adc_val();
		gpio_pin_toggle_dt(&led);
		led_state_counter++;
		k_sleep(K_SECONDS(1));

	}

    return 0;
}