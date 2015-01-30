/*
 * types.c
 * Utility include that defines useful types
 *
 * Created: 2014-07-22 15:45:34
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#pragma once

#include <stdint.h>
#include "pinout.h"


void display_init();
void display_update();
void display_digit(uint8_t led_idx, uint8_t digit);
void display_number(int16_t number);
