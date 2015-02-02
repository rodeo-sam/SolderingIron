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




config_t *conf;

int main(void)
{
	display_init();

	//display_digit(1,3);
	conf = config_load();
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
		conf->pid_d = 0.1;
		conf->pid_i = 0.1;
		conf->pid_p = 0.1;
		conf->default_temp = 350;
		conf->temp_offset = 50;
		_delay_ms(1000);
	}
	uint16_t num = config_save();
	display_number(num);
	_delay_ms(200);


	while(1)
	{
		display_fixed_point((uint16_t)(conf->pid_p * 10),-1);
		_delay_ms(300);		
		display_fixed_point((uint16_t)(conf->pid_i * 10),-1);
		_delay_ms(300);		
		display_fixed_point((uint16_t)(conf->pid_d * 10),-1);
		_delay_ms(300);
		display_number(conf->default_temp);
		_delay_ms(300);
		display_number(conf->temp_offset);
		_delay_ms(300);
	}
}
