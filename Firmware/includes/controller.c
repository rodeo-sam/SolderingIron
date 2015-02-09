/*
 * controller.c 
 *
 * Created:  Sat Jan 31 23:03:24 2015
 * Author: Georg von Zengen < oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "eeprom-config.h"
#include "tip.h" 
#include "timer0.h"
#include "uart.h"



#include "display.h"

#define Ta 100 //ms
#define Trand ((Ta*32)/255)

#define MAXESUM 10   // maximum of error integration most not be too high because the tip heats very fast.
#define MINESUM (-1*MAXESUM)

volatile int16_t w = 0;

volatile int16_t esum = 0;
volatile int16_t eold = 0;
volatile int16_t y_buffer = 255;
void new_temperature_ready_callback(int16_t);

void control_set_temp(int16_t temp)
{
	w = temp;
}

void control_init(void)
{
	printf("controller boot\r\n");
	OCR0A = Trand;
	TCCR0A |= (1 << WGM01); //CTC mode
	TIMSK0 |= (1 << OCIE0A) ;
	tip_init(&new_temperature_ready_callback);
	tip_start_conversion();
	DDRD |= (1<< PD4) ;
	w = config.default_temp;
	timer0_init();
	
}
void control(int16_t temp)
{
	int16_t e = w - temp; 
	//guarding preventing esum over/underflows
	if(esum >= (int32_t)(MAXESUM - e)){
		esum = MAXESUM;
	} else if(esum <= (int32_t)(MINESUM + e)){
		esum = (MINESUM);
	}else{
		if(((y_buffer > 0) && (y_buffer < 255) ) || ((y_buffer == 255) && (e < 0)) || ( (y_buffer == 0) && (e > 0))){
			esum += e;
		}
	}
	
	int16_t p = ((int32_t)((int32_t)config.pid_p*(int32_t)e))/(int32_t)128;
	int16_t i = (int32_t)((int32_t)config.pid_i*(int32_t)Ta*(int32_t)esum)/(uint32_t)128;
	int16_t d = (int32_t)(config.pid_d*(e - eold)/Ta)/(uint32_t)128;
	int16_t y = (int32_t)((int32_t)p + (int32_t)i + (int32_t)d);
	eold = e;

	//guarding values higher then 255 are not possible 
	if(y > 255){
		y = 255;
	} else if (y < 0){
		y = 0;
	}
	//printf("e: %d y: %d\r\n",esum,y);

	y_buffer = y;
}
uint16_t pwm_count =0;
uint8_t compb_state = 0;
void new_temperature_ready_callback(int16_t temp){
	control(temp);
	compb_state=1;
}

ISR(TIMER0_COMPA_vect) {
	uint8_t t = 0;
	if((pwm_count == 0) && compb_state && (y_buffer > 2) ){
		compb_state = 0;
		PORTD |= (1 << PD4);
	}else if((pwm_count == 1) && (y_buffer == 0)){
		PORTD &= ~(1 << PD4);
		compb_state = 0;
		tip_start_conversion();
	} else if((pwm_count == 0) && !compb_state ){
		pwm_count = 0;
		t = 1;
	} else {
		if(pwm_count == y_buffer){ 
			PORTD &= ~(1 << PD4);
		}else if((pwm_count == y_buffer + 2)){ 
			tip_start_conversion();
		}
	}
	if((compb_state == 1) && (pwm_count >= 255)){
		pwm_count = 0;
		t = 1;
	}
	if(!t){
		pwm_count++;
	}
}

