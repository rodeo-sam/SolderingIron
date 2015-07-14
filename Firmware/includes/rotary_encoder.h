/*
* @file: rotary_encoder.h
*
* Reads rotary encoder every 2ms and calls plus/minus/button-callbacks
*
* Author: Keno Garlichs, <k.garlichs@tu-braunschweig.de>              
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "timing.h"

// Configure pins here
#define ROTARY_A_PIN     PINC
#define ROTARY_B_PIN     PINC
#define ROTARY_BUTTON_PIN		PINC

#define ROTARY_A_PIN_NR     PC2
#define ROTARY_B_PIN_NR     PC3
#define ROTARY_BUTTON_PIN_NR	PC4

#define ROTARY_A_PORT     PORTC
#define ROTARY_B_PORT     PORTC
#define ROTARY_BUTTON_PORT		PORTC

#define ROTARY_A_DDR     DDRC
#define ROTARY_B_DDR     DDRC
#define ROTARY_BUTTON_DDR		DDRC

#define ROTARY_A     (ROTARY_A_PIN & (1 << ROTARY_A_PIN_NR))
#define ROTARY_B     (ROTARY_B_PIN & (1 << ROTARY_B_PIN_NR))
#define ROTARY_BUTTON_PRESSED		!(ROTARY_BUTTON_PIN & (1 << ROTARY_BUTTON_PIN_NR))

#define ROTARY_A_STATE		(ROTARY_A >> 1)		// Shifts ROTARY_A bit to bit 1
#define ROTARY_B_STATE		(ROTARY_B >> 3)		// Shifts ROTARY_B bit to bit 0

extern void (*plus_callback)(void);
extern void (*minus_callback)(void);
extern void (*third_callback)(void);
extern void (*third_long_callback)(void);

void rotary_callback(void);
void encode_init(void (*p_callback)(void), void (*m_callback)(void), void (*t_callback)(void), void (*tl_callback)(void));
void check_rotary_timer(void);
