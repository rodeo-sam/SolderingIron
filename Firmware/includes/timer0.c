/*
 * timer0.c 
 *
 * Created:  Sun Feb  1 17:15:39 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
void timer0_init(void)
{
	static once = 0;
	if(!once){
		once = 1;
		TCCR0A = 0;
		TCCR0A |= (1 << COM0A1); //Clear OC0B on Compare Match
		TCCR0A |= (1 << WGM00) | (1 << WGM01) ; //Fast pwm
		TCCR0B = 0;
		TCCR0B |= (1 << WGM02); //Fast pwm
		TCCR0B |= (1 << CS01) | (1 << CS00); // prescaler = 64 => ~2ms to overflow
		TIMSK0 = 0;
		TIMSK0 |=  (1 << OCIE0B);
		sei();
	}
}
