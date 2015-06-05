/*
 * timing_test.c 
 *
 * Created:  Wed Feb  4 00:07:05 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "../config.h"
#include "clock.h"
#include "timing.h"

int main(void)
{
	DDRD |= (1<<PD5); // power pin
	PORTD |= (1<<PD5); // enabling power permanently (pwm-test)
	
	for (int i = 0; i < 100; i++) // for 10 seconds
		_dely_ms(100);
	
	// now creating 50% pwm at the usual 10 Hz
	while(1)
	{
		PORTD &= ~(1<<PD5); // disabling power
		_delay_ms(50);
		PORTD |= (1<<PD5); // enabling power
		_delay_ms(50);
	}
	
	// now a voltage above 4.2 volts at test point TPTEMP should trigger safety once again
	// if not please adjust resistors accordingly
}
