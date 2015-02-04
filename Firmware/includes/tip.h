/*
 * tip.h 
 *
 * Created:  Mon Feb  2 22:31:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

int16_t tip_temperature_from_adc(uint16_t adc);
void tip_init(void(*callback)(uint16_t));
int16_t get_tip_temp(void);
void tip_start_conversion(void);
