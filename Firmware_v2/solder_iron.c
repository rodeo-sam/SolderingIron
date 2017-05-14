#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global_vars.h"
#include "config.h"

#include "display.h"
#include "adc.h"
#include "temperature.h"
#include "uart.h"
#include "buttons.h"
#include "tip.h"
#include "timer0.h"


/******************VERY IMPORTANT********************************************
 * this code block is necessary to prevent the processor to run into watchdog-reset-lifelock
 * do NOT delete this block
****************************************************************************/
//void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
//
//void wdt_init(void)
//{
//	wdt_reset();
//	wdt_enable(WDTO_8S);
//    return;
//}
/*******************end of very important block***************************/




int main(void)
{
	global_timer0_trigger = 0;
	global_targetTemperature = 320;
	global_currentTemperature = 0;
	global_showTargetTemperature = 625; // 5 sec (in counts of 8ms)

	uart_init();

	adc_init();

	//on_watchdog_reset();
	//wdt_enable(WDTO_500MS);


	buttons_init(); // enable buttons

	display_init(); // setting up the display

//	tip_init();
//	tip_enable();

	timer0_init(); // sets global_timer0_trigger to 1 every 8ms

	//control_init();  // this heats things up!

	while(1)
	{
		if(global_timer0_trigger) {
			if(global_showTargetTemperature < 625) {
				global_showTargetTemperature++;
				display_number(global_targetTemperature);
			} else {
				display_number(global_currentTemperature);
			}
			display_update();
		}
		global_timer0_trigger = 0; // reset trigger
		//wdt_reset(); //still alive
	}
}


/*******************************************
This function holds the soldering station
from heating and forces the user to power
cycle it
*******************************************/

/*
void on_watchdog_reset(void)
{
	if(MCUSR & (1 << WDRF)){ //stay in ERROR-mode when reseted by watchdog until power cycle
		//wdt_enable(WDTO_8S);
		MCUSR &= ~(1 << WDRF);
		DDRD |= (1 << PD4);
		PORTD &= ~(1 << PD4); //make sure tip is off
		config_load();
		display_init();
		clock_init();
		//uart_init(19200, one_stop_bit_e, no_parity_e);
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
}*/
