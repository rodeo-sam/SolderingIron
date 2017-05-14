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
#include "global_vars.h"
#include "adc.h"
#include "tip.h"
#include "config.h"

static uint8_t tip_enabled = 0;
static uint8_t tip_powered = 0;


void tip_init(void) {
	PWR0_DDR |= (1<< PWR0); // set heating pin as output
	tip_powerOff();
}


uint8_t tip_powerOn(void) {
	if (tip_enabled) {
		tip_powered = 1;
		PWR0_PORT |= (1 << PWR0);
		return 1;
	}
	return 0; // do not power if tip is disabled
}


uint8_t tip_powerOff(void) {
	tip_powered = 0;
	PWR0_PORT &= ~(1 << PWR0);
	return 1;
}


void tip_disable(void) {
	tip_enabled = 0;
	tip_powerOff(); // also power off
}


void tip_enable(void) {
	tip_enabled = 1;
}


void tip_updateTemperature(void) {
	global_currentTemperature = (int16_t) adc_read() * TEMP_CONVERSION_M + TEMP_CONVERSION_B;
	global_currentTemperature += 40; // hacked temperature offset
}
