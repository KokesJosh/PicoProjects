#include "pico/stdlib.h"


/*
enum gpio_function_rp2350 { GPIO_FUNC_HSTX = 0, GPIO_FUNC_SPI = 1, 
GPIO_FUNC_UART = 2, GPIO_FUNC_I2C = 3, GPIO_FUNC_PWM = 4, GPIO_FUNC_SIO = 5, 
GPIO_FUNC_PIO0 = 6, GPIO_FUNC_PIO1 = 7, GPIO_FUNC_PIO2 = 8, GPIO_FUNC_GPCK = 9, 
GPIO_FUNC_XIP_CS1 = 9, GPIO_FUNC_CORESIGHT_TRACE = 9, GPIO_FUNC_USB = 10, GPIO_FUNC_UART_AUX = 11, GPIO_FUNC_NULL = 0x1f }
*/

int main(void){
	// Initilize gpio 26-28 and set their direction to out
	for(int i = 26; i<=28; i++){ 
		gpio_init(i);
		gpio_set_dir(i, true); 
	}
	
	while(true){
		gpio_put_all(0);
		sleep_ms(1000);
		
		gpio_put(26, true);
		sleep_ms(1000);
		
		gpio_put(27, true);
		sleep_ms(1000);
		
		gpio_put(28, true);
		sleep_ms(1000);
	}
	
	return 0;
}
