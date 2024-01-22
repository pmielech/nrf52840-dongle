
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

// #include <zephyr/drivers/rtc.h>

#include "PCF8563.h"
#include "common.h"
#include "remote.h"
#include "adc.h"

//#include <zephyr/net/mqtt.h> not today...

#define BT_UUID_BAS BT_UUID_DECLARE_16(BT_UUID_BAS_VAL)

#define LED0_NODE DT_ALIAS(led0)
#define GPIO_SW0_NODE DT_ALIAS(sw0)


#define SW0_NODE    DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

#define GPIO_INTER_NODE    DT_NODELABEL(butt1)
#if !DT_NODE_HAS_STATUS(GPIO_INTER_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif


static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
                                  {0});
static struct gpio_callback button_cb_data;


static struct gpio_callback rtc_int_cb;

static const struct gpio_dt_spec rtc_int = GPIO_DT_SPEC_GET_OR(GPIO_INTER_NODE, gpios,
                                  {0});

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios,
                             {0});

static uint16_t sys_cnt = 0;


uint8_t PROGRAM_SESSION = 0;



void gpio_rtc_callback(const struct device *dev, struct gpio_callback *cb,
                   uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
    PROGRAM_SESSION = 1;

}


void button_pressed(const struct device *dev, struct gpio_callback *cb,
            uint32_t pins)
{
	PROGRAM_SESSION = 1u;
    gpio_pin_toggle_dt(&led);
	sys_reboot();
	//k_msleep(SLEEP_TIME_MS);

    
    // printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int gpio_init(){
	int ret = 0;

 	ret += gpio_pin_configure_dt(&button, GPIO_INPUT);


    ret += gpio_pin_interrupt_configure_dt(&button,
                          GPIO_INT_EDGE_TO_ACTIVE);

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);


    ret += gpio_pin_configure_dt(&rtc_int, GPIO_INPUT);


    ret += gpio_pin_interrupt_configure_dt(&rtc_int,
                          GPIO_INT_EDGE_TO_INACTIVE);

    gpio_init_callback(&rtc_int_cb, gpio_rtc_callback, BIT(rtc_int.pin));
    gpio_add_callback(rtc_int.port, &rtc_int_cb);

    if (led.port) {
        ret += gpio_pin_configure_dt(&led, GPIO_OUTPUT | GPIO_OUTPUT_INACTIVE);
    }

	return ret;
}

void deep_sleep(void)
{
    k_cpu_idle();
}



int main(void)
{
	int ret = 0;

	ret += gpio_init();

   

	if(ret > 0){

	while(true){
		sys_reboot();
	}

	} else {

		
		//PCF8563_Init();


		while(true) 
		{
			
			deep_sleep();

			if(PROGRAM_SESSION == 1u){
				ret += init_i2c();
				ret += bluetooth_init();
				while (true)
				{
					k_msleep(10);
				}
			}


			update_system_status(sys_cnt, 0);


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