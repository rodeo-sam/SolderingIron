
/*
 * uart_register_map.h
 *
 * Created: Di 3. Feb 14:00:07 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

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

