#include "pico/stdlib.h"

int main(void){
	gpio_init(0);
	
	gpio_set_dir(0, GPIO_OUT);
	while(true){
		gpio_put(0, true);
		sleep_ms(1000);
		
		gpio_put(0, false);
		sleep_ms(1000);
	}
}