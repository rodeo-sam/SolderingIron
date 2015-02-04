/*
 * tip.c 
 *
 * Created:  Mon Feb  2 22:32:48 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>

static int16_t tip_temperature = 25; //off on startup

//TODO fill with usefull code

void tip_init(void(*callback)(void))
{
	//TODO remove it
	return;
}

void set_tip_temp()
{
	return g_tip_temperature;
}

int16_t get_tip_temp(void)
{
	return g_tip_temperature;
}

void tip_start_conversion(void)
{
	adc_configuration_t conf = {
		.channel = CH_ADC0,
		.reference = REF_VCC,
		.trigger = TRGR_MANUEL,
	};
	adc_init(conf);
	adc_run();
	return;
}
