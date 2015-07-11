/*
 * controller.c 
 *
 * Created:  Sat Jan 31 23:03:24 2015
 * Author: Georg von Zengen < oni303@gmail.com>
 *
 *
 * WARNING: messing around in this file may cause damage to the tip.
 * This code comes WITHOUT any warranty at all.
 * The author is not responsible for any harm it causes to you, people surrounding you, or anything else.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "eeprom-config.h"
#include "tip.h" 
#include "timer0.h"
#include "uart.h"

#include "pinout.h"
#include "config.h"

#define Ta 100 //ms  //controll cycle time
#define Trand ((Ta*32)/255)

#define TIP_CHECK_INTERVAL 10

#define MAXESUM 10 //*Ta ms  // maximum of error integration most not be too high because the tip heats very fast.
#define MINESUM (-1*MAXESUM)

volatile int16_t w = 0;

volatile int16_t esum = 0;
volatile int16_t eold = 0;
volatile int16_t y_buffer = 255;
volatile int16_t temp_no_power = 0;
volatile int16_t temp_power = 0;
//volatile uint8_t tip = 0;
//volatile uint8_t no_tip_count = 0;
void new_temperature_ready_callback(int16_t);

void control_set_temp(int16_t temp)
{
	w = temp;
}

void control_init(void)
{
	OCR0A = Trand;
	TCCR0A |= (1 << WGM01); //CTC mode
	TIMSK0 |= (1 << OCIE0A) ;
	tip_init(&new_temperature_ready_callback);
	tip_start_conversion();
	PWR0_DDR |= (1<< PWR0);
	w = config.default_temp;
	timer0_init();
	printf("controller booted\r\n");
}
void control(int16_t temp)
{
	int16_t e = w - temp; 
	//guarding preventing esum over/underflows
	if(esum >= (int32_t)(MAXESUM - e)){
		esum = MAXESUM;
	} else if(esum <= (int32_t)(MINESUM + e)){
		esum = (MINESUM);
	}else{
		//make sure esum is not increased if maximum heating is reached, prevents overshooting
		if(((y_buffer > 0) && (y_buffer < 255) ) || ((y_buffer == 255) && (e < 0)) || ( (y_buffer == 0) && (e > 0))){
			esum += e;
		}
	}
	
	
	int16_t p = ((int32_t)((int32_t)config.pid_p*(int32_t)e))/(int32_t)128;
	int16_t i = (int32_t)((int32_t)config.pid_i*(int32_t)Ta*(int32_t)esum)/(uint32_t)128;
	int16_t d = (int32_t)(config.pid_d*(e - eold)/Ta)/(uint32_t)128;
	int16_t y = (int32_t)((int32_t)p + (int32_t)i + (int32_t)d);
	eold = e;

	//guarding values higher then 255 are not possible 
	if(y > 255){
		y = 255;
	} else if (y < 0){
		y = 0;
	}

	if(temp > TEMP_MAX){ //guard tip form overheating
		y = 0;
	}

//	if (tip == 0){
//		y_buffer = 0; //do not heat if no tip is connected
// 		no_tip_count++;
//		if (no_tip_count > TIP_CHECK_INTERVAL){ //check if tip is connected now
//			tip = 1;
//			no_tip_count = 0;
//			y_buffer = y;
//		}
//	}else{
//		y_buffer = y;
//	}

	y_buffer = y;
}

uint16_t pwm_count =0;
uint8_t compb_state = 0;
volatile uint8_t measurement_state = 0;
volatile uint8_t overflow = 0;
void new_temperature_ready_callback(int16_t temp){
	if (measurement_state == 0){
		control(temp); //calculating a new y
		compb_state = 1; //temperature is measured
		temp_no_power = temp;
	} else {
		temp_power = temp;
		//tip = tip_present(temp_power, temp_no_power);
	}
}

ISR(TIMER0_COMPA_vect) {
	uint8_t t = 0;
	if((pwm_count == 0) && compb_state && (y_buffer > 2) ){
		compb_state = 0;     
		PWR0_PORT |= (1 << PWR0);  //set pin high on timer overflow if the temperature is already measured and the duty cycle is over 2 (noise reduction)
	}else if((pwm_count == 1) && (y_buffer == 0)){
		PWR0_PORT &= ~(1 << PWR0); // just to be sure the power is off
		compb_state = 0;
		measurement_state = 0;
		tip_start_conversion(); // measure the temperature if as soon as possible if the duty cycle is zero
	} else if((pwm_count == 0) && !compb_state ){
		pwm_count = 0; // wait for temperature measurement
		t = 1; //make sure pwm_count is not increased (clean overflow)
	} else {
		if(pwm_count == y_buffer ){ 
			PWR0_PORT &= ~(1 << PWR0); //clear pin if duty cycle is over
//			if (overflow){ //tip detection
//				overflow = 0;
//				measurement_state = 1;
//				tip_start_conversion(); // measure direcly after tip is powered down for tip detection
//			}
		}else if((pwm_count == y_buffer + 50)){
			measurement_state = 0;
			tip_start_conversion(); //start temperature measurement after waiting (50) for the offset voltage, caused by powering the tip, has faded
		}
	}
	if((compb_state == 1) && (pwm_count >= 255)){
		pwm_count = 0; //emulate a timer overflow
		overflow = 1;
		t = 1; //make sure pwm_count is not increased (clean overflow)
	}
	if(!t){ // do not increase pwm_count if the timer just overflew
		pwm_count++;
	}
}

