/*
 * timing.c 
 *
 * Created:  Tue Feb  3 22:34:06 2015
 * Author: Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/atomic.h>

#include "timing.h"
#include "clock.h"



void timer_prepare (void)
{
	timer_cnt++;
}

void timer_delete(next_time_t *next)
{
	timer_cnt--;
}


void timer_set (next_time_t *next)
{
	uint32_t current;
	uint32_t next_long;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		current = ( (uint32_t) seconds << 9) + ( (uint32_t) subsec << 2) + subsubsec;
	}
	next_long = ( (uint32_t) next->next_sec << 9) + ( (uint32_t) next->next_subsec << 2) + next->next_subsubsec;
	next_long+=current;
	next->timer.next_sec=(next_long>>9)%512;
	next->timer.next_subsec=(next_long>>2)%128;
	next->timer.next_subsubsec=(next_long)%4;
	if (current > next_long) {
		next->overflow = 1;
	} else {
		next->overflow = 0;
	}
	next->active=1;
}

uint8_t timer_past (next_time_t *next)
{
	uint8_t ret=0;
	if(timer_cnt > timer_updated){
		if(!next->updated){
			next->timer.next_sec += clock_offset.sec;
			next->timer.next_subsec += clock_offset.subsec;
			next->timer.next_subsubsec=0;
			timer_updated++;
			next->updated=1;
		}
	}else{
		next->updated=0;
	}
	
	if(next->active){
		uint32_t current;
		uint32_t next_long;
		ATOMIC_BLOCK(ATOMIC_FORCEON){
			current = (uint32_t)( (uint32_t) seconds << 9) + ( (uint32_t) subsec << 2) + subsubsec;
		}
		next_long = (uint32_t)( (uint32_t) next->timer.next_sec << 9) + ( (uint32_t) next->timer.next_subsec << 2) + next->timer.next_subsubsec;
        
        //overflow
		if (next_long > current) {  
			ret = 0; // not expired 
		} else {
			ret = 1; // expired 
			next->active=0;
		}
	}
	return ret;
}

void timer_init (next_time_t* timer, uint16_t sec, uint8_t subsec, uint8_t subsub_sec)
{
	timer->next_sec=sec;
	timer->next_subsec=subsec;
	timer->next_subsubsec=subsub_sec;
}

