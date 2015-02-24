/*
 * menu.c 
 *
 * Created:  Sat Feb 21 20:20:43 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include "display.h"
uint8_t in_menu;

void (*exit_callback)(void);
void exit_menu(void);

void menu_init(void(*exit)(void))
{
	in_menu = 1;
	exit_callback = exit;
	display_sign(2,SIGN_S);
	display_sign(1,SIGN_E);
	display_sign(0,SIGN_T); 
	buttons_init(0, 0, &exit_menu, 0);

}

void exit_menu(void)
{
	in_menu = 0;
	exit_callback();
}
