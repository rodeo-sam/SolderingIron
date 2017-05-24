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
#include "config.h"
#include "adc.h"
#include "tip.h"
#include "uart.h"



void tip_init(void) {
	PWR0_DDR |= (1<< PWR0); // set heating pin as output
	tip_powerOff();
  unsigned char buffer[17] = "TIP_CTRL - INIT\r\n";
  uart_transmit(buffer, 17);
}


void tip_powerOn(void) {
	if (global_tip_enabled) {
		PWR0_PORT |= (1 << PWR0);
	}
}


void tip_powerOff(void) {
	PWR0_PORT &= ~(1 << PWR0);
}


void tip_disable(void) {
	global_tip_enabled = 0;
	tip_powerOff(); // also power off
}


void tip_enable(void) {
	global_tip_enabled = 1;
}
