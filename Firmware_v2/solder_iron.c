#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global_vars.h"
#include "config.h"

#include "controller.h"
#include "display.h"
#include "adc.h"
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
	global_display_main_cycle_flag = 0;
  global_tip_enabled = 0;
	global_targetTemperature = MIN_TEMP;
	global_currentTemperature = 0;
	global_showTargetTemperature = TARGET_TEMP_SHOW_TIME;
  global_buttons = 0;
  global_adc_value = 0;
  global_cycle_counter = 0;
  global_pwm_value = 0;

	uart_init();

	adc_init();

	//on_watchdog_reset();
	//wdt_enable(WDTO_500MS);


	buttons_init(); // enable buttons

	display_init(); // setting up the display

	tip_init();
	tip_disable();
  ;//tip_enable();


	timer0_init();

	//control_init();  // this heats things up!

	sei();


  DDRC |= (1<<PC2); // DEBUG
  PORTC &= ~(1<<PC2); // DEBUG


	while(1)
	{

    if (global_buttons & 0x01) { // MINUS BUTTON PRESSED
      if (global_targetTemperature > MIN_TEMP) {
        global_targetTemperature -= 10;
        global_showTargetTemperature = 0; // reset time to show digits
        unsigned char buffer1[23] = "TARGET TEMP DECREASED\r\n";
        uart_transmit(buffer1, 23);
      }
      global_buttons &= ~(0x01); // reset flag
    }

    if (global_buttons & 0x02) { // PLUS BUTTON PRESSED
      if (global_targetTemperature < MAX_TEMP) {
        global_targetTemperature += 10;
        global_showTargetTemperature = 0; // reset time to show digits
        unsigned char buffer2[23] = "TARGET TEMP INCREASED\r\n";
        uart_transmit(buffer2, 23);
      }
      global_buttons &= ~(0x02); // reset flag
    }

		if(global_display_main_cycle_flag == 1) { // CYCLE EVERY 8,16ms
      PORTC |= (1<<PC2); // DEBUG


      // CALCULATE TEMPERATURE
      if (global_cycle_counter == 0) {
        OCR0A = END_COUNT - 10; // safety!!

        // read temperature
        int32_t temperature = ((((int32_t)adc_read()) * TEMP_CONVERSION_M) + TEMP_CONVERSION_B)/TEMP_CONVERSION_D;
    	  global_currentTemperature += (temperature+40)*3; // hacked temperature offset by +40 :D
        global_currentTemperature /= 4; // make recent measurement more important

        // calc temperature
        global_pwm_value = controller_calculate(global_targetTemperature, global_currentTemperature);
        OCR0A = END_COUNT - global_pwm_value;
        // if point to turn on tip already passed, just switch it on.
        if (TCNT0 > OCR0A) {
          tip_powerOn();
        }
      }


      if (global_currentTemperature < global_targetTemperature && global_cycle_counter < (MEASUREMENT_EVERY_N_CYCLES-1)) {
        tip_enable();
      } else {
        tip_disable();
      }


      //unsigned char buffer2[9] = "   ;   ; ";
      //buffer2[0]=(global_currentTemperature/100)+48;
      //buffer2[1]=((global_currentTemperature/10)%10)+48;
      //buffer2[2]=(global_currentTemperature%10)+48;
      //buffer2[4]=(OCR0A/100)+48;
      //buffer2[5]=((OCR0A/10)%10)+48;
      //buffer2[6]=(OCR0A%10)+48;
      //buffer2[8]=global_tip_enabled+48;
      //uart_transmit(buffer2, 9);


      // REFRESH DISPLAY
			if(global_showTargetTemperature < TARGET_TEMP_SHOW_TIME) {
				global_showTargetTemperature++;
				display_set_number(global_targetTemperature);
			} else {
        if (global_currentTemperature > MAX_TEMP) {
          display_show_tip();
        //} else if (global_currentTemperature < MIN_TEMP) {
        //  display_show_bars();
        } else {
		      display_set_number(global_currentTemperature);
        }
			}
			display_update();











      // RESET CYCLE FLAG
      global_display_main_cycle_flag = 0;
      global_cycle_counter = (global_cycle_counter+1)%5;
      PORTC &= ~(1<<PC2); // DEBUG
		}
	}
}


ISR(TIMER0_OVF_vect) {
  // TRIGGER MAIN CYCLE
	global_display_main_cycle_flag = 1;
}


ISR(TIMER0_COMPA_vect) {
  // TURN ON TIP
	tip_powerOn();
}


ISR(TIMER0_COMPB_vect) {
  // TURN OFF TIP
	tip_powerOff();
}


ISR(INT1_vect) // PLUS button
{
  global_buttons |= 0x01;
  //unsigned char buffer2[21] = "MINUS BUTTON PRESSED\r\n";
  //uart_transmit(buffer2, 21);
}


ISR(INT0_vect) // MINUS button
{
  global_buttons |= 0x02;
  //unsigned char buffer2[22] = "PLUS BUTTON PRESSED\r\n";
  //uart_transmit(buffer2, 22);
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
