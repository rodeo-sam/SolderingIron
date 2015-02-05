/*
 * uart.c
 *
 * Created: Di 3. Feb 13:13:27 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 *         Georg von Zengen <oni303@gmail.com>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"
#include "uart_register_map.h"

// USART lib for USART0 on Atmega328

//---------------------------------------------//
//-Configure this:-----------------------------//
// to disable in/out FIFOs and interrupts set this flag
//#define BLOCKING 1
//---------------------------------------------//


typedef enum{
	normal_e,
	double_e
}speed_mode_t;

#ifndef BLOCKING
//-------------------------------------------------------------//
// Fifo buffer size ( 255 bytes are designated for each, transmitter and reciever)
//-------------------------------------------------------------//
//BUFFER_SIZE has to be 2^n <= 256
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128
//-------------------------------------------------------------//
//-------------------------------------------------------------//
// Allocation of the fifo buffer
//-------------------------------------------------------------//
static uint8_t RX_buffer[RX_BUFFER_SIZE];
//-------------------------------------------------------------//
static uint8_t TX_buffer[TX_BUFFER_SIZE];
//-------------------------------------------------------------//
//-------------------------------------------------------------//
// Control variables for transmitter and reciever
//-------------------------------------------------------------//
static volatile  uint8_t tx_wr_ptr;
static volatile  uint8_t tx_rd_ptr;
static volatile  uint8_t rx_wr_ptr;
static volatile  uint8_t rx_rd_ptr;

//-------------------------------------------------------------//
//-------------------------------------------------------------//
// Function prototypes for each interrupt service routine
//-------------------------------------------------------------//
void USARTn_RX_vect ( void )
__attribute__ (( used, externally_visible, signal ));
//-------------------------------------------------------------//
void USARTn_TX_vect (void )
__attribute__ (( used, externally_visible, signal ));
//-------------------------------------------------------------//
// Recieve interrupt
void USARTn_RX_vect(void){
	/* write data register to RX_FIFO */
	if(rx_rd_ptr == rx_wr_ptr+1) {
		//skip input if no space left
		return;
	}
	
	RX_buffer[rx_wr_ptr] = UDRn;
	rx_wr_ptr++;
	rx_wr_ptr &= RX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
}
//-------------------------------------------------------------//
// Transmit interupt
void USARTn_TX_vect(void){
	/*read from TX_FIFO and write to Hardware register*/
	
	//reads byte from TX_FIFO
	if (tx_rd_ptr == tx_wr_ptr) {
		//if empty
		UCSRnB &= ~(1<<TXCIEn); //disable interrupt - to signal first transmission
		return;
	}
	
	UDRn = TX_buffer[tx_rd_ptr]; //write value to hardware
	tx_rd_ptr++;
	tx_rd_ptr &= TX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
}
#endif //end ndef BLOCKING

