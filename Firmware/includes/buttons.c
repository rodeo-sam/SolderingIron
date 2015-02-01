/*
 * buttons.c
 *
 * Created: Su 1. Feb 18:06:27 CET 2015
 * Author: Johannes van Balen <j.van-balen tu-bs.de>
 */
 
 
#include "buttons.h"
 

static void buttons_init() {

	#if (HW_VERSION == 1)
		// Button Up is connected to PD2 (INT0)
		// Button Down is connected to PD3 (INT1)
		DDRD  &= ~((1<<PD2) | (1<<PD3)); // set as input
		PORTD |= (1<<PD2) | (1<<PD3); // enable internal pullups
	
		EICRA |= (1<<ISC01); // enable falling edge interrupt for int0
		EICRA |= (1<<ISC11); // enable falling edge interrupt for int1
	
		EIMSK |= (1<<INT0); // enable int0
		EIMSK |= (1<<INT1); // enable int1
	
		sei();
	#else
		#error "buttons.c: buttons are not implemented for this hw_version"
	#endif
}

ISR(INT0_vect) {
	if (desired_temperature != TEMP_MAX)
		desired_temperature++;
}

ISR(INT1_vect) {
	if (desired_temperature != -99)
		desired_temperature--;
}
