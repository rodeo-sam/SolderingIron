
/*
 * test-display.c 
 *
 * Created:  Sun Feb  1 17:38:58 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "config.h"

int main(void)
{
	display_init();

	//display_digit(1,3);
		display_number(11);

	uint8_t count = 100;
	while(1)
	{
		count++;
		display_number(count);
		_delay_ms(500);
	}
}
