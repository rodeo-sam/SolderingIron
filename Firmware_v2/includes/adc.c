/*
 * adc.c
 *
 * Created: Fr 30. Jan 13:42:47 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 * Adapted by: Johannes van Balen <johannes@van-balen.de>
 */

#include <avr/io.h>
#include "adc.h"
#include "uart.h"

#ifndef F_CPU
#error F_CPU undefined
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


/* Setting up the ADC to be able to read the temperature. */
void adc_init(void)	{
	ADMUX &= 0xF0; // set input pin to ADC0
	ADMUX |= (1<<REFS0); // set reference voltage to AVcc
	ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADCSRA |= (ADC_PRESCALER); // set prescaler
	ADCSRA |= (1<<ADEN); // enabling the ADC
	unsigned char buffer[17] = "ADC - INITIATED\r\n";
  uart_transmit(buffer, 17);
}

/* This lets the system read only one value from the ADC, representing the current temperature. */
uint16_t adc_read(void) {
	ADCSRA |= (1<<ADSC); //start
	while(ADCSRA & (1<<ADSC)); //wait unitl finish
	uint16_t raw = ADCL; //has to be read first
	raw |= (ADCH << 8);
	return raw;
}
