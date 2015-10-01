/*
 * grip.c
 *
 * Created:  Tue Sep 29 14:00:00 2015
 * Author: Johannes van Balen <johannes@van-balen.de>
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"

void grip_init(void) {
	DDRC &= ~(1 << PC1); // set PC1 as input
	PORTC |= (1 << PC1); // use pullup for PC1
}

uint8_t grip_getState(void) {
	if (PINC & (1 << PC1) && GRIP_ENABLE) {
		return 1;
	}
	return 0;
}
