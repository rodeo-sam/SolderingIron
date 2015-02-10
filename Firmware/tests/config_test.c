/*
 * config_test.c
 *
 * Created: Sa 31. Jan 2015
 * Author: Georg von Zengen <g@vz-ge.org>
 *
 * Please start this test on a erased eeprom twice to verify whether the save and load function are working correctly
 */ 

#include <util/delay.h>
#include <avr/io.h>

#include "config.h"
#include "display.h"
#include "eeprom-config.h"
#include "buttons.h"

void erase(void){
	uint16_t i = 0;
	for(; i < sizeof(config_t); i++){
		*(((uint8_t*) &config) + i) = 0xff ;
	}
	uint16_t num = config_save();
	display_number(num);
	_delay_ms(1000);
}
	


config_t *conf;

int main(void)
{
	clock_init();
	display_init();
	conf = config_load();
	buttons_init(0,0,0,&erase);
	display_number(100);
	uint8_t initialized = 0;
	uint16_t i = 0;
	for(; i < sizeof(config_t); i++){
		if ( (*(((uint8_t*) conf) + i) != 0xff) && (*(((uint8_t*) conf) + i) != 0)){
			initialized = 1;
			break;
		}
	}
	if(!initialized){
		display_number(1000); //to display ERR
		conf->pid_d = PID_D;
		conf->pid_i = PID_I;
		conf->pid_p = PID_P;
		conf->default_temp = 300;
		_delay_ms(1000);
	}
	uint16_t num = config_save();
	display_number(num);
	_delay_ms(200);


	while(1)
	{
		display_number(conf->pid_p);
		_delay_ms(300);		
		display_number(conf->pid_i);
		_delay_ms(300);		
		display_number(conf->pid_d);
		_delay_ms(300);
		display_number(conf->default_temp);
		_delay_ms(300);
	}
}
