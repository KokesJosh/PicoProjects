#include "pico/stdlib.h"

#define INPUT_GPIO 16
#define OUTPUT_GPIO 15

int main(void){
	// initilize our GPIO pins to input
	gpio_init(INPUT_GPIO);
	gpio_init(OUTPUT_GPIO);
	
	gpio_set_dir(INPUT_GPIO, GPIO_IN);
	gpio_set_dir(OUTPUT_GPIO, GPIO_OUT);
	
	gpio_put(OUTPUT_GPIO, false);
	
	while(true){
		gpio_put(OUTPUT_GPIO, !gpio_get(INPUT_GPIO));
	}
	return 0;
}