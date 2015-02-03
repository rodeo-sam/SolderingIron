
/*
 * clock.c 
 *
 * Created:  Tue Feb  3 23:19:19 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 


#include <util/atomic.h>
#include <avr/interrupt.h>
#include "clock.h"
#include "timing.h"

void clock_init (void)
{
	//ASSR = (1<<AS2); //select external 32.768kHz Crystal, must be written before TCCR2A and OCR2A
	TCNT2 = 0; // start time is 0
	TCCR2A = 0;
	TCCR2A |= (1 << WGM21) | (1 << CS22) | (1 << CS21); //CTC mode, prescaler 256
	OCR2A = 122; //Timer2 resets after 122 steps
	TIMSK2 = 0;
	TIMSK2 |= (1 << OCIE2A); // enable interrupt when timer2 is 128
	sei();
}

void clock_set (uint16_t new_sec, uint8_t new_subsec)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		clock_offset.sec= new_sec- seconds;
		clock_offset.subsec= new_subsec - subsec;
		seconds = new_sec;
		subsec = new_subsec;
		subsec %= 128; //prevent values over 128
		timer_updated=0;
	}
}

uint16_t clock_get_sec (void)
{
	uint16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		tmp=seconds;
	}
	return tmp;
}

uint8_t clock_get_subsec (void)
{
	uint8_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		tmp=subsec;
	}
	return tmp;
}

uint8_t clock_get_subsubsec(void)
{
	uint8_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		tmp=subsubsec;
	}
	return tmp;
}

ISR (TIMER2_COMP_vect)
{
	subsubsec++;
	if (subsubsec == 4) {
		subsubsec = 0;
		subsec++;
		if (subsec == 128) {
			subsec = 0;
			seconds++;
			seconds %= 512;
		}
	}

}
