/*
 * buttons.c
 *
 * Created: Su 1. Feb 18:06:27 CET 2015
 * Author: Johannes van Balen <j.van-balen tu-bs.de>
 */
 
 
#include "buttons.h"
 
#if BUTTON_TRIGGER_TYPE == ISR

static void buttons_init() {
	BUTTON_DDR |= BUTTON_UP | BUTTON_DN; // set as input
	BUTTON_PORT |= BUTTON_UP | BUTTON_DN; // enable internal pullups
	
	EICRA |= (1<<ISC01); // enable falling edge interrupt for int0
	EICRA |= (1<<ISC11); // enable falling edge interrupt for int1
	
	// TODO: Edit more important registers
	
	sei();
}

ISR(BUTTON_UP_ISR) {
	// TODO: Button_up
}

ISR(BUTTON_DN_ISR) {
	// TODO: Button_dn
}

 
#else
	#error "buttons.c: buttons are implemented with an ISR only"
#endif