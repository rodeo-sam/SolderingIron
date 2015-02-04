/*
 * adc.c
 *
 * Created: Fr 30. Jan 13:42:47 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "display.h"

#ifndef F_CPU
#error F_CPU needs to be defined for the AD Keypad to work
#endif

// The ADC clock needs to be between 50kHz and 200kHz
#if F_CPU == 1000000UL
#define ADC_PRESCALER 3 // Prescaler of 8 = ADC clock of 125.0kHz
#elif F_CPU == 2000000UL
#define ADC_PRESCALER 4 // Prescaler of 16 = ADC clock of 125.0kHz
#elif F_CPU == 4000000UL
#define ADC_PRESCALER 5 // Prescaler of 32 = ADC clock of 125.0kHz
#elif F_CPU == 6000000UL
#define ADC_PRESCALER 5 // Prescaler of 32 = ADC clock of 187.5kHz
#elif F_CPU == 7372800UL
#define ADC_PRESCALER 6 // Prescaler of 64 = ADC clock of 115.2kHz
#elif F_CPU == 8000000UL
#define ADC_PRESCALER 6 // Prescaler of 64 = ADC clock of 125.0kHz
#elif F_CPU == 16000000UL
#define ADC_PRESCALER 7 // Prescaler of 128 = ADC clock of 125.0kHz
#else
#error Unsupported system clock
#endif

static void (*adc_complete_callback)(uint16_t);

/* Attention: if using (auto) trigger aka interrupt based
 * convertion - pass a function pointer to adc_complete_callback 
 */
void adc_init(adc_configuration_t conf)	{

	// set input
	ADMUX = conf.channel;

	// set reference voltage
	ADMUX &= ~((1<<REFS1)|(1<<REFS0));
	ADMUX |= (conf.reference<<REFS0);
	
	// clear and set adjustment
	ADMUX |= (0<<ADLAR);
	// set auto trigger source
	if (!(conf.trigger & 8)) {
		// enable auto trigger 
		ADCSRA |= (1<<ADATE); 
		// set source
		ADCSRB &= ~7;
		ADCSRB |= (conf.trigger & 7);
		// set interrupt
		ADCSRA |= (1<< ADIE);

	} else if (conf.trigger == TRGR_MANUEL) {
		// disable auto trigger
		ADCSRA &= ~(1<<ADATE);
		// enable interrupt
		ADCSRA |= (1<<ADIE);
	} else {/* NO_TRIGGER */
		// disable auto trigger
		ADCSRA &= ~(1<<ADATE);
		// disable interrupt
		ADCSRA &= ~(1<<ADIE);
	}
	// set Prescaler
	ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADCSRA |= (ADC_PRESCALER);
}

bool adc_is_busy() {
	return (ADCSRA & (1<<ADSC));
}

uint16_t adc_read_blocking() {
	ADCSRA |= (1<<ADSC); //start 
	while(ADCSRA & (1<<ADSC)); //wait unitl finish	
	uint16_t raw = ADCL; //has to be read first
	raw |= (ADCH << 8);
	return raw;
}

void adc_stop() {
	// disable ADC
	ADCSRA &= ~(1<<ADEN);
	//clear interrupt request if one occuered
	ADCSRA |= (1<<ADIF);
}

void adc_trigger() {
	// enable ADC
	ADCSRA |= (1<<ADEN);
	//start 
	ADCSRA |= (1<<ADSC); 
}

void adc_run() {
	// enable ADC
	ADCSRA |= (1<<ADEN);
}

void adc_set_conversion_complete_callback(void (*adc_callback)(uint16_t)) {
	adc_complete_callback = adc_callback;
}

//-------------------------------------------------

// number of measurements to average
#define ADC_AVERAGE_SIZE 8
uint16_t adc_average() {
	return adc_average_hi(0);
}

uint16_t adc_average_hi(uint8_t r_in_M_ohm) {
	adc_run();
	uint32_t sum = 0;
	for (uint8_t i=0; i<ADC_AVERAGE_SIZE; i++) {
		//delay_us = 4*R*C ~= 4*20pF*R
		for (;r_in_M_ohm>0; r_in_M_ohm--)
			_delay_us(80);
		uint16_t value = adc_read_blocking();
		//_delay_ms(2);//XXX
		sum += value;
	}
	adc_stop();
	uint16_t av = sum / ADC_AVERAGE_SIZE;
	return av;
}

//-------------------------------------------------------------//
void ADC_vect (void )
__attribute__ (( used, externally_visible, signal ));
//-------------------------------------------------------------//
void ADC_vect() {
	adc_stop();
	// calls the callback an passes the new measure
	adc_complete_callback(ADC);
}
