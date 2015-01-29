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

#define LED0 (1<<PC3)
#define LED1 (1<<PC4)
#define LED2 (1<<PC5)

uint16_t last_temperature EEMEM = 350;//default


int main(void)
{
	display_init();
	PORTC |= LED0;


	display_digit(0, 4);
	display_digit(1, 5);
	display_digit(2, 6);

	_delay_ms(100);

	while(1)
	{
		display_update();
		_delay_ms(2);
	}
}
