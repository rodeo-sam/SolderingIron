/*
 * controller.c 
 *
 * Created:  Sat Jan 31 23:03:24 2015
 * Author: Georg von Zengen < oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "eeprom-config.h"
#include "tip.h" 
#include "timer0.h"

#include "display.h"

#define Ta 2 //ms

volatile int16_t w = 0;
int16_t esum = 0;
int16_t eold = 0;
volatile int16_t y_buffer = 0;
void adc_callback(void);
void control_set_temp(int16_t temp)
{
	w = temp;
}

void control_init(void)
{
	timer0_init();
	tip_init(&adc_callback);
	TIMSK0 |= (1 << TOIE0) | (1 << OCIE0A);
	DDRD |= (1<< PD4);
	w = config.default_temp;
}

void control(void)
{
	//TODO get temperature
	
	int16_t e = w - 350; //get_tip_temp(); //TODO maybe rename
	//display_fixed_point((uint16_t)(config.pid_p * 10),-1);
	   //guarding preventing esum over/underflows
	if(esum >= (int32_t)(0x3fff - e)){
		esum = 0x3fff;
	} else if(esum <= (int32_t)(-0x3fff + e)){
		esum = (-0x3fff);
	}else{
			esum += e;
	}

	int16_t p = (uint32_t)(config.pid_p*e)/(uint32_t)128;
	int16_t i = (int32_t)((int32_t)config.pid_i*(int32_t)Ta*(int32_t)esum)/(uint32_t)128;
	int16_t d = (int32_t)(config.pid_d/Ta*(e - eold))/(uint32_t)128;
	int16_t y = (int32_t)((int32_t)p + (int32_t)i + (int32_t)d);
	eold = e;

	   //guarding
	if(y > 255){
		y = 255;
	} else if (y < 0){
		y = 0;
	}

	y_buffer = y;
}

void adc_callback(void){
	PORTD |= (1<<PD4); //set pwm pin high
	control();
}
ISR(TIMER0_OVF_vect) {
	OCR0A = y_buffer;
	tip_start_conversion();
}
ISR(TIMER0_COMPA_vect) {
		PORTD &= ~(1<<PD4); //set pwm pin low
}
