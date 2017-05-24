/*
 * buttons.c
 *
 * Created:  Sun Feb  1 23:55:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 * 		   Keno Garlichs <k.garlichs@tu-braunschweig.de>
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "global_vars.h"
#include "config.h"
#include "buttons.h"
#include "uart.h"


/*
 * Initialize buttons and the corresponding interrupts
 */
void buttons_init(void)
{
	// Set portpins for buttons as input
	DDRD &= ~(1 << PD3) & ~(1 << PD2);

	// Disable internal pullups since we have them in hardware already
	PORTD &= ~(1 << PD3) & ~(1 << PD2);

	// interrupts
	EICRA |= (1 << ISC11) | (1 << ISC01); //falling edge generates interrupt
	EIMSK |= (1 << INT0)  | (1 << INT1); //enable interrupts

  unsigned char buffer[16] = "BUTTONS - INIT\r\n";
  uart_transmit(buffer, 16);
}
