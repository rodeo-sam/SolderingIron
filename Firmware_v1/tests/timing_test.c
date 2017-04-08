/*
 * timing_test.c 
 *
 * Created:  Wed Feb  4 00:07:05 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "../config.h"
#include "clock.h"
#include "timing.h"

int main(void)
{
	display_init();
	clock_init();

	DDRD |= (1<<PD4);

	next_time_t timer;
	timer_init(&timer,0,1,1); // 10ms
	timer_prepare();
	timer_set(&timer);
	
	while(1)
	{
		_delay_ms(2);
		if(timer_past(&timer)){ //every 10ms
			timer_set(&timer); 
			PORTD ^= (1<<PD4); //toggle powerpin
		}
	}
}
