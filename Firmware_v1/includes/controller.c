/*
 * controller.c 
 *
 * Created:  Sat Jan 31 23:03:24 2015
 * Author: Georg von Zengen < oni303@gmail.com>
 *         Yannic Schröder <yannic@dontclickthis.de>
 *
 *
 * WARNING: messing around in this file may cause damage to the tip.
 * This code comes WITHOUT any warranty at all.
 * The author is not responsible for any harm it causes to you, people surrounding you, or anything else.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "eeprom-config.h"
#include "tip.h" 
#include "timer0.h"
#include "uart.h"

#define Ta 100 //ms  //control cycle time
#define Trand ((Ta*32)/255)

#define MAXESUM 10 //*Ta ms  // maximum of error integration most not be too high because the tip heats very fast.
#define MINESUM (-1*MAXESUM)

static int16_t esum = 0;
static int16_t eold = 0;

static volatile uint8_t pwm_value = 0;					// current heating value (0-255)

static uint16_t pwm_count = 0;							// counter for the software pwm
static volatile uint8_t measurement_finished = 0;

static void control(int16_t temp);

void new_temperature_ready_callback(int16_t temp){
	control(temp);				//calculate a new y
	measurement_finished = 1;	//temperature measurement is finished
}

void control_init(void)
{
	OCR0A = Trand;
	TCCR0A |= (1 << WGM01); //CTC mode
	TIMSK0 |= (1 << OCIE0A) ;
	tip_init(&new_temperature_ready_callback);
	tip_startConversion();
	timer0_init();
	printf("controller booted\r\n");
}

static void control(int16_t temp)
{
	// current error is target temperature minus measured temperature
	int16_t e = tip_getTargetTemp() - temp;

	// prevent esum over- and underflows
	if(esum >= (int32_t)(MAXESUM - e)){
		esum = MAXESUM;
	} else if(esum <= (int32_t)(MINESUM + e)){
		esum = (MINESUM);
	}
	// make sure esum is not increased if maximum heating is reached to prevent overshooting
	else if(((pwm_value > 0) && (pwm_value < 255) ) || ((pwm_value == 255) && (e < 0)) || ((pwm_value == 0) && (e > 0))) {
		esum += e;
	}
	
	// calculate P, I and D values
	int16_t p = ((int32_t)((int32_t)config.pid_p*(int32_t)e))/(int32_t)128;
	int16_t i = (int32_t)((int32_t)config.pid_i*(int32_t)Ta*(int32_t)esum)/(uint32_t)128;
	int16_t d = (int32_t)(config.pid_d*(e - eold)/Ta)/(uint32_t)128;

	// add P, I and D for new output value
	int16_t y = (int32_t)((int32_t)p + (int32_t)i + (int32_t)d);

	// update error
	eold = e;

	// clamp y between 0 and 255 for the pwm value
	if(y > 255){
		y = 255;
	} else if (y < 0){
		y = 0;
	}

	// only heat if tip is under maximum temperature
	if (temp > TEMP_MAX) {
		y = 0;
	}

	// store new value for the pwm loop
	pwm_value = y;
}

ISR(TIMER0_COMPA_vect) {
	uint8_t increment = 1; // sometimes the counter is not incremented in the current cycle

	// power tip on timer overflow if the temperature is already measured and the duty cycle is over 2 (avoids a clicking noise of the tip)
	if( (pwm_count == 0) && measurement_finished && (pwm_value > 2) ){
		measurement_finished = 0;
		tip_powerOn();
	}
	// if the timer overflows and the measurement is not yet finished wait and do nothing (should not happen)
	else if( (pwm_count == 0) && !measurement_finished ){
		increment = 0; //make sure pwm_count is not increased (clean overflow)
	}
	// unpower tip if duty cycle is over
	else if (pwm_count == pwm_value) {
			tip_powerOff();
	}
	// measure temperature some time after unpowering to allow the voltage to settle
	// powering the tip saturates the opamp, so measurement is only possible when not powered
	else if (pwm_count == pwm_value + 50) {
			tip_startConversion();
	}
	// start a new cycle if measurement is finished  and the cycle is over by overflowing the counter manually
	else if( measurement_finished && (pwm_count >= 255) ){
		pwm_count = 0; // emulate a timer overflow
		increment = 0; // make sure pwm_count is not increased (clean overflow)
	}

	// only increment the counter if needed
	if (increment) {
		pwm_count++;
	}
}

