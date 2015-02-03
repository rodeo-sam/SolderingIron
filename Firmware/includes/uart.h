/*
 * uart.h
 *
 * Created: Di 3. Feb 13:14:24 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#pragma once

#include <stdio.h>
#include <avr/pgmspace.h>
//----------------------------------------//
typedef enum{
	odd_parity_e,
	even_parity_e,
	no_parity_e
}parity_t;
//----------------------------------------//
typedef enum{
	one_stop_bit_e,
	two_stop_bits_e
}stop_bits_t;
//----------------------------------------//

/** initializes the UART peripheral depending on its arguments
* 
* @param	baudrate	eg 19200
* @param	stop_bits	one or two to seperate frames
* @param	parity	none, even or odd
*
* @return 1 on error
*/
uint8_t uart_init(uint16_t baudrate, stop_bits_t stop_bits, parity_t parity);

uint8_t uart_hasByte( void );
uint8_t uart_getByte( void );

void uart_sendByte(uint8_t);
//----------------------------------------//
int put(char, FILE*);
char get(FILE*);

