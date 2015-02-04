/*
 * tip.c 
 *
 * Created:  Mon Feb  2 22:32:48 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 *         Karsten Hinz <k.hinz@tu-bs.de>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdfix.h>
#include "adc.h"

static volatile int16_t tip_temperature = 25; //off on startup
static void (*adc_complete_callback)(uint16_t);


void tip_init(void(*callback)(uint16_t))
{
	adc_complete_callback = callback;
	return;
}

int16_t tip_temperature_from_adc(uint16_t adc)
{
	accum temperatur = 13.3081k + adc*0.5943k;
	tip_temperature = (int16_t) temperatur;
	return tip_temperature;
}

int16_t tip_get_temp(void)
{
	return tip_temperature;
}

void tip_start_conversion(void)
{
	//ensure adc init and start is done atomicly
	uint8_t sreg = SREG;
	cli();

	adc_configuration_t conf = {
		.channel = CH_ADC0,
		.reference = REF_VCC,
		.trigger = TRGR_MANUEL,
	};
	adc_init(conf);
	adc_set_conversion_complete_callback(adc_complete_callback);
	adc_trigger();

	SREG = sreg;
	return;
}
