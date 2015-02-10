/*
 * eeprom-config.c
 *
 * Created: Sa 31. Jan 2015 
 * Author:  Georg von Zengen <g@vz-ge.org>
 */ 

#include <avr/eeprom.h>

#include "config.h"
#include "eeprom-config.h"



config_t config;

config_t* config_load(void)
{
	uint16_t i = 0;
	for(; i < sizeof(config); i++){
		*(((uint8_t *) &config) + i) = eeprom_read_byte((uint8_t*)i + CONFIG_OFFSET);
	}
	return &config;
}

int config_save(void)
{
	uint16_t i = 0;
	for(; i < sizeof(config); i++){
		eeprom_write_byte((uint8_t*)i + CONFIG_OFFSET, *(((uint8_t *) &config) + i));
	}
	return i;
}

void config_init(void)
{
	config_load();
	uint8_t initialized = 0;
	uint16_t i = 0;
	for(; i < sizeof(config_t); i++){
		if ( ((*((uint8_t*) &config) + i) != 0xff) && (*(((uint8_t*) &config) + i) != 0)){
			initialized = 1;
			break;
		}
	}
	if(!initialized){
		config.pid_d = PID_D;
		config.pid_i = PID_I;
		config.pid_p = PID_P;
		config.default_temp = DEFAULT_TEMP;
    config_save();
	}
}
