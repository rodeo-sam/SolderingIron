/*
 * solder_iron.c
 *
 * Created: Do 29. Jan 14:23:00 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#include <util/delay.h>
//#include "includes/uart.h"
#include <avr/eeprom.h>
#include "includes/display.h"
#include "includes/adc.h"
#include "includes/temperature.h"
#include <avr/io.h>
#include "config.h"


uint16_t last_temperature EEMEM = 350; //default
int8_t temperature_offset EEMEM = -52; //default


int main(void)
{
	display_init();

	//display_digit(1,3);

	_delay_ms(100);

	while(1)
	{
		uint16_t value = measure_get_internal_temperature();
		//uint16_t value = measure_get_internal_temperature();
		display_temperature(value);
		display_update();
		_delay_us(200);
	}
}
