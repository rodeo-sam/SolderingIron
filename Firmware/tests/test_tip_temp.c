
/*
 * test_tip_temp.c 
 *
 * Created: Mi 4. Feb 16:53:09 CET 2015
 * Author: Karsten Hinz <k.hinz@tu-bs.de>
 */ 

#include <util/delay.h>
#include "display.h"
#include "config.h"
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "temperature.h"
#include "tip.h"
#include <avr/interrupt.h>

// a. redirect stdout (part 1)
static FILE mystream = FDEV_SETUP_STREAM( put, get, _FDEV_SETUP_RW );

void show_adc(uint16_t raw) {
	display_number(raw);
	printf("%i\r\n", raw);
}

int main(void)
{
	uart_init(19200, one_stop_bit_e, no_parity_e);
	// b. redirect stdout (part 2)
	stdin=stdout = &mystream;
	display_init();
	tip_init(&show_adc);

	display_digit(1,4);

	sei();
	printf_P(PSTR("\r\nSolder Tip Temperature test\r\n "));

	tip_start_conversion();


	while(1)
	{
		display_update();
		_delay_ms(2);
	}
}
