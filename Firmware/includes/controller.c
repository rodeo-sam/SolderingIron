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
#include "timer0.h"

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
	timer0_init();
	TIMSK0 |= (1 << TOIE0) | (1 << OCIE0A);
	DDRD |= (1<< PD4);
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
	PORTD |= (1<<PD4); //set pwm pin high, only needed in rev 0.1
}
ISR(TIMER1_OCA0_vect) {
	PORTD &= ~(1<<PD4); //set pwm pin low, only needed in rev 0.1
	control();
}
