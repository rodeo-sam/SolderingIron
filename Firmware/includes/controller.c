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
#include "uart.h"



#include "display.h"


volatile int16_t w = 0;

volatile int16_t esum = 0;
volatile int16_t eold = 0;
volatile int16_t y_buffer = 255;
volatile uint8_t Trand = 255; 
void new_temperature_ready_callback(int16_t);

void control_set_temp(int16_t temp)
{
	w = temp;
}

void control_init(void)
{
	printf("controller boot\r\n");
	timer0_init();
	tip_init(&new_temperature_ready_callback);
	TCCR0A |= (1 << WGM01); //CTC mode
	//TCCR0A |= (1 << COM0A1); //| (1 << COM0B0) | (1 << COM0B1); //clear at COMPA(MAX), set at COMPB 
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B);
	OCR0A = 255;
	
	DDRD |= (1<< PD4) ;
	w = config.default_temp;
}

void control(int16_t temp)
{
	// XXX test this
	int16_t e = w - temp; 
	//display_fixed_point((uint16_t)(config.pid_p * 10),-1);
	   //guarding preventing esum over/underflows
	if(esum >= (int32_t)(0x3fff - e)){
		esum = 0x3fff;
	} else if(esum <= (int32_t)(-0x3fff + e)){
		esum = (-0x3fff);
	}else{
			esum += e;
	}
	
	int16_t p = ((int32_t)((int32_t)config.pid_p*(int32_t)e))/(int32_t)128;
	int16_t i = 0;//(int32_t)((int32_t)config.pid_i*(int32_t)Trand*(int32_t)esum)/(uint32_t)128;
	int16_t d = 0;//(int32_t)(config.pid_d/Trand*(e - eold))/(uint32_t)128;
	int16_t y = (int32_t)((int32_t)p + (int32_t)i + (int32_t)d);
	eold = e;

	   //guarding
	if(y > 255){
		y = 255;
	} else if (y < 0){
		y = 0;
	}
	//printf("e: %d y: %d\r\n",e,y);

	y_buffer = y;
}

uint8_t compb_state = 0;
void new_temperature_ready_callback(int16_t temp){
	if(compb_state == 1 && TCNT0 < OCR0B){
		compb_state = 2;
		TIFR0 |= (1 << OCIE0B);
		TIMSK0 |= (1 << OCIE0B);
	}else if(compb_state == 2 ||TCNT0 >= OCR0B ){
		// we missed compb
		if(y_buffer){
			PORTD |= (1 << PD4);
		}
		//TCCR0A |= (1 << COM0A1);//clear pin on COMPA
	}
	control(temp); //do this some where else?
}

ISR(TIMER0_COMPA_vect) {
	//set pwm pin low
	PORTD &= ~(1 << PD4);
	Trand = 16 + (get_random() % (255-16));
	OCR0A = Trand; //set OCR0A to next pin low time
	compb_state = 0;
	TIFR0 |= (1 << OCIE0B);
	TIMSK0 |= (1 << OCIE0B);
	OCR0B = TCNT0 + 3; // wait until PWM caused offset voltate has faded from temperature sensor
}

ISR(TIMER0_COMPB_vect) {
	if(compb_state == 0){
		OCR0B = ((uint16_t)((uint16_t)(255-y_buffer) * (uint16_t)Trand) / 255) - 1; //set OCR0B to pin high time
		tip_start_conversion();
		compb_state = 1;
		TIMSK0 &= ~(1 << OCIE0B);
	}else if(compb_state == 1){
		OCR0B = TCNT0 + 1; //wait some more time for ADC
	}else if(compb_state == 2){
		//TCCR0A |= (1 << COM0A1);//clear pin on COMPA
		//TIMSK0 &= ~(1 << OCIE0B); //disable COMPB interrupt
		if(y_buffer){
			PORTD |= (1 << PD4);
		}
	}
}
