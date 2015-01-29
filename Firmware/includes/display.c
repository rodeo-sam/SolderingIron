
#include "display.h"
#include <avr/io.h>

void display_init() {
	DDRB = 0xff;
	PORTB= 0;
	DDRC |= LED0|LED1|LED2;
}

void display_digit(uint8_t led, uint8_t digit) {
	PORTC &= ~(LED0|LED1|LED2); //all off

	if (digit>15) {
		//error
		PORTB = 1;
	} else {
		uint8_t sign[16] = {250, 10, 236, 174, 30, 182,  246, 42, 254, 190,  126, 214, 196, 206, 244, 116};
		PORTB = sign[digit];
	}
	PORTC |= led;
}
