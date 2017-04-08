/*
 * menu.c 
 *
 * Created:  Sat Feb 21 20:20:43 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include "menu.h"
#include "solder_iron.h"
#include "tip.h"
#include "buttons.h"
#include "eeprom-config.h"
#include "eeprom-menu.h"
uint8_t in_menu;

submenu_t main_menu[4];
submenu_t *curr_menu;
uint8_t curr_menu_size;

submenu_t set_temp_m;

int16_t saved_temp[3] = {30,40,50};
void (*exit_callback)(void);
void menu_exit(void);
void (*menu_enter)(void);
void (*menu_third_long)(void);
void (*up_p)(void);
void (*down_p)(void);

void submenu_setup(uint8_t idx)
{
	submenu_t *m = &curr_menu[idx];
	menu_enter = m->on_enter_action;
	menu_third_long = m->on_long_action;
	m->menu_display_name(m->data);
	buttons_init(up_p, down_p, menu_enter, menu_third_long);
}
void up(void)
{
	if(in_menu < curr_menu_size){
		in_menu++;
		submenu_setup(in_menu - 1);
	}
}
void down(void)
{
	if(in_menu > 1){
		in_menu--;
		submenu_setup(in_menu - 1);
	}
}
void display_temp_as_name(void *name)
{
	display_temperature(*(int16_t*)name);
}
void display_name_as_name(void *name)
{
	submenu_name_t *n = (submenu_name_t*)name;
	display_sign(2,n->l);
	display_sign(1,n->m);
	display_sign(0,n->r);
}
void menu_set_temp(void)
{
	tip_setTargetTemp(*((int16_t*)curr_menu[in_menu - 1].data));
	
	menu_exit();
}

static void enter_sub_menu(void)
{
	curr_menu = &main_menu[in_menu - 1];
	eeprom_menu_init(exit_callback);
}

void menu_init(void(*exit)(void))
{
	in_menu = 1;
	if (exit){
		exit_callback = exit;
	}
	menu_enter = 0;
	menu_third_long = 0;
	up_p = &down;
	down_p = &up;

	main_menu[0].menu_display_name = &display_name_as_name;
	main_menu[0].on_enter_action = 0;
	main_menu[0].on_leave_action = &menu_exit;

	main_menu[0].data = &main_menu[0].name;
	main_menu[0].name.l=SIGN_S;
	main_menu[0].name.m=SIGN_E;
	main_menu[0].name.r=SIGN_T;
	
	
	main_menu[1].menu_display_name = &display_temp_as_name;
	main_menu[1].on_enter_action = &menu_set_temp;
	main_menu[1].on_long_action = &enter_sub_menu;
	main_menu[1].on_leave_action = &menu_exit;
	main_menu[1].data = &config.default_temp;
	main_menu[2].menu_display_name = &display_temp_as_name;
	main_menu[2].on_enter_action = &menu_set_temp;
	main_menu[2].on_long_action = &enter_sub_menu;
	main_menu[2].on_leave_action = &menu_exit;
	main_menu[2].data = &config.saved_temp[0];
	main_menu[3].menu_display_name = &display_temp_as_name;
	main_menu[3].on_enter_action = &menu_set_temp;
	main_menu[3].on_long_action = &enter_sub_menu;
	main_menu[3].on_leave_action = &menu_exit;
	main_menu[3].data = &config.saved_temp[1];
	curr_menu = &main_menu[0];
	curr_menu_size = 4;

	submenu_setup(0);
}

void menu_exit(void)
{
	in_menu = 0;
	exit_callback();
}
