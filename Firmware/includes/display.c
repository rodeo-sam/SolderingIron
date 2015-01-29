
#include "display.h"
#include <avr/io.h>

typedef enum {
	SIGN_0=(DISP_BOT|DISP_UL|DISP_UR|DISP_UP|DISP_BL|DISP_BR),
	SIGN_1=(DISP_BR|DISP_UR),
	SIGN_2=(DISP_UP|DISP_UR|DISP_MID|DISP_BL|DISP_BOT),
	SIGN_3=(DISP_UP|DISP_UR|DISP_MID|DISP_BR|DISP_BOT),
	SIGN_4=(DISP_UL|DISP_MID|DISP_UR|DISP_BR) ,
	SIGN_5=(DISP_UP|DISP_UL|DISP_MID|DISP_BR|DISP_BOT),
	SIGN_6=(DISP_UP|DISP_UL|DISP_MID|DISP_BL|DISP_BOT|DISP_BR),
	SIGN_7=(DISP_UP|DISP_UR|DISP_BR) ,
	SIGN_8=(DISP_UP|DISP_UR|DISP_BR|DISP_UL|DISP_BL|DISP_MID|DISP_BOT),
	SIGN_9=(DISP_UP|DISP_UR|DISP_BR|DISP_UL|DISP_MID|DISP_BOT),
	SIGN_A=(DISP_UP|DISP_UR|DISP_BR|DISP_UL|DISP_BL|DISP_MID),
	SIGN_B=(DISP_BR|DISP_UL|DISP_MID|DISP_BL|DISP_BOT),
	SIGN_C=(DISP_UP|DISP_UL|DISP_BL|DISP_BOT),
	SIGN_D=(DISP_UR|DISP_BR|DISP_BL|DISP_MID|DISP_BOT),
	SIGN_E=(DISP_UP|DISP_UL|DISP_BL|DISP_MID|DISP_BOT),
	SIGN_F=(DISP_UP|DISP_UL|DISP_BL|DISP_MID),
	SIGN_DOT=(DISP_DOT)
} sign_t;

static uint8_t framebuffer[3];
//extern uint16_t g_temperature;


// initializing the DDRs and ports for the LED display
void display_init() {
	DDRB = 0xff;
	PORTB= 0;
	DDRC |= LED0|LED1|LED2;
	framebuffer[2] = 0;
	framebuffer[1] = 0;
	framebuffer[0] = 0;
}

// put a specific character up on a specific area of the seven segment display
void display_digit(uint8_t led_idx, uint8_t digit) {
	PORTC &= ~(LED0|LED1|LED2); //all off

	if (digit>16) {
		//error in case digit was too high
		framebuffer[led_idx] = 0;
	} else {
		sign_t sign[17] = {SIGN_0, SIGN_1, SIGN_2, SIGN_3, SIGN_4, SIGN_5, SIGN_6, SIGN_7, SIGN_8, SIGN_9, SIGN_A, SIGN_B, SIGN_C, SIGN_D,SIGN_E, SIGN_F, SIGN_DOT};
		framebuffer[led_idx] = (uint8_t) sign[digit];
	}
}

/* framebuffer to display */
void display_update() {
	static uint8_t led_idx = 0;

	PORTC &= ~(LED0|LED1|LED2); //all off
	PORTB = framebuffer[led_idx];
	PORTC |= (1<<3+led_idx);

	if (led_idx++ >2) led_idx = 0;
}

// setting up custom characters on the seven segment displays
void display_custom(uint8_t led_idx, uint8_t segments) {
	framebuffer[led_idx] = (uint8_t) segments;
}
