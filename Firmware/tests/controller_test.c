/*
 * controller_test.c 
 *
 * Created:  Mon Feb  2 18:42:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <util/delay.h>
#include "display.h"
#include "../config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"
#include "controller.h"
#include "uart.h"
#include "random.h"
#include "clock.h"

static int16_t count = 27;
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


	int16_t countt = count;
	while(1)
	{
		display_updater();
		generate_random();
		if(count != countt){
			display_number(count);
			countt = count;
			control_set_temp(count);
		}
	}
}
