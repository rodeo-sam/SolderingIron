/*
 * buttons_test.c 
 *
 * Created:  Mon Feb  2 00:15:18 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "../config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"

static int count = 0;
void plus(void)
{
  if (count != TEMP_MAX) {
  	count++;
    
  }
}

void minus(void)
{
  if (count != TEMP_MIN) {
    count--;
  }
}

void zero(void)
{
	count=0;
}
void one(void)
{
	count=1;
}


int main(void)
{
	display_init();

	buttons_init(&plus, &minus, &zero, &one);

	while(1)
	{
		display_number(count);
		_delay_ms(20);
	}
}
