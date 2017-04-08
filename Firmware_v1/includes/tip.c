/*
 * tip.c 
 *
 * Created:  Mon Feb  2 22:32:48 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 *         Karsten Hinz <k.hinz@tu-bs.de>
 *         Yannic Schröder <yannic@dontclickthis.de>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdfix.h>
#include "adc.h"
#include "eeprom-config.h"

#include "tip.h"
#include "uart.h"
#include "config.h"

static volatile int16_t tip_temperature = 25; //off on startup
static void (*new_temperature_ready_callback)(int16_t);

static uint8_t tip_enabled = 0;
static uint8_t tip_state = TIP_DISCONNECTED;
static uint8_t tip_powered = 0;

static uint8_t tip_was_powered = 0; // tracks if the tip was powered since the last cycle of update_state()

//uint8_t tip_connected = 0;
static int16_t target_temperature = 0;
void tip_init(void(*callback)(int16_t))
{
	new_temperature_ready_callback = callback;
	PWR0_DDR |= (1<< PWR0); // set heating pin as output
	return;
}

uint8_t tip_getState(void) {
	return tip_state;
}

uint8_t tip_powerOn(void) {
	if (!tip_enabled) { // do not power if tip is disabled))
		return 0;
	}
	if (tip_getState() == TIP_CONNECTED) { // only allow powering when tip is connected
		tip_powered = 1;
		tip_was_powered = 1;
		PWR0_PORT |= (1 << PWR0);
		return 1;
	}
	return 0;
}

uint8_t tip_powerOff(void) {
	tip_powered = 0;
	PWR0_PORT &= ~(1 << PWR0);
	return 1;
}

#define MAX_STATE_CNT 10 // wait for MAX_STATE_CNT control cycles
void updateState(uint16_t temp) {
	static uint16_t state_counter = 0;	// counts for how many cycles the state did not change
										// changing broken and disconnected state is only allowed if the temp was stable long enough
	static int16_t last_temp = -1;

	// initialize last temperature
	if (last_temp == -1) {
		last_temp = temp;
	}

	switch (tip_state) {
	case TIP_CONNECTED:
		if (temp > NO_TIP_TEMP) {
			tip_state = TIP_DISCONNECTED; // tip got disconnected
			break;
		}

		// check if temperature change is sane and change to TIP_BROKEN

		int16_t temp_delta = last_temp - temp;

		// if the temperature changes too fast, something must be wrong
		if (temp_delta > MAX_TEMP_PER_SECOND) {
			tip_state = TIP_BROKEN;
			break;
		}
		if (temp_delta < -MAX_TEMP_PER_SECOND) {
			tip_state = TIP_BROKEN;
			break;
		}

		// check if tip is able to heat up at all (detects broken heating elements)
		if (temp < tip_getTargetTemp() - 20) {	// if tip is 20 degree colder than the target temperature it should be heated
			if (tip_was_powered) {				// if the tip was powered in the last cycle
				static int16_t saved_temp = -1;
				if (state_counter == 0) { // initialize by saving the current temperature
					saved_temp = temp;
				}
				else if (state_counter == MAX_STATE_CNT) {	// check if the temperature has risen after one second
					if ( temp - saved_temp < MIN_TEMP_PER_SECOND) {
						tip_state = TIP_BROKEN;
					}
					state_counter = 0;
					break;
				}
				state_counter++;
			}
		}

		// broken thermocouples cannot be detected (but it does not heat up in that case)

		break;
	case TIP_DISCONNECTED:
		if (temp < NO_TIP_TEMP) {
			state_counter++;
		} else {
			state_counter = 0;
		}
		if (state_counter > MAX_STATE_CNT) {
			state_counter = 0;
			tip_state = TIP_CONNECTED;
		}
		break;
	case TIP_BROKEN:
		if (temp > NO_TIP_TEMP) {
			state_counter++;
		} else {
			state_counter = 0;
		}
		if (state_counter > MAX_STATE_CNT) {
			state_counter = 0;
			tip_state = TIP_DISCONNECTED;
		}
		break;
	}

	// save last temperature
	last_temp = temp;

	// reset powered info
	tip_was_powered = 0;
}

int16_t tip_getTemp(void)
{
	return tip_temperature;
}
void tip_setTargetTemp(int16_t temp)
{
	target_temperature = temp;
}
int16_t tip_getTargetTemp(void)
{
	return target_temperature;
}
	
void tip_disable(void) {
	tip_enabled = 0;
	tip_powerOff(); // also power off
}

void tip_enable(void) {
	tip_enabled = 1;
}

static void tip_conversionCompleteCallback(uint16_t adc)
{
	//TODO use lookup table
	accum temperature = adc * TEMP_CONVERSION_M + TEMP_CONVERSION_B;
	tip_temperature = (int16_t) temperature;
	tip_temperature += 40; // hacked temperature offset

	updateState(tip_temperature);

	new_temperature_ready_callback(tip_temperature);
}

void tip_startConversion(void)
{
	tip_powerOff(); // only measure when tip is powered off

	// wait if adc is busy
	while(adc_is_busy());

	//ensure adc init and start is done atomicly
	uint8_t sreg = SREG;
	cli();

	adc_configuration_t conf = {
		.channel = CH_ADC0,
		.reference = REF_VCC,
		.trigger = TRGR_MANUAL,
	};
	adc_init(conf);
	adc_set_conversion_complete_callback(tip_conversionCompleteCallback);
	adc_trigger();

	SREG = sreg;
	return;
}
