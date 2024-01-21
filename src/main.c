
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>

// #include <zephyr/drivers/rtc.h>

#include "PCF8563.h"
#include "common.h"
#include "remote.h"
#include "adc.h"

//#include <zephyr/net/mqtt.h> not today...

#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

#define GPIO_INT_NODE DT_NODELABEL(butt1)
#define GPIO_INT_PORT DT_ALIAS_BUTT1_GPIOS_CONTROLLER
#define GPIO_INT_NODE_PIN     DT_GPIO_PIN(GPIO_INT_NODE, gpios)

#define BT_UUID_BAS BT_UUID_DECLARE_16(BT_UUID_BAS_VAL)

#define LED0_NODE DT_ALIAS(led0)
#define GPIO_SW0_NODE DT_ALIAS(sw0)

#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button_cb_data;

static struct gpio_callback gpio_cb;

static const struct gpio_dt_spec gpio_int_dev = GPIO_DT_SPEC_GET(GPIO_INT_NODE, gpios);


static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios,
							      {0});
static uint16_t adc_val = 0;
static uint32_t led_state_counter = 0;
static uint16_t sys_cnt = 0;


void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	for(int i = 0; i< 10; i++){
		gpio_pin_toggle_dt(&led);
		k_sleep(K_SECONDS(1));
	}



}

void gpio_callback(const struct device *dev, struct gpio_callback *cb,
                   uint32_t pins)
{

	gpio_pin_set(led.port, led.pin, GPIO_ACTIVE_HIGH);
    // for(int i = 0; i < 5; i++){
	// 	gpio_pin_toggle_dt(&led
	// }
    
}

int configure_int_gpio(void)
{
	int ret = 0;
	ret += gpio_pin_configure_dt(&gpio_int_dev, 
                       GPIO_INPUT);
    ret +=  gpio_pin_interrupt_configure_dt(&gpio_int_dev, GPIO_INT_EDGE_TO_INACTIVE);
	gpio_init_callback(&gpio_cb, gpio_callback, BIT(gpio_int_dev.pin));
    ret +=  gpio_add_callback(gpio_int_dev.port,  &gpio_cb);
	return ret;
}

int main(void)
{
	int ret = 0;
	ret += gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	//ret += initialize_adc();

    ret += init_i2c();

	ret += bluetooth_init();
	update_system_status(ret, 1);


	ret += gpio_pin_configure_dt(&button, GPIO_INPUT);
	ret += gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	ret += configure_int_gpio();

	if(ret > 0){

	//gpio_pin_toggle_dt(&led);
	while(true){


	}

	} else {

		
		PCF8563_Init();
		// PCF8563_Cleare_TF_Flag();
    	// configure_gpio();

		while(true) 
		{

			gpio_pin_toggle_dt(&led);
			update_system_status(sys_cnt, 0);

			k_sleep(K_SECONDS(5));

			if(sys_cnt % 5 == 0){
			}

			if(sys_cnt >= 65534){
				sys_cnt = 0;	
			} else {
				sys_cnt++;
			}
		}

	}

    return 0;
}