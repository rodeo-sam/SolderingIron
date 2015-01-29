/*
 * types.c
 * Utility include that defines useful types
 *
 * Created: 2014-07-22 15:45:34
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#pragma once

#include <stdint.h>

#define LED0 (1<<PC3)
#define LED1 (1<<PC4)
#define LED2 (1<<PC5)

void display_init();
void display_digit(uint8_t led, uint8_t digit);
