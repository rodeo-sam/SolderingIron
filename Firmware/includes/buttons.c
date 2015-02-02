/*
 * buttons.c 
 *
 * Created:  Sun Feb  1 23:55:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void (*plus_callback)(void);
void (*minus_callback)(void);
void buttons_init(void (*p_callback)(void), void (*m_callback)(void))
{
	plus_callback = p_callback;
	minus_callback = m_callback;
	EICRA |= (1 << ISC11) | (1 << ISC01); //falling edge generates interrupt
	EIMSK |= (1 << INT0) | (1 << INT1); //enable interrupts
	sei();
}

ISR(INT0_vect)
{
	plus_callback();
}
ISR(INT1_vect)
{
	minus_callback();
}
