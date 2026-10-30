### Overview

 This was a definitive step up from the previous programs. The Freenove Neopixel LED follows the WS2812 communication protocol; one not nativley supported by the Pico SDK.
 
 The Neopixel LED wheel has 3 pins, one for voltage, one for grounding, and one for signaling. This means that all data is daisy chained through 8 leds rather than 1 input per led.
 
 In order to communicate to the LEDS I had to use one of the pico's PIOs and it's respective hardware api. It's necessary to also write a PIO assembly file to handle the processing of data.
 
 In this specific case it wasn't too complicated since it was only necessary to read from the Output Shift Register (x). Based on the contents of each bit we would transmit the electric frequency from a GPIO pins
 for a 1 or a 0 and push it to the Neopixel LED. Each bit transmission takes 10 clk cycles at a frequency of 800,000hz. Be sure to check my source files for additional comments.

### SDK Headers Used
pico/stdlib.h

hardware/pio.h