#include <math.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

/* ----------- Pin config ----------- */
#define 	PIN_COUNT 		2
#define 	HOT_GPIO 		14
#define 	COLD_GPIO 		15
#define 	ADC_GPIO 		26

/* ----------- ADC Config ----------- */
#define 	BASE_RESISTANCE 10000.0f
#define 	THERMAL_INDEX 	3950.0f
#define 	TEMP_ONE 		25.0f
#define 	ADC_CHANNEL 	0
/* ----------- PWM Config ----------- */
#define 	PWM_DIV 		150.0f
#define 	PWM_WRAP 		4095


/* ------ float utility functions ------ */
float float_max(float f1, float f2){
	return f1 < f2 ? f2 : f1;
}

float float_min(float f1, float f2){
	return f1 > f2 ? f2 : f1;
}

float float_clamp(float min, float max, float value){
	return float_min(float_max(value, min), max);
}

/* ------ ADC Temperature calculation functions ------ */

/* We will utilize 2 equations to calculate our temperature
	NEW_RESISTANCE = BASE_RESISTANCE * e^[THERMAL_INDEX*(1/TEMP_ONE - 1/T2)]
	
	Rearranging the equation, we can get the following:
	TEMP_TWO = 1 / [ (1 / TEMP_ONE) + ln(NEW_RESISTANCE / BASE_RESISTANCE) / THERMAL_INDEX]
	
	*All temps must be converted to kelvin*
*/

// Returns CELCIUS temp from our new resistance value
float adc_calculate_temp(float R_t){
	return (1.0f / ((1.0f / (TEMP_ONE + 273.150f)) + logf(R_t / BASE_RESISTANCE) / THERMAL_INDEX)) - 273.15f;
}

// Returns NEW_RESISTANCE, Utilizes the voltage divider formula
float adc_calculate_resistance(float voltage){
	return voltage != 3.3f ? BASE_RESISTANCE * voltage / (3.3f - voltage) : BASE_RESISTANCE * voltage / (1);
}



// init all gpio pins to use pwm in array
void pwm_init_output(uint8_t* pins, uint8_t num_pins, uint8_t attributes[][PIN_COUNT]){
	for(int index = 0; index < num_pins; index++){
		// Define the attributes for our pwm
		gpio_init(pins[index]);
		gpio_set_function(pins[index], GPIO_FUNC_PWM);
		
		uint8_t pwm_slice = pwm_gpio_to_slice_num(pins[index]);
		uint8_t pwm_channel = pwm_gpio_to_channel(pins[index]);
		
		//set our tick rate, wrap, & channel level
		pwm_set_clkdiv(pwm_slice, PWM_DIV);
		pwm_set_wrap(pwm_slice, PWM_WRAP);
		pwm_set_chan_level(pwm_slice, pwm_channel, 0);
		
		// Set our output polarity to negative to simplify calculations since we're using a common anode RGB LED
		pwm_set_output_polarity(pwm_slice, true, true);
		pwm_set_enabled(pwm_slice, true);
		
		// set our attributes array values
		attributes[index][0] = pwm_slice;
		attributes[index][1] = pwm_channel;
	}
}


int main(void){
	uint8_t pins[] = {HOT_GPIO, COLD_GPIO};
	uint8_t pwm_pin_attributes[PIN_COUNT][PIN_COUNT];
	pwm_init_output(pins, PIN_COUNT, pwm_pin_attributes);
	
	// Initilize our analog to digital reader, ensure ADC_CHANNEL matches PIN;
	// Check datasheet for more information
	adc_init();
	adc_gpio_init(ADC_GPIO);
	adc_select_input(ADC_CHANNEL);
	
	while(true){
		uint16_t adc_value = adc_read();
		float R_t = adc_calculate_resistance(((float)adc_value) / 4095.0f * 3.3f); 
		float temperature = adc_calculate_temp(R_t);
		
		// We will utilize a clamped function that scales linearly with temperature to determine the brightness of our R & B LEDS
		// Hot LVL is inversley proportional to Cold LVL
		
		// Hot LVL
		pwm_set_chan_level(	
			pwm_pin_attributes[0][0], 
			pwm_pin_attributes[0][1], 
			(uint16_t) float_clamp(
				0.0f, PWM_WRAP, (temperature - TEMP_ONE - 3) * (PWM_WRAP / (TEMP_ONE + 3 - TEMP_ONE))
			)
		);
		
		// Cold LVL
		pwm_set_chan_level(	
			pwm_pin_attributes[1][0], 
			pwm_pin_attributes[1][1], 
			(uint16_t) float_clamp(
				0.0f, PWM_WRAP, (temperature - TEMP_ONE + 3) * (PWM_WRAP / (TEMP_ONE - 3 - TEMP_ONE))
			)
		);
		
		sleep_ms(150);
	}
	
	return 0;
}