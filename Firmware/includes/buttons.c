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
#include "buttons.h"


uint8_t pin = 0;
volatile uint8_t third_pending = 0;

/* Plus button was pressed */
void (*plus_callback)(void);
/* Minus button was pressed */
void (*minus_callback)(void);
/* Both buttons (plus and minus) where pressed at the same time */
void (*third_callback)(void);
/* Both buttons were held */
void (*third_long_callback)(void);

/*
 * Initialize buttons and assign functions to be called when a respective button was pressed 
 */
void buttons_init(void (*p_callback)(void), void (*m_callback)(void), void (*t_callback)(void), void (*tl_callback)(void))
{
	// Set portpins for buttons as input
	BUTTON_PORT_DDR &= ~(1 << PLUS_PIN) & ~(1 << MINUS_PIN);

	// Disable internal pullups since we have them in hardware already
	BUTTON_PORT &= ~(1 << PLUS_PIN) & ~(1 << MINUS_PIN);

	plus_callback = p_callback;
	minus_callback = m_callback;
	third_callback = t_callback;
	third_long_callback = tl_callback;

	// interrupts
	EICRA |= (1 << ISC11) | (1 << ISC01); //falling edge generates interrupt
	EIMSK |= (1 << INT0)  | (1 << INT1); //enable interrupts

	// timer
	TCCR1B |= (1 << CS12); // prescaler = 256 => ~2.1s to overflow

	// globally enable interrupts
	sei();
}
volatile uint8_t plus_edge = 0;
ISR(PLUS_INTERRUPT_VECT)
{
	if (!third_pending){
		if (plus_edge == 1){
			if(plus_callback != NULL) {
				plus_callback();
			}
			plus_edge = 0;
			EICRA &= ~(1 << ISC00); //disable rising edge interrupt
			EIFR |= (1 <<INTF0);
		} else
		if (pin != MINUS_PIN || (PIND & (1 << MINUS_PIN)) && (plus_edge == 0)) {
			plus_edge = 1;
			OCR1A = TCNT1 + 15000; // pause of 0.5 second
			TIFR1 |= (1<<OCF1A); // clear interrupt flag
			TIMSK1 |= (1<<OCIE1A); // enable timer compare match interrupt
			pin = PLUS_PIN;
			EICRA |= (1 << ISC00); //rising edge generates interrupt
			EIFR |= (1 << INTF1) | (1 <<INTF0);
		} else if ((pin == MINUS_PIN) && (!(PIND & (1 << MINUS_PIN)))){
			pin += PLUS_PIN; 
			OCR1A = TCNT1 + 65000; // pause of 2 second
			third_pending = 1;
			EIFR |= (1 << INTF1) | (1 <<INTF0);
			EICRA |= (1 << ISC10) | (1 << ISC00); //rising edge generates interrupt
		}
	} else {
		// Third button was released, trigger event now
		EICRA &= ~((1 << ISC10) | (1 << ISC00)); //disable rising edge interrupt
		pin = 0;
		third_pending = 0;

		// If defined, call handling function for third button
		if(third_callback != NULL) {
			third_callback();
		}
	}

}


volatile uint8_t minus_edge = 0;
ISR(MINUS_INTERRUPT_VECT)
{
	if (!third_pending){
		if (minus_edge == 1){
			if(minus_callback != NULL) {
				minus_callback();
			}
			minus_edge = 0;
			EICRA &= ~(1 << ISC10); //disable rising edge interrupt
			EIFR |= (1 << INTF1) ;
		} else
		if (pin != PLUS_PIN || (PIND & (1 << PLUS_PIN))) {
			minus_edge = 1;
			OCR1A = TCNT1 + 15000; // pause of 0.5 second
			TIFR1 |= (1<<OCF1A); // clear interrupt flag
			TIMSK1 |= (1<<OCIE1A); // enable timer compare match interrupt
			pin = MINUS_PIN;
			EICRA |= (1 << ISC10) ; //rising edge generates interrupt
			EIFR |= (1 << INTF1) ;
		} else if ((pin == PLUS_PIN) && (!(PIND & (1 << PLUS_PIN)))){
			pin += MINUS_PIN; 
			OCR1A = TCNT1 + 65000; // pause of 2 second
			third_pending = 1;
			// We want to trigger the event when the button was released
			EIFR |= (1 << INTF1) | (1 <<INTF0);
			EICRA |= (1 << ISC10) | (1 << ISC00); // Rising edge generates interrupt
		}
	} else {
		EICRA &= ~((1 << ISC10) | (1 << ISC00)); // Disable rising edge interrupt
		pin = 0;
		third_pending = 0;

		// If defined, call handling function for third button
		if(third_callback != NULL) {
			third_callback();
		}
	}
}


ISR(TIMER1_COMPA_vect) {
	// A button was pressed before, but only one of them and
	if (pin && (pin < (PLUS_PIN + MINUS_PIN)) && (!(PIND&(1<<pin)))) {
		OCR1A += 1700;
		if (pin == PLUS_PIN) {
			// Plus button was pressed
			if(plus_callback != NULL) {
				plus_callback();
			}
		} else if (pin == MINUS_PIN){
			// Minus button was pressed
			if(minus_callback != NULL) {
				minus_callback();
			}
		}
	} else if((pin == (PLUS_PIN + MINUS_PIN)) && ((!(PIND & (1 << PLUS_PIN))) && (!(PIND & (1 << MINUS_PIN))))) {
		// Both buttons were kept pressed, trigger event and reset everything to initial state
		pin = 0;
		third_pending = 0;
		EICRA &= ~((1 << ISC10) | (1 << ISC00)); //disable rising edge interrupt

		// If defined, call handling function for third button hold
		if(third_long_callback != NULL) {
			third_long_callback();
		}
	} else {
		TIMSK1 &= ~(1<<OCIE1A);
		pin = 0;
	}
}
