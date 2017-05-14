/*
 * tip.h
 *
 * Created:  Mon Feb  2 22:31:29 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 *  *      Yannic Schröder <yannic@dontclickthis.de>
 */
#pragma once

void tip_init(void);

void tip_updateTemperature(void);

uint8_t tip_powerOn(void); // returns 1 if powering on was successful or tip was already on
uint8_t tip_powerOff(void); // returns 1 if powering off was successful or tip was already off

void tip_disable(void);
void tip_enable(void);
