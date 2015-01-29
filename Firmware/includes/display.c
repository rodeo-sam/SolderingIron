
#include "display.h"
#include <avr/io.h>



// initializing the DDRs and ports for the LED display
void display_init() {
	DDRB = 0xff;
	PORTB= 0;
	DDRC |= LED0|LED1|LED2;
}

// put a specific character up on a specific area of the seven segment display
void display_digit(uint8_t led, uint8_t digit) {

	PORTC &= ~(LED0|LED1|LED2); //switching all LEDs off

	if (digit>16) {
		//error in case digit was too high
		digit = 16; // displaying only the dot

	} else {

		uint8_t sign[17] = {DISP_0, DISP_1, DISP_2, DISP_3, DISP_4, DISP_5, DISP_6, DISP_7, DISP_8, DISP_9, DISP_A, DISP_B, DISP_C, DISP_D, DISP_E, DISP_F, DISP_DOT};
		PORTB = sign[digit];

	}

	PORTC |= led; // enabling only the desired LED display
}

// setting up custom characters on the seven segment displays
void display_custom(uint8_t led, uint8_t segments) {
	PORTC &= ~(LED0|LED1|LED2); //switching all LEDs off

	PORTB = segments;

	PORTC |= led; // enabling only the desired LED display
}
