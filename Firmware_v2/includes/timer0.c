/*
 * timer0.c
 *
 * Created:  Sun Feb  1 17:15:39 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */

#include <avr/io.h>
#include "config.h"
#include "global_vars.h"
#include "tip.h"
#include "uart.h"



void timer0_init(void)
{
	// timer 0
	// used for a 8ms cycle for everything but heating control
	TCCR0B |= (1 << CS02); // prescaler = 256 @ 8MHz -> ~122Hz -> 8ms
  TIMSK0 |= (1 << OCIE0A); // enable compare match interrupt A
  TIMSK0 |= (1 << OCIE0B); // enable compare match interrupt B
  TIMSK0 |= (1 << TOIE0); // enable overflow interrupt
  OCR0B = END_COUNT; // Shut off tip
  unsigned char buffer[20] = "TIMER0 - INITIATED\r\n";
  uart_transmit(buffer, 20);
}
