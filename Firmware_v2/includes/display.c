
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global_vars.h"
#include "config.h"
#include "display.h"


static uint16_t i2bcd(uint16_t i);
static void display_custom(uint8_t led_idx, uint8_t segments);


static uint8_t framebuffer[3];


// initializing the DDRs and ports for the LED display
void display_init() {
  led_idx = 0;
	DISP_DDR = 0xff;
	DISP_PORT = 0;
	LED_DDR |= (LED0|LED1|LED2);
  LED_PORT &= ~(LED0|LED1|LED2);
	framebuffer[2] = 255;
	framebuffer[1] = 255;
	framebuffer[0] = 255;
}


void display_error() {
	framebuffer[2] = SIGN_E;
	framebuffer[1] = SIGN_R;
	framebuffer[0] = SIGN_R;
}




// converts a binary number to BCD and copys into framebuffer
void display_set_number(int16_t number) {

  int16_t number_local = number;

	// check for valid data
	if ((number_local < -99) || (number_local > 999)) {
		display_error();
		return;
	}

	// get bcd digits of the absolut value

  sign_t sign[10] = {SIGN_0, SIGN_1, SIGN_2, SIGN_3, SIGN_4, SIGN_5, SIGN_6, SIGN_7, SIGN_8, SIGN_9};

  if (number_local < 0) {
    number_local *= (-1);
  }

  // FIRST DIGIT
  if ((number_local/100)>0) { // number > 99
    framebuffer[2] = sign[number_local/100];
  } else if (number >= 0) { // 99 >= number >= 0
    framebuffer[2] = 0;
  } else { // number < 0
    framebuffer[2] = SIGN_MINUS;
  }

  // SECOND DIGIT
  if (((number_local/10)%10)>0) {
    framebuffer[1] = sign[(number_local/10)%10];
  } else {
    if (framebuffer[2] == 0) {
      framebuffer [1] = 0;
    } else {
      framebuffer[1] = sign[0];
    }
  }

  //THIRD DIGIT
  if ((number_local%10)>0) {
    framebuffer[0] = sign[number_local%10];
  } else {
    framebuffer[0] = sign[0];
  }
}



void display_show_bars(void) {
  framebuffer[2] = SIGN_MINUS;
  framebuffer[1] = SIGN_MINUS;
  framebuffer[0] = SIGN_MINUS;
}

void display_show_tip(void) {
  framebuffer[2] = SIGN_T;
  framebuffer[1] = SIGN_I;
  framebuffer[0] = SIGN_P;
}



/* framebuffer to display */
void display_update() {
  led_idx = (led_idx + 1) % 3;
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
}
