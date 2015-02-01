/*
 * controller.c 
 *
 * Created:  Sat Jan 31 23:03:24 2015
 * Author: Georg von Zengen < oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "eeprom-config.h"
#include "tip.h" //TODO maybe rename

#define Ta 2 //ms

static uint8_t w = 0;
static int32_t esum = 0;
static int16_t eold = 0;

void conrol_set_temp(uint16_t temp)
{
	w = temp;
}

void control_init(void)
{
	TCCR0A = 0;
	TCCR0A |= (1 << COM0A1); //Clear OC0B on Compare Match
	TCCR0A |= (1 << WGM00) | (1 << WGM01) ; //Fast pwm
	TCCR0B = 0;
	TCCR0B |= (1 << WGM02); //Fast pwm
	TCCR0B |= (1 << CS01) | (1 << CS00); // prescaler = 64 => ~2ms to overflow
	TIMSK0 = 0;
	TIMSK0 |= (1 << TOIE0);
	sei();
}

void control(void)
{
	//TODO get temperature
	
	int16_t e = w - get_tip_temp(); //TODO maybe rename
	   //guarding preventing esum over/underflows
	if(esum > (0xffffffff>2) - e){
		esum = (0xffffffff>2);
	} else if(esum < ((0xffffffff>2) * (-1)) + e){
		esum = ((0xffffffff>2) * (-1));
	}else{
		esum = esum + e;				
	}

	int16_t y = config.pid_p*e + config.pid_i*Ta*esum + config.pid_d/Ta*(e – eold);
	eold = e;

	   //guarding
	if(y > 255){
		y = 255;
	} else if (y < 0){
		y = 0;
	}

	OCR0A  = y
}

ISR(TIMER1_OVF_vect) {
	control();
}
