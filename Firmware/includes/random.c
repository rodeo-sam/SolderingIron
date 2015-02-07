/*
 * random.c 
 *
 * Created:  Sat Feb  7 00:46:24 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 


#include <stdlib.h>
#include <avr/io.h>

uint8_t rand_a[3];
void generate_random(void)
{
	static uint8_t i = 0;
	i = (i+1)%3;
	rand_a[i] = (uint8_t)random();
}

uint8_t get_random(void)
{
	static uint8_t i = 0;
	i = (i+1)%3;
	return rand_a[i];
}
