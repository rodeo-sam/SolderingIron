/*
 * eeprom-config.c
 *
 * Created: Sa 31. Jan 2015 
 * Author:  Georg von Zengen <g@vz-ge.org>
 */ 

#include <avr/eeprom.h>

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

