/*
 * controller_test.c 
 *
 * Created:  Mon Feb  2 18:42:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "buttons.h"
#include "controller.h"
#include "uart.h"
#include "random.h"
#include "clock.h"
#include "timing.h"
#include "tip.h"
#include "eeprom-config.h"

static int16_t count = 250;
void plus(void)
{
  if (count != TEMP_MAX) {
  	count++;
    
  }
}

void minus(void)
{
  if (count != 0) {
    count--;
  }
}

int main(void)
{
	config_load();
	display_init();

	buttons_init(&plus, &minus,0,0);
	clock_init();
	uart_init(19200, one_stop_bit_e, no_parity_e);
	control_init();
	control_set_temp(count);
	printf("hallo\r\n");

	next_time_t timer;
	timer_init(&timer,1,0,0); // 1s
	timer_prepare();
	timer_set(&timer);

	int16_t countt = count;
	while(1)
	{
		generate_random();
		if(count != countt){
			display_number(count);
			countt = count;
			control_set_temp(count);
		}
		if(timer_past(&timer)){ //every 1s
			timer_set(&timer); 
			display_temperature(tip_get_temp());
			printf("soll %d, temp: %d tip: %d\r\n",count,tip_get_temp(),tip_state());
		}
	}
}
