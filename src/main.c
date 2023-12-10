#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

//#define LED_PORT DT_LABEL(DT_ALIAS(led1))
// #define LED_PIN DT_GPIO_PIN(DT_ALIAS(led1), gpios)
// #define LED_PORT DT_GPIO_LABEL(DT_NODELABEL(gpio0), gpios)
// #define LED_PIN 6
// #define LED_PORT "GPIO_0"
// #define LED_PIN 6

#define LED0_NODE DT_ALIAS(led0)

#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios,
							      {0});

static struct gpio_callback button_cb_data;


// static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
// 						     {0});


// static bool led_is_on = true;
// void button_pressed(const struct device *dev, struct gpio_callback *cb,
// 		    uint32_t pins)
// {
// 	// printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
// 	// gpio_pin_set_dt(&led, (int)led_is_on);
// 	led_is_on = !led_is_on;
// }

void main(void)
{
	int ret;
	// const struct device *cons = device_get_binding(CONSOLE_LABEL);

	// if (!device_is_ready(button.port)) {
	// 	printk("Error: button device %s is not ready\n",
	// 	       button.port->name);
	// 	return;
	// }

        // ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	// if (ret != 0) {
	// 	//printk("Error %d: failed to configure %s pin %d\n",
	// 	       ret, button.port->name, button.pin;
        //                //);
	// 	return;
	// }

	gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	// if (ret != 0) {
	// 	//printk("Error %d: failed to configure %s pin %d\n",
	// 	       ret, button.port->name, button.pin;
                       
        //                //);
	// 	return;
	// }

	// ret = gpio_pin_interrupt_configure_dt(&button,
	// 				      GPIO_INT_LEVEL_LOW);
	// if (ret != 0) {
	// 	//printk("Error %d: failed to configure interrupt on %s pin %d\n",
	// 		ret, button.port->name, button.pin;
        //                 //);
	// 	return;
	// }

        while(true) 
        {
	        //gpio_pin_toggle_dt(&led);
                gpio_pin_set_dt(&led, GPIO_ACTIVE_HIGH);
		k_sleep(K_SECONDS(5));
                gpio_pin_set_dt(&led, GPIO_ACTIVE_LOW);
		k_sleep(K_SECONDS(5));
	}
	// gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	// gpio_add_callback(button.port, &button_cb_data);
	// printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	// if (led.port && !device_is_ready(led.port)) {
	// 	printk("Error %d: LED device %s is not ready; ignoring it\n",
	// 	       ret, led.port->name);
	// 	led.port = NULL;
	// }
	// if (led.port) {
	// 	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	// 	if (ret != 0) {
	// 		printk("Error %d: failed to configure LED device %s pin %d\n",
	// 		       ret, led.port->name, led.pin);
	// 		led.port = NULL;
	// 	} else {
	// 		printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
	// 	}
	// }

	// pm_device_state_set(cons, PM_DEVICE_STATE_SUSPENDED);


	// for (ret = 0; ret < 5; ret++) {
	// 	led_is_on = !led_is_on;
	// 	k_sleep(K_SECONDS(1));
	// }
}