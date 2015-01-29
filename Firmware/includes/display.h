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

#define DISP_OFF 0x00
#define DISP_0 (DISP_BR|DISP_UR|DISP_UP|DISP_BOT|DISP_BL|DISP_UL)
#define DISP_1 (DISP_BR|DISP_UR)
#define DISP_2 (DISP_UP|DISP_UR|DISP_MID|DISP_BL|DISP_BOT)
#define DISP_3 (DISP_UP|DISP_UR|DISP_MID|DISP_BR|DISP_BOT)
#define DISP_4 (DISP_UL|DISP_MID|DISP_UR|DISP_BR)
#define DISP_5 (DISP_UP|DISP_UR|DISP_MID|DISP_BR|DISP_BOT)
#define DISP_6 (DISP_UP|DISP_UL|DISP_MID|DISP_BL|DISP_BOT|DISP_BR)
#define DISP_7 (DISP_UP|DISP_UR|DISP_BR)
#define DISP_8 (DISP_UP|DISP_UR|DISP_BR|DISP_UL|DISP_BL|DISP_MID|DISP_BOT)
#define DISP_9 (DISP_UP|DISP_UR|DISP_BR|DISP_UL|DISP_MID|DISP_BOT)
#define DISP_A (DISP_UP|DISP_UR|DISP_BR|DISP_UL|DISP_BL|DISP_MID)
#define DISP_B (DISP_BR|DISP_UL|DISP_BL|DISP_BOT)
#define DISP_C (DISP_UP|DISP_UL|DISP_BL|DISP_MID|DISP_BOT)
#define DISP_D (DISP_UR|DISP_BR|DISP_BL|DISP_MID|DISP_BOT)
#define DISP_E (DISP_UP|DISP_UL|DISP_BL|DISP_MID|DISP_BOT)
#define DISP_F (DISP_UP|DISP_UL|DISP_BL|DISP_MID)
#define Cdp (DISP_DOT)

// ADD YOUR CUSTOM CHARS HERE!!

void display_init();
void display_digit(uint8_t led, uint8_t digit);
void display_custom(uint8_t led, uint8_t segments);
