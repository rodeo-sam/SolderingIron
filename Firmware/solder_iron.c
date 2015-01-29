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
	PORTC |= LED0;

/*		PORTB = 1;//punkt
		PORTB = 2;//rechts unten
		PORTB = 4;//-
		PORTB = 8;//ro
		PORTB = 16;//lo
		PORTB = 32;//o
		PORTB = 64;//lu
		PORTB = 128;//u
*/

		_delay_ms(100);

		int i = 0;
	while(1)
	{
		display_digit(LED1, i++);

		_delay_ms(1000);
	}
}
