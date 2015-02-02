/*
 * buttons.h 
 *
 * Created:  Mon Feb  2 00:10:01 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#pragma once


extern void *(plus_callback)(void);
extern void *(minus_callback)(void);

void buttons_init(void (*p_callback)(void), void (*m_callback)(void));
