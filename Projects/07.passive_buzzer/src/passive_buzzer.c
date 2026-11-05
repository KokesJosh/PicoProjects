#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "note_frequencies.h"

// Define Global Constants
#define PICO2W_freq 150000000.0f
#define INPUT_GPIO 16
#define OUTPUT_GPIO 15
#define WRAP 4095

// Calculate the div value from our desired frequency and the wrap value
float calculateDiv(float freq, float wrap){
	return PICO2W_freq / (freq * (wrap + 1));
}

int main(){
	// Create the array of our notes and their durations (Hollow Knight Theme)
	float notes[] = {C3, REST, C3, D3, Eb3, D3, REST, C3, G2, Ab3, REST, G2, F2, G2, REST};
	int time[] = {950, 50, 1000, 500, 500, 2950, 50, 500, 500, 950, 50, 500, 500, 1950, 550};
	
	// Init our input GPIO
	gpio_init(INPUT_GPIO);
	gpio_set_dir(INPUT_GPIO, false);
	
	// Init our output GPIO, it will use the PWM to drive the passive buzzer
	gpio_set_function(OUTPUT_GPIO, GPIO_FUNC_PWM);
	uint32_t slice = pwm_gpio_to_slice_num(OUTPUT_GPIO);
	uint32_t channel = pwm_gpio_to_channel(OUTPUT_GPIO);
	
	
	// Init our pwm and set our buzzer to max volume
	pwm_set_clkdiv(slice, 150.0f);
	pwm_set_wrap(slice, WRAP);
	pwm_set_chan_level(slice, channel, WRAP);
	
	//for our buzzer, frequency controls pitch while duty cycle controls volume
	// f(pwm) = f sys (150,000,000hz) / (div * (wrap + 1))
	while(true){
		// If our GPIO is still recieving voltage, then the button is not pressed. Do not play sound
		if(gpio_get(INPUT_GPIO)){
			pwm_set_wrap(slice, 0);
			pwm_set_enabled(slice, false);
			sleep_ms(50);
			continue;
		}
		
		// Our button has been pressed, play our song
		pwm_set_enabled(slice, true);
		pwm_set_wrap(slice, WRAP);
		for(int i = 0; i < 15; ++i){
			// Play sound only if the note is not a rest.
			if(!notes[i] == REST){
				pwm_set_clkdiv(slice, calculateDiv(notes[i], 4095));
				sleep_ms(time[i]);
			}else{
				pwm_set_enabled(slice, false);
				sleep_ms(time[i]);
				pwm_set_enabled(slice, true);
			}
			
		}
	}
	return 0;
}