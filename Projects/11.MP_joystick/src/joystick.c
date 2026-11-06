#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "neopixel_communication.pio.h"

/* ------------ Pin Config ------------- */
#define 	NEOPIXEL_OUT_PIN 		15
#define		JOYSTICK_X_PIN			26
#define 	JOYSTICK_Y_PIN			27
#define		JOYSTICK_BUTTON_PIN		16
/* ------------ ADC config ------------- */
#define		JOYSTICK_CHANNEL_X		0
#define		JOYSTICK_CHANNEL_Y		1
/* ------------ PIO Config ------------- */
#define		NUM_CC_PER_BIT			10
#define 	DEAD_ZONE_BOUNDS		0.08
#define 	MAX_BRIGHTNESS			100.0f
#define 	LED_COUNT				8
#define		MAX_COLOR				(((uint32_t)MAX_BRIGHTNESS) << 24 | ((uint32_t)MAX_BRIGHTNESS) << 16 | ((uint32_t)MAX_BRIGHTNESS) << 8)
#define 	PIO_CLKDIV              ((1.25f / 1000000.0f * clock_get_hz(clk_sys)) / NUM_CC_PER_BIT) // Calculate clkdiv using formula in README

/* ------------ Joystick Functions ----------- */
typedef struct {
	uint16_t x;
	uint16_t y;
	bool button_press;
} joystick_data_t;

void joystick_init_pins(void) {
	// Init our joystick pin with pullup resistor to remove electrical noise
	gpio_init(JOYSTICK_BUTTON_PIN);
	gpio_set_dir(JOYSTICK_BUTTON_PIN, false);
	gpio_pull_up(JOYSTICK_BUTTON_PIN);
	gpio_set_input_enabled(JOYSTICK_BUTTON_PIN, true);
	
	// init our ADC readings for the x and y positions
	adc_init();
	adc_gpio_init(JOYSTICK_X_PIN);
	adc_gpio_init(JOYSTICK_Y_PIN);
}

joystick_data_t get_joystick_values(void) {
	// Create a struct with the joystick readings
	joystick_data_t data = {0};
	
	adc_select_input(JOYSTICK_CHANNEL_X);
	data.x = adc_read();
	
	adc_select_input(JOYSTICK_CHANNEL_Y);
	data.y = adc_read();
	
	// negate input because pullup resistor's default state is high
	data.button_press = !gpio_get(JOYSTICK_BUTTON_PIN);
	
	return data;
}

/* ------------ Neopixel Functions ----------- */
uint32_t init_neopixel(void) {
	// Get the address of our pio program
	const pio_program_t *program = &neopixel_communication_program;

	// Get instruction ptr (offset) and our state machine
	uint32_t offset = pio_add_program(pio0, program);
	int state_machine = pio_claim_unused_sm(pio0, true);
	hard_assert(offset >= 0);
	
	// Create config struct and set attributes about it to our state machine
	pio_gpio_init(pio0, NEOPIXEL_OUT_PIN);
	pio_sm_config config = neopixel_communication_program_get_default_config(offset);
	
	// Set out shift to shift left with 24 bit auto pull, this neopixel expects GRB in groups of 8 bits
	sm_config_set_out_shift(&config, false, true, 24);
	sm_config_set_clkdiv(&config, PIO_CLKDIV);
	pio_sm_set_consecutive_pindirs(pio0, state_machine, NEOPIXEL_OUT_PIN, 1, true);
	sm_config_set_sideset_pins(&config, NEOPIXEL_OUT_PIN);
	
	
	pio_sm_init(pio0, state_machine, offset, &config);
	pio_sm_set_enabled(pio0, state_machine, true);
	
	return state_machine;
}

// Turn on LED correlated to joystick position, all LEDs if button is clicked
void neopixel_put_data(joystick_data_t joystick_data, uint32_t state_machine) {
	uint32_t write_values[] = {0, 0, 0, 0, 0, 0, 0, 0};
	// Take our state machine values and normalize them to the range [-1,1]
	float x = ((joystick_data.x / 4095.0f) - 0.5f) * 2.0f;
	float y = ((joystick_data.y / 4095.0f) - 0.5f) * 2.0f;
	float dist =  sqrtf(x*x + y*y);
	
	// Utilize circuilar dead zone by calculating distance and checking bounds
	if(dist < DEAD_ZONE_BOUNDS)
		goto write;
	
	float angle = (atan2(y, x) * (180.0f / 3.14159f)) + 180.0f;
	uint32_t index = (uint32_t) round(angle / 45.0f) % 8;
	
	// Make brightness of LED proportional to distance for smooth brightness gradient
	uint8_t brightness = (uint8_t) (MAX_BRIGHTNESS * (dist));
	write_values[index] = (brightness) << 24 | (brightness) << 16 | (brightness) << 8;
	
	// Write all values to our PIO block
	write:
	for(uint8_t i = 0; i < LED_COUNT; i++){
		pio_sm_put_blocking(pio0, state_machine, 
		joystick_data.button_press ? 
		MAX_COLOR : write_values[i]
		);
	}
}

int main(void) {
	joystick_init_pins();
	uint32_t state_machine = init_neopixel();
	
	while(true){
		joystick_data_t joystick_data = get_joystick_values();
		neopixel_put_data(joystick_data, state_machine);
		sleep_us(500);
	}
	return 0;
}