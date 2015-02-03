
/*
 * test-display.c 
 *
 * Created:  Sun Feb  1 17:38:58 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "config.h"
#include <stdio.h>
#include "uart.h"

// a. redirect stdout (part 1)
static FILE mystream = FDEV_SETUP_STREAM( put, get, _FDEV_SETUP_RW );

int main(void)
{
	uart_init(19200, one_stop_bit_e, no_parity_e);
    // b. redirect stdout (part 2)
    stdin=stdout = &mystream;
	display_init();

	display_digit(1,3);
	printf("\n\rhello ");
	printf_P(PSTR("world\r\n "));

	printf("enter your name: ");
	char name[20];
	scanf("%s",name);
	printf("%s\r\n",name);
	while(1)
	{
		printf(".");
		_delay_ms(400);
	}
}
