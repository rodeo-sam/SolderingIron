
/*
 * clock.h 
 *
 * Created:  Tue Feb  3 23:22:32 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 


#pragma once
#include <avr/io.h>

typedef struct{
	int16_t sec;
	int8_t subsec;
} clock_offset_t;

clock_offset_t clock_offset;
/** 128*subsec = seconds = 0,999424s **/
volatile uint16_t seconds;
/** 4*subsubsec = subsec = 7,808ms **/
volatile uint8_t subsec; 
/**Zeitbasis 1,952ms **/
volatile uint8_t subsubsec; 
/** initialisiert die Uhr **/
void clock_init (void);

/** stellt die Uhr
 * @param sec aktuelle Sekunde
 * @param subsec aktuelle Subsekunde
**/
void clock_set (uint16_t sec, uint8_t subsec);

/** liefert die aktuelle Sekunde
 * @return aktuelle Sekunde
**/
uint16_t clock_get_sec (void);

/** liefert die aktuelle Subsekunde
 * @return aktuelle Subsekunde
**/
uint8_t clock_get_subsec (void);
uint8_t clock_get_subsubsec(void);


