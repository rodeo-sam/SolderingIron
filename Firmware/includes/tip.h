/*
 * tip.h 
 *
 * Created:  Mon Feb  2 22:31:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

void tip_init(void(*callback)(void));
int16_t get_tip_temp(void);
void tip_start_conversion(void);
