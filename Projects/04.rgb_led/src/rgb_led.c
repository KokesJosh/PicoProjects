#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

int main(void){
	for(uint GPIO_PIN = 0; GPIO_PIN <= 4; GPIO_PIN += 2){
		
		//init our GPIO to use the PWM
		gpio_set_function(GPIO_PIN, GPIO_FUNC_PWM);
		
		//get the slice and channel # for our GPIO pins
		uint slice = pwm_gpio_to_slice_num(GPIO_PIN);
		uint channel = pwm_gpio_to_channel(GPIO_PIN);
		
		// Determine our PWM freqency
		pwm_set_wrap(slice, 999);
		pwm_set_clkdiv(slice, 150.0f);
		pwm_set_chan_level(slice, channel, 999);
		
		// start the PWM
		pwm_set_enabled(slice, true);
	}
	
	float x = 0.0f;
	while(true){
		float period = 360.0f;
		// each value is a trapezoidal wave function with a period of 360 degrees, offset each one by 120 degrees from last one'
		// to cover full color whell
		float rvalue = fmin(fabs(fmod(x, period) / (period / 2) - 1.0f), 0.5f);
		float gvalue = fmin(fabs(fmod(x + 120, period) / (period / 2) - 1.0f), 0.5f);
		float bvalue = fmin(fabs(fmod(x + 240, period) / (period / 2) - 1.0f), 0.5f);
		
		// scale our trapezoidal values to the channel level values
		pwm_set_chan_level(0, 0, (uint)(rvalue * 999 * 2));
		pwm_set_chan_level(1, 0, (uint)(gvalue * 999 * 2));
		pwm_set_chan_level(2, 0, (uint)(bvalue * 999 * 2));
		
		// increase our counter and wrap it to prevent overflow
		x++;
		x = fmod(x, period);
		sleep_ms(10);
	}
	
	/* RESULT: A rainbow LED that should cover the full HUE wheel */
	return 0;
}