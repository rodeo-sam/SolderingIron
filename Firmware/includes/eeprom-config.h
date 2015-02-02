/*
 * eeprom-config.h
 *
 * Created: Sa 31. Jan 2015 
 * Author:  Georg von Zengen <g@vz-ge.org>
 */ 
#pragma once

#define CONFIG_OFFSET 0

typedef struct {
	uint16_t pid_p;
	uint16_t pid_i;
	uint16_t pid_d;
	uint16_t default_temp;
	uint16_t temp_offset;
} config_t;

extern config_t config;

/*! \brief loads the config form eeprom
  \return pointer to config struct
*/
config_t* config_load(void);

/*! \brief saves the config to eeprom
  \return number of saved bytes 
*/
int config_save(void);
