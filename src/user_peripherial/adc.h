#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <hal/nrf_saadc.h>


uint16_t get_adc_val(void);
int initialize_adc(void);


