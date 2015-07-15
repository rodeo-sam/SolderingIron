/*
 * solder_iron.c
 *
 * Created: Do 29. Jan 14:23:00 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 * Author: Georg von Zengen <oni303@gmail.com>
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
#include "timing.h"
#include "buttons.h"
#include "menu.h"
#include "tip.h"
#include "clock.h"
#include "controller.h"

#ifdef ROTARY_ENCODER
	#include "rotary_encoder.h"
#endif


void on_watchdog_reset(void);
void from_menu(void);
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
int16_t temperature = 0;
int16_t temperature_save = 0;

void plus(void)
{
  temperature = tip_setted_temp();
  if (temperature != TEMP_MAX) {
  	temperature++;
	display_temperature(temperature);
	tip_set_temp(temperature);
  }
}

void minus(void)
{
  temperature = tip_setted_temp();
  if (temperature != TEMP_MIN) {
    temperature--;
	display_temperature(temperature);
	tip_set_temp(temperature);
  }
}

void back_to_default(void)
{
	temperature = config.default_temp;
	tip_set_temp(temperature);
	display_temperature(temperature);
}

void goto_menu(void)
{
	control_set_temp(0);
	menu_init(&from_menu);
}
void from_menu(void)
{
	buttons_init(&plus, &minus, &back_to_default, &goto_menu);
#ifdef ROTARY_ENCODER
	encode_init(&plus, &minus, &back_to_default, &goto_menu);
#endif
	control_set_temp(tip_setted_temp());

}

int main(void)
{
	int16_t old_temp = 0;
	on_watchdog_reset();
	wdt_enable(WDTO_500MS);
	config_init();
	config_load();
	config_reset();	// make sure to use default settings for now
	display_init();
	clock_init();
	buttons_init(&plus, &minus, &back_to_default, &goto_menu);
#ifdef ROTARY_ENCODER
	encode_init(&plus, &minus, &back_to_default, &goto_menu);
#endif
	uart_init(19200, one_stop_bit_e, no_parity_e);

	next_time_t new_temp_timer;
	timer_init(&new_temp_timer,1,0,0); // 1s
	timer_prepare();
	next_time_t temp_timer;
	timer_init(&temp_timer,0,32,0); // 0.25s
	timer_prepare();
	timer_set(&temp_timer);

	temperature = config.default_temp;
	tip_set_temp(temperature);
	control_set_temp(temperature);
	
	control_init();  // this heats things up!


	uint8_t temp_to_show = 0;
	display_sign(2, SIGN_H);
	display_sign(1, SIGN_I);
	display_sign(0, SIGN_EXCLAMATION);
	printf("booted\r\n");
	while(1)
	{

		if (!in_menu){
			if (old_temp != temperature){
				old_temp = temperature;
				control_set_temp(temperature);
				timer_set(&new_temp_timer);
				temp_to_show = 1;
			}
			if(timer_past(&new_temp_timer)){
				temp_to_show = 0;
				timer_set(&temp_timer);
			}
			if(temp_to_show == 0){
				if(timer_past(&temp_timer)){
					timer_set(&temp_timer);
					display_temperature(tip_get_temp());
				}
			}
		}

#ifdef ROTARY_ENCODER
		check_rotary_timer();
#endif

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

void set_temperatur(int16_t temp)
{
	temperature_save = temp;
}
