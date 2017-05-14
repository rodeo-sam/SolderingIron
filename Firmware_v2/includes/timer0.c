/*
 * timer0.c
 *
 * Created:  Sun Feb  1 17:15:39 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */

#include <avr/io.h>

#include "global_vars.h"
#include "tip.h"
#include "uart.h"



void timer0_init(void)
{
	// timer 0
	// used for a 8ms cycle for everything but heating control
	TCCR0B |= (1 << CS02); // prescaler = 256 @ 8MHz -> ~122Hz -> 8ms
}

ISR(TIMER0_OVF_vect) {
	if (!global_timer0_trigger) {
		global_timer0_trigger = 1;
	} else {
		unsigned char buffer[42] = "TIMER0: LOOP OVERLAP ERROR - SHUTTING DOWN\r\n";
	  uart_transmit(buffer, 42);
		while(1) {
			tip_disable();
			// Let the watchdog bite!
		}
	}
}
