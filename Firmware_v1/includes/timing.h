
/*
 * timing.h 
 *
 * Created:  Tue Feb  3 23:21:52 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 


#pragma once

struct timer_t {
	uint16_t next_sec;
	uint8_t next_subsec;
	uint8_t next_subsubsec;
};

typedef  struct{
	uint16_t next_sec;
	uint8_t next_subsec;
	uint8_t next_subsubsec;
	uint8_t overflow;
	uint8_t active;
	uint8_t updated;
	struct timer_t timer;
} next_time_t;

uint8_t timer_cnt;
uint8_t timer_updated;


/** registriert eine neuen Timer 
 
**/
void timer_prepare (void);

/** löscht einen Timer 
 * @param *next Pointer auf zulöschenden Timer
**/
void timer_delete(next_time_t *next);

/** setzt einen Timer die gegebenen Werte in die Zukunft
 * @param *next Pointer auf ein next_time_t in dem steht, wie weit in den Zunkunft der Timer ablaufen soll. Maximal 511,127,3
**/
void timer_set (next_time_t *next);

/**testet ob ein Timer abgelaufen ist
 * @param *next Pointer auf den Timer
 * @return 1 wenn abgelaufen, 0 wenn nicht
**/
uint8_t timer_past (next_time_t *next);

void timer_init(next_time_t *timer, uint16_t sec, uint8_t subsec, uint8_t subsub_sec);
