/*
 * uart.h
 *
 * Created: Di 3. Feb 13:14:24 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#pragma once

#include <stdint.h>
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
//sends a string stored in PROGMEM
void uart_sendString_P(PGM_P s);

//sends a string from ram
void uart_sendString (const char *s);

// send as bin string
void uart_sendByte_as_bin(uint8_t data);
void uart_sendByte_as_hex(uint8_t data);

void uart_sendInt(int16_t i);
void uart_sendDouble(double d);
//void putRX_FiFO_test(uint8_t);
//uint8_t popTX_FIFO_test(void);
//----------------------------------------//
