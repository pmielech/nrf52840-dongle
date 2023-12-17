#include "adc.h"

// #define ADC_NODE DT_ALIAS(sw0)

//const struct device *adc_dev = device_get_binding(DT_LABEL(DT_INST(0, nordic_nrf_saadc)));
//struct device *adc_dev = device_get_binding(ADC_NODE);

//const struct device *adc_dev = device_get_binding(ADC_NODE);



#define ADC_DEVICE_NAME DT_ADC_0_NAME
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_6
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)
#define ADC_1ST_CHANNEL_ID 0
#define ADC_1ST_CHANNEL_INPUT NRF_SAADC_INPUT_AIN0
#define ADC_2ND_CHANNEL_ID 2
#define ADC_2ND_CHANNEL_INPUT NRF_SAADC_INPUT_AIN2
#define BUFFER_SIZE 8


static const struct adc_channel_cfg m_1st_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_1ST_CHANNEL_ID,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive = ADC_1ST_CHANNEL_INPUT,
#endif
};

static int16_t m_sample_buffer[BUFFER_SIZE];
uint16_t adc_single_value;

const struct adc_sequence_options sequence_opts = {
	.interval_us = 0,
	.callback = NULL,
	.user_data = NULL,
	.extra_samplings = 7,
};


const struct adc_sequence adc_seq = {
		.channels = BIT(ADC_1ST_CHANNEL_ID),
		.buffer = &adc_single_value,
		.buffer_size = sizeof(adc_single_value),
		.resolution = ADC_RESOLUTION,
	};

const struct device *adc_dev;

// static uint16_t adc_value;

// struct adc_channel_cfg adc_cfg_channel = {
//         .gain  = ADC_GAIN_1,
//         .reference = ADC_REF_INTERNAL,
//         .acquisition_time = ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40),
//         .input_positive = SAADC_CH_PSELP_PSELP_AnalogInput0, // Dostosuj do używanego pinu ADC
//     };

//     struct adc_sequence adc_seq = {
//         .channels    = BIT(0),
//         .buffer      = &adc_value,
//         .buffer_size = sizeof(adc_value),
//         .resolution  = 12,
//     };


static int adc_sample(void)
{
	int ret;

	ret = adc_read(adc_dev, &adc_seq);
	printk("ADC read err: %d\n", ret);

	/* Print the AIN0 values */
	printk("ADC raw value: ");
	for (int i = 0; i < BUFFER_SIZE; i++) {
		printk("%d ", m_sample_buffer[i]);
	}
	
	printf("\n Measured voltage: ");
	for (int i = 0; i < BUFFER_SIZE; i++) {
		float adc_voltage = 0;
		adc_voltage = (float)(((float)m_sample_buffer[i] / 1023.0f) *
				      3600.0f);
		printk("%f ",adc_voltage);
	}
	printk("\n");


	return ret;
}

uint16_t get_adc_val(void){
    return adc_read(adc_dev, &adc_seq);
}

int initialize_adc(void){
    int ret = 0;


    adc_dev = device_get_binding("ADC_0");
	ret += adc_channel_setup(adc_dev, &m_1st_channel_cfg);

	//NRF_SAADC_S->TASKS_CALIBRATEOFFSET = 1;


	return ret;
}
