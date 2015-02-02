/*
 * buttons.c 
 *
 * Created:  Sun Feb  1 23:55:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t pin = 0;
volatile uint8_t third_pending = 0;

void (*plus_callback)(void);
void (*minus_callback)(void);
void (*third_callback)(void);
void (*third_long_callback)(void);
void buttons_init(void (*p_callback)(void), void (*m_callback)(void), void (*t_callback)(void), void (*tl_callback)(void))
{
	plus_callback = p_callback;
	minus_callback = m_callback;
	third_callback = t_callback;
	third_long_callback = tl_callback;

  // interrupts
	EICRA |= (1 << ISC11) | (1 << ISC01); //falling edge generates interrupt
	EIMSK |= (1 << INT0)  | (1 << INT1); //enable interrupts

  // timer
	TCCR1B |= (1 << CS12); // prescaler = 256 => ~2.1s to overflow

	sei();
}

ISR(INT0_vect)
{
	if (!third_pending){
		if (pin != PD3) {
			plus_callback() ;
			OCR1A = TCNT1 + 15000; // pause of 0.5 second
			TIFR1 |= (1<<OCF1A); // clear interrupt flag
			TIMSK1 |= (1<<OCIE1A); // enable timer compare match interrupt
			pin = PD2;
		} else if (pin == PD3){
			pin += PD2; 
			OCR1A = TCNT1 + 65000; // pause of 2 second
			third_pending = 1;
			EIFR |= (1 << INTF1) | (1 <<INTF0);
			EICRA |= (1 << ISC10) | (1 << ISC00); //rising edge generates interrupt
		}
	}else{
		EICRA &= ~((1 << ISC10) | (1 << ISC00)); //disable rising edge interrupt
		pin = 0;
		third_pending = 0;
		third_callback();
	}

}


ISR(INT1_vect)
{
	if (!third_pending){
		if (pin != PD2) {
			minus_callback();
			OCR1A = TCNT1 + 15000; // pause of 0.5 second
			TIFR1 |= (1<<OCF1A); // clear interrupt flag
			TIMSK1 |= (1<<OCIE1A); // enable timer compare match interrupt
			pin = PD3;
		} else if (pin == PD2){
			pin += PD3; 
			OCR1A = TCNT1 + 65000; // pause of 2 second
			third_pending = 1;
			EIFR |= (1 << INTF1) | (1 <<INTF0);
			EICRA |= (1 << ISC10) | (1 << ISC00); //rising edge generates interrupt
		}
	}else{
		EICRA &= ~((1 << ISC10) | (1 << ISC00)); //disable rising edge interrupt
		pin = 0;
		third_pending = 0;
		third_callback();
	}
}


ISR(TIMER1_COMPA_vect) {
  if (pin && (pin < (PD2 + PD3)) && (!(PIND&(1<<pin)))) {
    OCR1A += 1700;
    if (pin == PD2) {
      plus_callback();
    } else if (pin == PD3){
      minus_callback();
    }
  } else if((pin == (PD2 + PD3)) && ((!(PIND & (1 << PD2))) && (!(PIND & (1 << PD3))))){
    pin = 0;
    third_pending = 0;
    EICRA &= ~((1 << ISC10) | (1 << ISC00)); //disable rising edge interrupt
    third_long_callback();
  } else {
    TIMSK1 &= ~(1<<OCIE1A);
    pin = 0;
  }
}
