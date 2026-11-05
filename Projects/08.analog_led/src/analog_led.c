#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define INPUT_GPIO 26
#define OUTPUT_GPIO 15
int main(void){
	// Init our output GPIO to use the PWM
	gpio_set_function(OUTPUT_GPIO, GPIO_FUNC_PWM);
	
	uint8_t slice = pwm_gpio_to_slice_num(OUTPUT_GPIO);
	uint8_t channel = pwm_gpio_to_channel(OUTPUT_GPIO);
	
	// Set our PWM counter speed, wrap value, and high to low threshold
	pwm_set_clkdiv(slice, 150.0f);
	pwm_set_wrap(slice, 999);
	pwm_set_chan_level(slice, channel, 0);
	pwm_set_enabled(slice, true);
	
	// Enable the adc component
	adc_init();
	adc_gpio_init(INPUT_GPIO);
	
	
	while(true){
		// Take our measured value and covert it to a pwm chan amount
		uint16_t measure = adc_read();
		float chan_value = (((float)measure) / 4095.0f) * 999;
		
		// Set or pwm frequency to % of 3.3v passed through
		pwm_set_chan_level(slice, channel, chan_value);
		sleep_ms(10);
	}
	return 0;
}