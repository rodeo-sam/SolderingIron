#pragma once


#define LED_DDR   DDRD
#define LED_PORT  PORTD
#define LED0      (1<<PD7)
#define LED1      (1<<PD6)
#define LED2      (1<<PD4)

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
