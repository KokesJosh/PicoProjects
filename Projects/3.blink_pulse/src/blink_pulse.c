#include<stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/platform.h"
/*
enum gpio_function_rp2350 { GPIO_FUNC_HSTX = 0, GPIO_FUNC_SPI = 1, 
GPIO_FUNC_UART = 2, GPIO_FUNC_I2C = 3, GPIO_FUNC_PWM = 4, GPIO_FUNC_SIO = 5, 
GPIO_FUNC_PIO0 = 6, GPIO_FUNC_PIO1 = 7, GPIO_FUNC_PIO2 = 8, GPIO_FUNC_GPCK = 9, 
GPIO_FUNC_XIP_CS1 = 9, GPIO_FUNC_CORESIGHT_TRACE = 9, GPIO_FUNC_USB = 10, GPIO_FUNC_UART_AUX = 11, GPIO_FUNC_NULL = 0x1f }
*/

int main(void){
	
	for(int GPIO_PIN = 0; GPIO_PIN <= 4; GPIO_PIN +=2){
		// Set our GPIO function to use the Pulse-Width-Module
		gpio_set_function(GPIO_PIN, GPIO_FUNC_PWM);
		
		uint slice = pwm_gpio_to_slice_num(GPIO_PIN);
		uint channel = pwm_gpio_to_channel(GPIO_PIN);
		
		pwm_set_wrap(slice, 999); // wrap counter after reaching counts, 1 period = 100 cts
		pwm_set_clkdiv(slice, 100.0f); // tell the pwm how quick to increment the counter
		/* pwm clock speed is 150 mhz
		150,000,000 hz / 100 hz = 1,500,000hz. 
		Therefore counter will increment one million five hundred-thousand times per second
		*/
		pwm_set_chan_level(slice, channel, 0);
		//this sets the counter to be 25% high, 75% low
		// Assuming the pins are 3.3V on high, they avg voltage becones 0.825V
		pwm_set_enabled(slice, true);
	}
	
	while(true){
		for (int GPIO_PIN = 0; GPIO_PIN <= 4; GPIO_PIN += 2){
			uint slice = pwm_gpio_to_slice_num(GPIO_PIN);
			uint channel = pwm_gpio_to_channel(GPIO_PIN);
			
			for(int LEVEL = 0; LEVEL < 999; LEVEL++){
				pwm_set_chan_level(slice, channel, LEVEL);
				sleep_ms(2);
			}
			pwm_set_chan_level(slice, channel, 0);
		}
	}
	
	return 0;
}