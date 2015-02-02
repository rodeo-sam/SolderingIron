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
	static uint8_t once = 0;
	if(!once){
		once = 1;
		TCCR0B |= (1 << CS01) | (1 << CS00); // prescaler = 64 => ~2ms to overflow
		sei();
	}
}
