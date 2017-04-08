/*
 * tip.h 
 *
 * Created:  Mon Feb  2 22:31:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 *  *      Yannic Schröder <yannic@dontclickthis.de>
 */ 
#pragma once

#define TIP_CONNECTED    0
#define TIP_DISCONNECTED 1
#define TIP_BROKEN       2

static void tip_conversion_complete_callback(uint16_t adc);
void tip_init(void(*callback)(int16_t));
int16_t tip_getTemp(void);
void tip_startConversion(void);
uint8_t tip_getState(void);
uint8_t tip_powerOn(void); // returns 1 if powering on was successful or tip was already on
uint8_t tip_powerOff(void); // returns 1 if powering off was successful or tip was already off
void tip_disable(void);
void tip_enable(void);
int16_t tip_getTargetTemp(void);
void tip_setTargetTemp(int16_t temp);