//-------------------------------------------------------------//
//----------------------------------------//
// Computes the error  (multiplied by 100 to fit in an unsigned integer) of a given baudrate (des_baudrate) and its prescaler value (ubbr)
// addtionally the mode (fast mode or normal mode) must be part of the argument
static uint16_t baud_error(const uint16_t des_baudrate, uint16_t ubrr, speed_mode_t mode){
	
	uint16_t baud_closest_match = 0;
	
	switch (mode) {
		case normal_e:
		baud_closest_match = F_CPU / (16 *ubrr + 16);
		break;
		case double_e:
		baud_closest_match = F_CPU / (8 *ubrr + 8);
		break;
	}
	
	//calc error according datasheet page 193 (mult 100)
	uint16_t error = baud_closest_match*10000/des_baudrate-10000;
	
	return error>0 ? error : (-1*error);//abs(error);
}
//----------------------------------------//
// initializes the UART peripheral depending on its arguments
uint8_t uart_init(uint16_t baudrate, stop_bits_t st, parity_t par){
	//cli(); //disable interrupts
	
	/* calculate prescaler */
	uint16_t ubrr_normal = ((uint32_t) F_CPU)/((16*(uint32_t)baudrate))-1;
	uint16_t ubrr_double = ((uint32_t) F_CPU)/((8*(uint32_t)baudrate))-1;
	uint16_t ubrr;

	if (baud_error(baudrate,ubrr_normal,normal_e) > baud_error(baudrate,ubrr_double,double_e)) {
		//use double mode
		ubrr = ubrr_double;
		UCSRnA |= (1<<U2Xn);

	}else{
		//use normal mode
		ubrr = ubrr_normal;
		UCSRnA &= ~(1<<U2Xn);
	}

	/* Set baud rate */
	UBRRnH = (unsigned char)(ubrr>>8);
	UBRRnL = (unsigned char)ubrr;
	
	/* Set output pin */
	//REMAP |= (1<<U0MAP);

	/* Set stop bits */
	switch(st) {
		case one_stop_bit_e:
			UCSRnC &= ~(1<<USBSn);
			break;
		case two_stop_bits_e:
			UCSRnC |= (1<<USBSn);
			break;
		default:
			//error
			return 1;
	}

	/* Set frame format: 8data */
	UCSRnC |= (1<<UCSZn1)|(1<<UCSZn0);
	UCSRnB &= ~(1<<UCSZn2);
	
	//parity
	switch(par) {
		case no_parity_e:
			UCSRnC &= ~((1<<UPMn0)|(1<<UPMn1));
			break;
		case odd_parity_e:
			UCSRnC |= (1<<UPMn0)|(1<<UPMn1);
			break;
		case even_parity_e:
			UCSRnC &= ~(1<<UPMn0);
			UCSRnC |= (1<<UPMn1);
			break;
		default:
			//error
			return 1;
	}
	stdin=stdout = &mystream;

#ifndef BLOCKING
	//FIFO init
	tx_wr_ptr = 0;
	tx_rd_ptr = 0;
	rx_wr_ptr = 0;
	rx_rd_ptr = 0;

	/* set interupt flags */
	UCSRnB |= (1<<RXCIEn); //RX Complete Interrupt Enable
#endif

	/* Enable receiver and transmitter */
	UCSRnB |= (1<<RXENn)|(1<<TXENn);

#ifndef BLOCKING	
    //sei();//enable interrupts
#endif
	return 0;
}

#ifdef BLOCKING
//----------------------------------------//
// Gets a byte from the UART1 peripheral
uint8_t uart_getByte( void ){
	/* Wait for data to be received */
	while ( !(UCSRnA & (1<<RXCn)) );

	/* Get and return received data from buffer */
	return UDRn;
}
//----------------------------------------//
// Sends a Byte via the UART1 peripheral
void uart_sendByte( uint8_t data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSRnA & (1<<UDREn)) );

	/* Put data into buffer, sends the data */
	UDRn = data;
}

uint8_t uart_hasByte( void ) {
	return 0;//in blocking mode is never a byte in FiFO
}

#else //ifDef BLOCKING
uint8_t uart_hasByte( void ) {
	return (rx_rd_ptr != rx_wr_ptr);
}

//----------------------------------------//
// Gets a byte from the UART0 peripheral
uint8_t uart_getByte( void ){
	//reads byte from RX_FIFO
	while (rx_rd_ptr == rx_wr_ptr);//wait until not empty
	
	uint8_t ret = RX_buffer[rx_rd_ptr];
	rx_rd_ptr++;
	rx_rd_ptr &= RX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
	return ret;
}
//----------------------------------------//
// Sends a Byte via the UART0 peripheral
void uart_sendByte( uint8_t data ) {
	
	//put data in TX_FIFO
	while (tx_rd_ptr == tx_wr_ptr+1); //wait until space to write
	
	TX_buffer[tx_wr_ptr] = data;
	tx_wr_ptr++;
	tx_wr_ptr &= TX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
	
	/* handle first byte transmission */
	if (!(UCSRnB & (1<<TXCIEn))) { //if not tx interrupt enabled
		
		UDRn = TX_buffer[tx_rd_ptr]; //write value to hardware
		tx_rd_ptr++;
		tx_rd_ptr &= TX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
		
		UCSRnB |= (1<<TXCIEn);//activate interrupt
	}
	
}

#endif //end ifnDef BLOCKING

//----------------------------------------//
// Wrapper for sendByte
// takes at argument the input character, which should be sent
// (c) and the file stream (s).
int put(char c, FILE *s){
  // Put your code here
  uart_sendByte((uint8_t) c);
  return 0;
}
//----------------------------------------//
// Wrapper for getByte
// get a character out of the file stream (s)
char get(FILE *s){
  // Put your code here
  return (char) uart_getByte();
}
//----------------------------------------//

