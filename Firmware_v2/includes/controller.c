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
#include <stdint.h>

#include "config.h"
#include "global_vars.h"
#include "uart.h"


#define Ta 40 //ms  //control cycle time
#define Trand ((Ta*32)/255)

#define MAXESUM 10 //*Ta ms  // maximum of error integration must not be too high because the tip heats very fast.
#define MINESUM (-1*MAXESUM)

static int16_t esum = 0;
static int16_t eold = 0;
static uint8_t pwm_value = 0;




uint8_t controller_calculate(int16_t target_temperature, int16_t current_temperature) {
	// current error is target temperature minus measured temperature
	int16_t e = target_temperature - current_temperature;

	// prevent esum over- and underflows
	if(esum >= (int32_t)(MAXESUM - e)){
		esum = MAXESUM;
	} else if(esum <= (int32_t)(MINESUM + e)){
		esum = (MINESUM);
	}
	// make sure esum is not increased if maximum heating is reached to prevent overshooting
	else if(((pwm_value > 0) && (pwm_value < MAX_PWM_VALUE) ) || ((pwm_value == MAX_PWM_VALUE) && (e < 0)) || ((pwm_value == 0) && (e > 0))) {
		esum += e;
	}

	// calculate P, I and D values
	int16_t p = ((int32_t)((int32_t)PID_P*(int32_t)e))/(int32_t)128;
	int16_t i = (int32_t)((int32_t)PID_I*(int32_t)Ta*(int32_t)esum)/(int32_t)128;
	int16_t d = (int32_t)(PID_D*(e - eold)/Ta)/(int32_t)128;

	// add P, I and D for new output value
	int16_t y = (int32_t)((int32_t)p + (int32_t)i + (int32_t)d);

	// update error
	eold = e;

	// clamp y between 0 and 255 for the pwm value
	if(y > MAX_PWM_VALUE){
		y = MAX_PWM_VALUE;
	} else if (y < 0){
		y = 0;
	}

	// store new value for the pwm loop
	pwm_value = y;

  //unsigned char buffer2[7] = "   ;   ";
  //buffer2[0]=(current_temperature/100)+48;
  //buffer2[1]=((current_temperature/10)%10)+48;
  //buffer2[2]=(current_temperature%10)+48;
  //buffer2[4]=(y/100)+48;
  //buffer2[5]=((y/10)%10)+48;
  //buffer2[6]=(y%10)+48;
  //uart_transmit(buffer2, 7);

  return (uint8_t)y;
}
