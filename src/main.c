#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>


#include "remote.h"


//#include <zephyr/net/mqtt.h> not today...


#define BT_UUID_BAS BT_UUID_DECLARE_16(BT_UUID_BAS_VAL)


#define LED0_NODE DT_ALIAS(led0)

#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)


static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios,
							      {0});



int main(void)
{
	int ret = 0;
	ret += gpio_pin_configure_dt(&led, GPIO_OUTPUT);


	ret += bluetooth_init();


	while(true) 
	{
		gpio_pin_toggle_dt(&led);
		k_sleep(K_SECONDS(1));
		// gpio_pin_set_dt(&led, GPIO_ACTIVE_HIGH);
		// gpio_pin_set_dt(&led, GPIO_ACTIVE_LOW);
		// k_sleep(K_SECONDS(1));
	}

    return 0;
}