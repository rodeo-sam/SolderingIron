#pragma once

#include "../config.h"

#if (HW_VERSION == 1)
	#define LED_DDR   DDRC
	#define LED_PORT  PORTC
	#define LED0      (1<<PC3)
	#define LED1      (1<<PC4)
	#define LED2      (1<<PC5)

	#define DISP_DDR  DDRB
	#define DISP_PORT PORTB
	#define DISP_DOT  (1<<PB0)
	#define DISP_BR   (1<<PB1)
	#define DISP_BL   (1<<PB6)
	#define DISP_UR   (1<<PB3)
	#define DISP_UL   (1<<PB4)
	#define DISP_TOP  (1<<PB5)
	#define DISP_MID  (1<<PB2)
	#define DISP_BOT  (1<<PB7)
//	#define DISP_TOP  (1<<PB2)

	#define SENS0     PC0 // ADC0
	#define HALL      PC1 // ADC1

	#define PWR0_DDR  DDRD
	#define PWR0_PORT PORTD
	#define PWR0      PD4

//	#define BUTTON_UP (1<<PD2) // INT0
//	#define BUTTON_DN (1<<PD3) // INT1

// not used
//	#define J11       ADC7
//	#define J21       PD0 // RX
//	#define J22       PD1 // TX
//	#define J41       PD5
//	#define J42       PD6
//	#define J43       PD7

#elif (HW_VERSION == 12)
	#define LED_DDR   DDRD
	#define LED_PORT  PORTD
	#define LED0      (1<<PD7) // the lcd on the right
	#define LED1      (1<<PD6) // the lcd in the middle
	#define LED2      (1<<PD4) // the lcd on the left

	#define DISP_DDR  DDRB
	#define DISP_PORT PORTB
	#define DISP_DOT  (1<<PB4)
	#define DISP_BR   (1<<PB1)
	#define DISP_BL   (1<<PB7)
	#define DISP_UR   (1<<PB3)
	#define DISP_UL   (1<<PB0)
	#define DISP_TOP  (1<<PB2)
	#define DISP_MID  (1<<PB5)
	#define DISP_BOT  (1<<PB6)

	#define SENS0     PC0 // ADC0
	#define HALL      PC1 // ADC1

	#define PWR0_DDR  DDRD
	#define PWR0_PORT PORTD
	#define PWR0      PD5

	#ifdef ROTARY_ENCODER
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
	#endif
	
//	#define BUTTON_UP (1<<PD2) // INT0
//	#define BUTTON_DN (1<<PD3) // INT1

// not used
//	#define J11       ADC7
//	#define J21       PD0 // RX
//	#define J22       PD1 // TX
//	#define J41       PD5
//	#define J42       PD6
//	#define J43       PD7
#endif
