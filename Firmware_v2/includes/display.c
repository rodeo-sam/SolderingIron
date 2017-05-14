
#include "display.h"
#include <avr/io.h>
#include <avr/interrupt.h>


static uint16_t i2bcd(uint16_t i);
static void display_custom(uint8_t led_idx, uint8_t segments);


static uint8_t framebuffer[3];


// initializing the DDRs and ports for the LED display
void display_init() {
	DISP_DDR = 0xff;
	DISP_PORT = 0;
	LED_DDR |= LED0|LED1|LED2;
	framebuffer[2] = 0;
	framebuffer[1] = 0;
	framebuffer[0] = 0;
}

// source http://www.expertcore.org/viewtopic.php?f=8&t=3742
static uint16_t i2bcd(uint16_t i) {
	uint16_t binaryShift = 0;
	uint8_t digit;
	uint16_t bcd = 0;
	while (i > 0) {
		digit = i % 10;
		bcd += (digit << binaryShift);
		binaryShift += 4;
		i /= 10;
	}
	return bcd;
}

void display_error() {
	framebuffer[2] = SIGN_E;
	framebuffer[1] = SIGN_R;
	framebuffer[0] = SIGN_R;
}

void display_sign(uint8_t led_idx, sign_t sign){
	framebuffer[led_idx] = sign;
}


// converts a fixed point number into framebuffer
void display_fixed_point(int16_t number, int8_t exp) {
	if ((exp <- 2) || (exp > 2)) {
		display_error();
		return;
	}

	switch(exp) {
		case 2:
			display_number(number*100);
			break;
		case 1:
			display_number(number*10);
			break;
		case 0:
			display_number(number);
			break;
		case -1:
			display_number(number);
			if (framebuffer[1] == 0) {
				framebuffer[1] = SIGN_0;
			}
			framebuffer[1] |= (SIGN_DOT);
			break;
		case -2:
			display_number(number);
			if (framebuffer[1] == 0) {
				framebuffer[1] = SIGN_0;
			}
			if (framebuffer[2] == 0) {
				framebuffer[2] = SIGN_0;
			}
			framebuffer[2] |= (SIGN_DOT);
			break;
	}
}




// converts a binary number to BCD and copys into framebuffer
void display_number(int16_t number) {

	uint8_t digits[3] = {0,0,0};

	// check for valid data
	if ((number < -99) || (number > 999)) {
		display_error();
		return;
	}

	// get bcd digits of the absolut value

  uint16_t bcd = 0;
  if (number < 0) {
    bcd = i2bcd(number*-1);
  } else {
    bcd = i2bcd(number);
  }

	digits[0] = bcd & 0x0f;
	digits[1] = (bcd>>4) & 0x0f;
	digits[2] = (bcd>>8) & 0x0f;

	// remove zeros
	int8_t i=2;
	while ((digits[i]==0) && (i>0)) {
		display_custom(i, SIGN_NONE);
		i--;
	}

	// show number
	for (; i>=0; i--) {
		display_digit(i, digits[i] );
	}

	if (number < 0 )
		display_custom(2, SIGN_MINUS);
}

// put a specific character up on a specific area of the seven segment display
void display_digit(uint8_t led_idx, uint8_t digit) {
	LED_PORT &= ~(LED0|LED1|LED2); //all off

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

	LED_PORT &= ~(LED0|LED1|LED2); //all off
	DISP_PORT = framebuffer[led_idx];
  switch(led_idx) {
  case(0):
    LED_PORT |= LED0;
    break;
  case(1):
    LED_PORT |= LED1;
    break;
  case(2):
    LED_PORT |= LED2;
    break;
  }

	if (led_idx++ >2) led_idx = 0;
}

// setting up custom characters on the seven segment displays
static void display_custom(uint8_t led_idx, uint8_t segments) {
	framebuffer[led_idx] = (uint8_t) segments;
}



// display a temp in the scale of your choice (see config.h)
void display_temperature(int16_t number) {
  display_number(number);
}
