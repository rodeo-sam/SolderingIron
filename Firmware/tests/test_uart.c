
/*
 * test-display.c 
 *
 * Created:  Sun Feb  1 17:38:58 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "config.h"
#include "uart.h"

int main(void)
{
	uart_init(19200, one_stop_bit_e, no_parity_e);
	display_init();

	display_digit(1,3);

	uart_sendString_P(PSTR("Pizza?\n\r(y/n)"));
	while (uart_getByte()!='y');
	while(1)
	{
		uart_sendString("Pizza\n\r");
		_delay_ms(400);
	}
}
