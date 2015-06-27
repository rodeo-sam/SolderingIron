/*
 * types.c
 * Utility include that defines useful types
 *
 * Created: 2014-07-22 15:45:34
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#pragma once

#include <stdint.h>
#include <avr/io.h>
#include "pinout.h"


typedef enum {
	SIGN_0=(DISP_BOT|DISP_UL|DISP_UR|DISP_TOP|DISP_BL|DISP_BR),
	SIGN_1=(DISP_BR|DISP_UR),
	SIGN_2=(DISP_TOP|DISP_UR|DISP_MID|DISP_BL|DISP_BOT),
	SIGN_3=(DISP_TOP|DISP_UR|DISP_MID|DISP_BR|DISP_BOT),
	SIGN_4=(DISP_UL|DISP_MID|DISP_UR|DISP_BR) ,
	SIGN_5=(DISP_TOP|DISP_UL|DISP_MID|DISP_BR|DISP_BOT),
	SIGN_6=(DISP_TOP|DISP_UL|DISP_MID|DISP_BL|DISP_BOT|DISP_BR),
	SIGN_7=(DISP_TOP|DISP_UR|DISP_BR) ,
	SIGN_8=(DISP_TOP|DISP_UR|DISP_BR|DISP_UL|DISP_BL|DISP_MID|DISP_BOT),
	SIGN_9=(DISP_TOP|DISP_UR|DISP_BR|DISP_UL|DISP_MID|DISP_BOT),
	SIGN_A=(DISP_TOP|DISP_UR|DISP_BR|DISP_UL|DISP_BL|DISP_MID),
	SIGN_B=(DISP_BR|DISP_UL|DISP_MID|DISP_BL|DISP_BOT),
	SIGN_C=(DISP_TOP|DISP_UL|DISP_BL|DISP_BOT),
	SIGN_D=(DISP_UR|DISP_BR|DISP_BL|DISP_MID|DISP_BOT),
	SIGN_E=(DISP_TOP|DISP_UL|DISP_BL|DISP_MID|DISP_BOT),
	SIGN_F=(DISP_TOP|DISP_UL|DISP_BL|DISP_MID),
	SIGN_G=(DISP_TOP|DISP_UL|DISP_BL|DISP_BOT|DISP_BR),
	SIGN_H=(DISP_UL|DISP_BL|DISP_MID|DISP_UR|DISP_BR),
	SIGN_I=(DISP_UL|DISP_BL),
	SIGN_J=(DISP_BOT|DISP_UR|DISP_TOP|DISP_BL|DISP_BR),
	SIGN_K=0, // not possible with this display
	SIGN_L=(DISP_UL|DISP_BL|DISP_BOT),
	SIGN_M=0, // not possible with this display
	SIGN_N=(DISP_BL|DISP_MID|DISP_BR),
	SIGN_O=(DISP_BL|DISP_MID|DISP_BR|DISP_BOT),
	SIGN_P=(DISP_BL|DISP_TOP|DISP_UL|DISP_MID|DISP_UR),
	SIGN_Q=0, // not possible with this display
	SIGN_R=(DISP_BL|DISP_MID),
	SIGN_S=SIGN_5,
	SIGN_T=(DISP_BL|DISP_BOT|DISP_MID|DISP_UL),
	SIGN_U=(DISP_BOT|DISP_UL|DISP_UR|DISP_BL|DISP_BR),
	SIGN_V=0, // not possible with this display
	SIGN_W=0, // not possible with this display
	SIGN_X=0, // not possible with this display
	SIGN_Y=(DISP_UR|DISP_BR|DISP_UL|DISP_MID|DISP_BOT),
	SIGN_Z=0, // not possible with this display
	SIGN_DOT=(DISP_DOT),
	SIGN_MINUS=(DISP_MID),
	SIGN_EXCLAMATION=(DISP_UR|DISP_DOT),
	SIGN_NONE=0
} sign_t;

void display_init();
void display_update();
void display_digit(uint8_t led_idx, uint8_t digit);
void display_sign(uint8_t led_idx, sign_t sign);
void display_temperature(int16_t number);
void display_number(int16_t number);
void display_fixed_point(int16_t number, int8_t exp);
