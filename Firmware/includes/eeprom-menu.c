/*
 * eeprom-menu.c 
 *
 * Created:  Sat May  2 15:40:08 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 


#include <avr/io.h>
#include "menu.h"
#include "solder_iron.h"
#include "tip.h"
#include "eeprom-config.h"

submenu_t eeprom_menu[1];

static int16_t *temp ;
static uint8_t temp_num = 0;

void (*eeprom_exit_callback)(void);
void eeprom_menu_exit(void);
void (*eeprom_menu_enter)(void);
void (*eeprom_menu_third_long)(void);
void (*eeprom_up_p)(void);
void (*eeprom_down_p)(void);

static void up(void)
{
  if (*temp != TEMP_MAX) {
	  int16_t t = *temp;
	  t++;
	  *temp = t;
	  display_temperature(*temp);
  }
}

static void down(void)
{
  if (*temp != TEMP_MIN) {
	  int16_t t = *temp;
	  t--;
	  *temp = t;
	  display_temperature(*temp);
  }
}

static void enter(void)
{
	config_save();
	tip_set_temp(*temp);
	eeprom_menu_exit();
}
void eeprom_menu_init(void(*exit)(void))
{
	temp = ((int16_t*)curr_menu->data);
	temp_num = in_menu;
	in_menu = 1;
	if (exit){
		eeprom_exit_callback = exit;
	}else{
		eeprom_menu[0].on_leave_action = curr_menu->on_leave_action;
	}
	menu_enter = &enter;
	menu_third_long = 0;
	up_p = &up;
	down_p = &down;
	eeprom_menu[0].menu_display_name = &display_name_as_name;
	eeprom_menu[0].on_enter_action = &enter;

	eeprom_menu[0].data = &eeprom_menu[0].name;
	eeprom_menu[0].name.l=SIGN_S;
	eeprom_menu[0].name.m=SIGN_E;
	eeprom_menu[0].name.r=SIGN_T;
	
	curr_menu = &eeprom_menu[0];
	curr_menu_size = 1;

	submenu_setup(0);
}

 void eeprom_menu_exit(void)
{
	in_menu = 0;
	if (eeprom_exit_callback != 0 ){
		eeprom_exit_callback();
	}
}
