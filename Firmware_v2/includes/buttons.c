/*
 * buttons.c
 *
 * Created:  Sun Feb  1 23:55:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 * 		   Keno Garlichs <k.garlichs@tu-braunschweig.de>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "config.h"
#include "buttons.h"
#include "global_vars.h"

/*
 * Initialize buttons and the corresponding interrupts
 */
void buttons_init(void)
{
	// Set portpins for buttons as input
	DDRD &= ~(1 << PD3) & ~(1 << PD2);

	// Disable internal pullups since we have them in hardware already
	PORTD &= ~(1 << PD3) & ~(1 << PD2);

	// interrupts
	EICRA |= (1 << ISC11) | (1 << ISC01); //falling edge generates interrupt
	EIMSK |= (1 << INT0)  | (1 << INT1); //enable interrupts

	// enable global interrupts
	sei();
}



ISR(INT0_vect) // MINUS button
{
	if (global_targetTemperature > MIN_TEMP) {
		global_targetTemperature -= 10;
		global_showTargetTemperature = 0; // reset time to show digits
	}
}



ISR(INT1_vect) // PLUS button
{
	if (global_targetTemperature < MAX_TEMP) {
		global_targetTemperature += 10;
		global_showTargetTemperature = 0; // reset time to show digits
	}
}
