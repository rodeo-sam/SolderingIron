/*
 * tip.h 
 *
 * Created:  Mon Feb  2 22:31:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 
#pragma once
void tip_conversion_complete_callback(uint16_t adc);
void tip_init(void(*callback)(int16_t));
int16_t tip_get_temp(void);
void tip_start_conversion(void);
//uint8_t tip_present(int16_t powered, int16_t nopowered);
//uint8_t tip_state(void);
int16_t tip_setted_temp(void);
void tip_set_temp(int16_t t);

