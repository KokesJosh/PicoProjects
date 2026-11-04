#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define INPUT_GPIO 26
#define OUTPUT_GPIO 15
#define ADC_CHANNEL 0
#define THRESHOLD 3500
#define SLOPE ((float)(4095)) / (4095 - THRESHOLD)

int main(void){
	
	// Init our output GPIO with PWM
	gpio_init(OUTPUT_GPIO);
	gpio_set_function(OUTPUT_GPIO, GPIO_FUNC_PWM);
	
	uint8_t slice = pwm_gpio_to_slice_num(OUTPUT_GPIO);
	uint8_t channel = pwm_gpio_to_channel(OUTPUT_GPIO);
	
	// set our PWM preferences
	pwm_set_wrap(slice, 4095);
	pwm_set_clkdiv(slice, 150.0f);
	pwm_set_chan_level(slice, channel, 0);
	pwm_set_enabled(slice, true);
	
	// Create our ADC connection
	adc_init();
	adc_gpio_init(INPUT_GPIO);
	adc_select_input(ADC_CHANNEL);
	
	// continuously poll the analog reader for values, only turn light on past the threshold and then linearly brighten proportional to darkness
	while(true){
		uint16_t adc_value = adc_read();
		if(adc_value < THRESHOLD)
			pwm_set_chan_level(slice, channel, 0);
		else
			pwm_set_chan_level(slice, channel, (uint16_t)((((float) adc_value) - THRESHOLD) * SLOPE));
		sleep_ms(25);
	}
}