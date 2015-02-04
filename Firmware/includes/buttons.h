/*
 * buttons.h 
 *
 * Created:  Mon Feb  2 00:10:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 * 		   Keno Garlichs <k.garlichs@tu-braunschweig.de>
 */ 

#pragma once

#define BUTTON_PORT PORTD
#define BUTTON_PORT_DDR DDRD
#define BUTTON_PIN PIND
#define PLUS_PIN PD2
#define MINUS_PIN PD3
#define PLUS_INTERRUPT_VECT INT0_vect
#define MINUS_INTERRUPT_VECT INT1_vect

extern void (*plus_callback)(void);
extern void (*minus_callback)(void);
extern void (*third_callback)(void);
extern void (*third_long_callback)(void);

void buttons_init(void (*p_callback)(void), void (*m_callback)(void), void (*t_callback)(void), void (*tl_callback)(void));
