/*
 * temperature.c
 *
 * Created: Fr 30. Jan 14:26:23 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#include "adc.h"
#include <avr/eeprom.h>
extern int8_t temperature_offset EEMEM;

/** reads the temperature value from the internal sensor
 * uses free running?
 */
static void temperature_init(void) {
	adc_configuration_t conf = {
		.channel = CH_TEMPERATURE,
		.reference = REF_1V1_NC,
		.trigger = TRGR_NO_TRIGGER,
	};
	adc_init(conf);
}

/* calc temperature in celsius degree*/
int8_t measure_get_internal_temperature() {
	temperature_init();

	uint8_t k = 1;
	int8_t offset =  eeprom_read_byte((uint8_t*)&temperature_offset);
	int16_t Tos = -290;
	Tos += offset;
	//T = k * [(ADCH << 8) | ADCL] + T OS
	uint16_t raw = adc_average(); // blocking call
	int16_t T = (k * raw + Tos);
	
	// limit values
	if (T<-127) T=-127;
	if (T>127) T=127;

	return (int8_t) T;
}

//TODO 
//int16_t measure_get_tip_temperature()
