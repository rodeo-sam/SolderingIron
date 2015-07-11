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
#include "eeprom-config.h"

#include "uart.h"

static volatile int16_t tip_temperature = 25; //off on startup
static void (*new_temperature_ready_callback)(int16_t);

//uint8_t tip_connected = 0;
static int16_t setted_temperature = 0;
void tip_init(void(*callback)(int16_t))
{
	new_temperature_ready_callback = callback;
	return;
}

static void tip_conversion_complete_callback(uint16_t adc)
{
	//TODO use lookup table
	accum temperature = 49.9424k + adc*0.4433k;
	tip_temperature = (int16_t) temperature;
	tip_temperature += 40; // hacked temperature offset

	new_temperature_ready_callback(tip_temperature);
}

int16_t tip_get_temp(void)
{
	return tip_temperature;
}
void tip_set_temp(int16_t t)
{
	setted_temperature = t;
}
int16_t tip_setted_temp(void)
{
	return setted_temperature;
}
//uint8_t tip_present(int16_t powered, int16_t nopowered)
//{
//	tip_connected = 0;
//	if (powered - nopowered > 10){
//		tip_connected = 1;
//	}
//	return tip_connected;
//}
//uint8_t tip_state(void)
//{
//	return tip_connected;
//}
	

void tip_start_conversion(void)
{
  // wait if adc is busy
	while(adc_is_busy());

	//ensure adc init and start is done atomicly
	uint8_t sreg = SREG;
	cli();

	adc_configuration_t conf = {
		.channel = CH_ADC0,
		.reference = REF_VCC,
		.trigger = TRGR_MANUEL,
	};
	adc_init(conf);
	adc_set_conversion_complete_callback(tip_conversion_complete_callback);
	adc_trigger();

	SREG = sreg;
	return;
}
