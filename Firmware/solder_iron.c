/*
 * solder_iron.c
 *
 * Created: Do 29. Jan 14:23:00 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

//#ifndef F_CPU
//#define F_CPU 8000000
//#endif

#include <util/delay.h>
//#include "includes/uart.h"
#include <avr/eeprom.h>
#include "includes/display.h"
#include <avr/io.h>
#include "config.h"


uint16_t last_temperature EEMEM = 350; //default


int main(void)
{
	display_init();

	display_number(-120);
	//display_digit(1,3);

	_delay_ms(100);

	while(1)
	{
		display_update();
		_delay_ms(2);
	}
}
