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

static int16_t count = 350;
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

	control_init();


	int16_t countt = count;
	while(1)
	{
		//	display_number(count);
		if(count != countt){
			countt = count;
			control_set_temp(count);
		}
		_delay_ms(20);
	}
}
