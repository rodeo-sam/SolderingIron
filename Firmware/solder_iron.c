/*
 * solder_iron.c
 *
 * Created: Do 29. Jan 14:23:00 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "eeprom-config.h"
#include "display.h"
#include "adc.h"
#include "temperature.h"
#include "uart.h"


void on_watchdog_reset(void);
/******************VERY IMPORTANT********************************************
 * this code block is necessary to prevent the processor to run into watchdog-reset-lifelock 
 * do NOT delete this block
****************************************************************************/
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void)
{
	wdt_reset();
	wdt_enable(WDTO_8S);
    return;
}
/*******************end of very important block***************************/

int main(void)
{
	wdt_disable();
	on_watchdog_reset();
	wdt_enable(WDTO_500MS);
	config_load();
	display_init();
	clock_init();
	//control_init();  //leave this as a comment until we want to heat things up



	display_number(100);
	while(1)
	{
		
		wdt_reset(); //still alive
	}
}


/*******************************************
This function holds the soldering station
from heating and forces the user to power 
cycle it
*******************************************/
void on_watchdog_reset(void)
{
	if(MCUSR & (1 << WDRF)){ //stay in ERROR-mode when reseted by watchdog until power cycle
		wdt_enable(WDTO_8S);
		MCUSR &= ~(1 << WDRF);
		DDRD |= (1 << PD4);
		PORTD &= ~(1 << PD4); //make sure tip is off
		config_load();
		display_init();
		clock_init();
		uart_init(19200, one_stop_bit_e, no_parity_e);
		sei();
		while(1){ // give a hint that a power cycle is necessary
			wdt_reset();
			printf("ERROR: please power cycle this device\r\n");
			display_sign(2,SIGN_E);
			display_sign(1,SIGN_R);
			display_sign(0,SIGN_R);
			_delay_ms(700);
			display_sign(2,SIGN_T);
			display_sign(1,SIGN_R);
			display_sign(0,SIGN_N); 
			_delay_ms(700);
			display_sign(2,SIGN_O);
			display_sign(1,SIGN_F);
			display_sign(0,SIGN_F); 
			_delay_ms(700);
			display_sign(2,SIGN_A);
			display_sign(1,SIGN_N);
			display_sign(0,SIGN_D); 
			_delay_ms(700);
			display_sign(2,0);
			display_sign(1,SIGN_O);
			display_sign(0,SIGN_N); 
			_delay_ms(700);
		}
	}
}

