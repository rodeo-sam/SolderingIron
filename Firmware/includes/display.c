
#include "display.h"
#include <avr/io.h>

#define LED0 (1<<PC3)
#define LED1 (1<<PC4)
#define LED2 (1<<PC5)

//dot
#define DOT 1
//right bottom
#define RB 	2
//middle
#define M 	4
//right top
#define RT 	8
//left top
#define LT 	16
//top
#define T 	32
//left bottom
#define LB 	64
//bottom
#define B 	128

typedef enum {
	SIGN_0=250,
	SIGN_1=10 ,
	SIGN_2=236,
	SIGN_3=174,
	SIGN_4=30 ,
	SIGN_5=182,
	SIGN_6=246,
	SIGN_7=42 ,
	SIGN_8=254,
	SIGN_9=190,
	SIGN_A=126,
	SIGN_B=214,
	SIGN_C=196,
	SIGN_D=206,
	SIGN_E=244,
	SIGN_F=116,
	SIGN_DOT=1
} sign_t;

static uint8_t framebuffer[3];
//extern uint16_t g_temperature;

void display_init() {
	DDRB = 0xff;
	PORTB= 0;
	DDRC |= LED0|LED1|LED2;
	framebuffer[2] = 0;
	framebuffer[1] = 0;
	framebuffer[0] = 0;
}

void display_digit(uint8_t led_idx, uint8_t digit) {
	PORTC &= ~(LED0|LED1|LED2); //all off

	if (digit>15) {
		//error
		framebuffer[led_idx] = SIGN_DOT;
	} else {
		sign_t sign[16] = {SIGN_0, SIGN_1, SIGN_2, SIGN_3, SIGN_4, SIGN_5, SIGN_6, SIGN_7, SIGN_8, SIGN_9, SIGN_A, SIGN_B, SIGN_C, SIGN_D,SIGN_E, SIGN_F};
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
