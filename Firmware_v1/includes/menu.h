
/*
 * menu.h 
 *
 * Created:  Tue Feb 24 21:32:05 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#pragma once
#include "display.h"

extern uint8_t in_menu;
extern uint8_t curr_menu_size;
extern void (*menu_enter)(void);
extern void (*menu_third_long)(void);
extern void (*up_p)(void);
extern void (*down_p)(void);

typedef struct {
	sign_t l;
	sign_t m;
	sign_t r;
} submenu_name_t;

typedef struct {
	submenu_name_t name;
	void(*menu_display_name)(void *name);
	void(*on_enter_action)(void);
	void(*on_long_action)(void);
	void(*on_leave_action)(void);
	void *data;
	uint8_t size;
} submenu_t;

extern submenu_t *curr_menu;

void menu_init(void(*exit)(void));
void display_temp_as_name(void *name);
void display_name_as_name(void *name);
void submenu_setup(uint8_t idx);
