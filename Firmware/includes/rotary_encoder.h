/*
* @file: rotary_encoder.h
*
* Reads rotary encoder every 2ms and calls plus/minus/button-callbacks
*
* Author: Keno Garlichs, <k.garlichs@tu-braunschweig.de>
*/
#pragma once

#include <avr/io.h>
#include <stdio.h>
#include "timing.h"

#define ROTARY_A (ROTARY_A_PIN & (1 << ROTARY_A_PIN_NR))
#define ROTARY_B (ROTARY_B_PIN & (1 << ROTARY_B_PIN_NR))
#define ROTARY_BUTTON_PRESSED !(ROTARY_BUTTON_PIN & (1 << ROTARY_BUTTON_PIN_NR))

#define ROTARY_A_STATE (ROTARY_A >> (ROTARY_A_PIN_NR - 1))		// Shifts ROTARY_A bit to bit 1
#define ROTARY_B_STATE (ROTARY_B >> ROTARY_B_PIN_NR)		// Shifts ROTARY_B bit to bit 0

extern void (*plus_callback)(void);
extern void (*minus_callback)(void);
extern void (*third_callback)(void);
extern void (*third_long_callback)(void);

void rotary_callback(void);
void encode_init(void (*p_callback)(void), void (*m_callback)(void), void (*t_callback)(void), void (*tl_callback)(void));
void check_rotary_timer(void);
