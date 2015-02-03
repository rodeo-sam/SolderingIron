/*
 * uart.c
 *
 * Created: Di 3. Feb 13:13:27 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"

// USART lib for USART0 on Atmega328

//---------------------------------------------//
//-Configure this:-----------------------------//
#define USE_UART0 1
// to disable in/out FIFOs and interrupts set this flag
//#define BLOCKING 1
//---------------------------------------------//

#ifdef USE_UART0

#define UDRn UDR0
#define UBRRnH UBRR0H
#define UBRRnL UBRR0L

#define UCSRnA UCSR0A
#define UCSRnB UCSR0B
#define UCSRnC UCSR0C
#define UCSRnD UCSR0D
//bits in UCSRnA
#define RXCn RXC0
#define TXCn TXC0
#define UDREn UDRE0
#define FEn FE0
#define DORn DOR0
#define UPEn UPE0
#define U2Xn U2X0
#define MPCMn MPCM0
//bits in UCSRnB
#define RXCIEn RXCIE0
#define TXCIEn TXCIE0
#define UDRIEn UDRIE0
#define RXENn RXEN0
#define TXENn TXEN0
#define UCSZn2 UCSZ02
#define RXB8n RXB80
#define TXB8n TXB80
//bits in UCSRnC
#define UMSELn1 UMSEL01
#define UMSELn0 UMSEL00
#define UPMn1 UPM01
#define UPMn0 UPM00
#define USBSn USBS0
#define UCSZn1 UCSZ01
#define UCSZn0 UCSZ00
#define UCPOLn UCPOL0
//bits in UCSRnD
#define RXSIEn RXSIE0
#define RXSn RXS0
#define SFDEn SFDE0

//interrupts
#define USARTn_RX_vect USART_RX_vect
#define USARTn_TX_vect USART_TX_vect

#else // USE_UART1

#define UDRn UDR1
#define UBRRnH UBRR1H
#define UBRRnL UBRR1L

#define UCSRnA UCSR1A
#define UCSRnB UCSR1B
#define UCSRnC UCSR1C
#define UCSRnD UCSR1D
//bits in UCSR0A
#define RXCn RXC1
#define TXCn TXC1
#define UDREn UDRE1
#define FEn FE1
#define DORn DOR1
#define UPEn UPE1
#define U2Xn U2X1
#define MPCMn MPCM1
//bits in UCSRnB
#define RXCIEn RXCIE1
#define TXCIEn TXCIE1
#define UDRIEn UDRIE1
#define RXENn RXEN1
#define TXENn TXEN1
#define UCSZn2 UCSZ12
#define RXB8n RXB81
#define TXB8n TXB81
//bits in UCSRnC
#define UMSELn1 UMSEL11
#define UMSELn0 UMSEL10
#define UPMn1 UPM11
#define UPMn0 UPM10
#define USBSn USBS1
#define UCSZn1 UCSZ11
#define UCSZn0 UCSZ10
#define UCPOLn UCPOL1
//bits in UCSRnD
#define RXSIEn RXSIE1
#define RXSn RXS1
#define SFDEn SFDE1

//interrupts
#define USARTn_RX_vect USART1_RX_vect
#define USARTn_TX_vect USART1_TX_vect

#endif // USE_UART1 

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


// FIFO TEST
void putRX_FiFO_test(uint8_t data){
	/* write data register to RX_FIFO */
	if(rx_rd_ptr == rx_wr_ptr+1) {
		//skip input if no space left
		return;
	}
	
	RX_buffer[rx_wr_ptr] = data;
	rx_wr_ptr++;
	rx_wr_ptr &= RX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
}
//-------------------------------------------------------------//
uint8_t popTX_FIFO_test(void){
	/*read from TX_FIFO and write to Hardware register*/
	
	while (tx_rd_ptr == tx_wr_ptr); //wait until data
	
	uint8_t data = TX_buffer[tx_rd_ptr]; //write value to hardware
	tx_rd_ptr++;
	tx_rd_ptr &= TX_BUFFER_SIZE-1; //to allow a dynamic buffer size - Overflow
	
	return data;
}
#endif //end ifnDef BLOCKING
//-------------------------------------------------------------//
// Higher-level sending
//-------------------------------------------------------------//
void uart_sendString_P(PGM_P s) {
	unsigned char c = pgm_read_byte(s++);
	while (c) {
		uart_sendByte(c);
		c = pgm_read_byte(s++);
	}
}

void uart_sendString (const char *s)
{
	// no empty strings allowed!
	if (*s)
	{
		do
		{
			uart_sendByte(*s);
		}
		while (*(++s));
	}
}

void uart_sendByte_as_bin(uint8_t data) {
	for (uint8_t i=8;i>0; i--) {
		uint8_t bit = (data & 0x80)>>7;
		data <<= 1;
		uart_sendByte(('0'+bit));
	}
}

void uart_sendByte_as_hex(uint8_t data) {
	for (uint8_t i=2;i>0; i--) {
		uint8_t digit = (data & 0xf0)>>4;
		data <<= 4;
		digit = (digit>9)?'A'+digit-10:'0'+digit;
		uart_sendByte(digit);
	}
}

void uart_sendInt(int16_t i) {
	char buf[7];// sign + 5 digits + terminator
	itoa(i,buf, 10);
	uart_sendString(buf);
}

void uart_sendDouble(double d) {
	char buf[11]; //sign + d.ddd + e+dd + terminator
	dtostre(d,buf,3,0);
	uart_sendString(buf);
}
