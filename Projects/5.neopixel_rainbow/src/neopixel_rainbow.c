#include "pico/stdlib.h"
#include "neopixel_rainbow.pio.h"
#include "hardware/clocks.h"
#include <math.h>

#define LED_COUNT 8
#define GPIO_PIN 16
#define PERIOD 360.0f
/* This code implements the common WS2812 protocol. The neopixel led wheel tkes in 24 bits for it's color, and then passes along the input ot the next led until all bits have been consumed. */
int main(void){
	// Claim an unused state machine at in PIO0
	uint state_machine = pio_claim_unused_sm(pio0, true);
	int offset = pio_add_program(pio0, &neopixel_rainbow_program);
	hard_assert(offset >= 0); // If our offset is not >= 0, something is wrong and abort
	
	// Init our GPIO to use the PIO slice 0
	pio_gpio_init(pio0, GPIO_PIN);
	pio_sm_set_consecutive_pindirs(pio0, state_machine, GPIO_PIN, 1, true); // Set GPIO to output
	
	// create config struct from our offset & set our sideset pin to 16
	pio_sm_config config = neopixel_rainbow_program_get_default_config(offset);
	sm_config_set_sideset_pins(&config, GPIO_PIN);
	
	// Set our auto pull threshold to be 24 bits
	sm_config_set_out_shift(&config, false, true, 24);
	
	// Join our two FIFO data streams together for more capacity (not needed in this instance but good to have)
	sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_TX);
	
	// 1 bit cycle needs to be exactly 1.25 pico seconds for proper data transmision
	int cyclesPerBit = 10;
	float divider = (float) clock_get_hz(clk_sys) / (800000.0f * cyclesPerBit); 
	sm_config_set_clkdiv(&config, divider);
	
	// Init and start our PIO
	pio_sm_init(pio0, state_machine, offset, &config);
	pio_sm_set_enabled(pio0, state_machine, true);
	
	//create our color variables
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t color_value;
	
	// loop endlessly
	float count = 0;
	while(true){
		// Iterate through each LED, providing a color offset to create a color wheel effect
		for(int i = 0; i < LED_COUNT; i++){
			red = (uint32_t) (fmin(fabs( fmod(45*i + count, PERIOD) / (PERIOD / 2) - 1 ), 0.5f) * 2.0f);
			green = (uint32_t) (fmin(fabs( fmod(45*i + count + 120, PERIOD) / (PERIOD / 2) - 1 ), 0.5f) * 2.0f);
			blue  = (uint32_t) (fmin(fabs( fmod(45*i + count + 240, PERIOD) / (PERIOD / 2) - 1 ), 0.5f) * 2.0f);
			// WS2812 expects 24 bits for color GRB
			color_value  = ((green << 24) | (red << 16) | (blue << 8));
			
			// Load the color value to the state machine and process
			pio_sm_put_blocking(pio0, state_machine, color_value);
			
			// Increment count, reseting of full period is completed
			count += 1.00f;
			count = fmod(count, PERIOD);
		}
		sleep_ms(25);
	}
	return 0;
}