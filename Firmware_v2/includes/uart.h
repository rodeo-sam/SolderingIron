#pragma once


#include <inttypes.h>


#define MYUBBR ((F_CPU/(16*BAUD))-1)


void uart_init(void);
void uart_transmit (unsigned char* data, uint8_t length);
