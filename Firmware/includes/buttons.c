/*
 * buttons.c 
 *
 * Created:  Sun Feb  1 23:55:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t pin = 0;

void (*plus_callback)(void);
void (*minus_callback)(void);
void buttons_init(void (*p_callback)(void), void (*m_callback)(void))
{
	plus_callback = p_callback;
	minus_callback = m_callback;

  // interrupts
	EICRA |= (1 << ISC11) | (1 << ISC01); //falling edge generates interrupt
	EIMSK |= (1 << INT0)  | (1 << INT1); //enable interrupts

  // timer
	TCCR1B |= (1 << CS12); // prescaler = 256 => ~2.1s to overflow

	sei();
}

ISR(INT0_vect)
{
  if (pin != PD3) {
	  plus_callback() ;
    OCR1A = TCNT1 + 15000; // pause of 1 second
    TIFR1 |= (1<<OCF1A); // clear interrupt flag
    TIMSK1 |= (1<<OCIE1A); // enable timer compare match interrupt
    pin = PD2;
  }
}

ISR(TIMER1_COMPA_vect) {
  if (pin && !(PIND&(1<<pin))) {
    OCR1A += 1700;
    if (pin == PD2) {
      plus_callback();
    } else {
      minus_callback();
    }
  } else {
    TIMSK1 &= ~(1<<OCIE1A);
    pin = 0;
  }
}

ISR(INT1_vect)
{
  if (pin != PD2) {
	  minus_callback();
    OCR1A = TCNT1 + 15000; // pause of 1 second
    TIFR1 |= (1<<OCF1A); // clear interrupt flag
    TIMSK1 |= (1<<OCIE1A); // enable timer compare match interrupt
    pin = PD3;
  }
}


