/*
 * buttons_test.c 
 *
 * Created:  Mon Feb  2 00:15:18 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "config.h"
#include "buttons.h"

int count = 0;
void plus(void)
{
	count++;
}
void minus(void)
{
	count--;
}

int main(void)
{
	display_init();

	buttons_init(&plus, &minus);

	while(1)
	{
		display_number(count);
		_delay_ms(20);
	}
}
