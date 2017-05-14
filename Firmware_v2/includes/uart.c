#include "config.h"

#include <avr/io.h>
#include <inttypes.h>

#include "uart.h"

static uint8_t uart_enabled = 0;

void uart_init(void) {
  uart_enabled = 1;

  // Set baud rate
  UBRR0H = (unsigned char) (BAUD >> 8);
  UBRR0L = (unsigned char) BAUD;

  // enable RX and TX
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);

  // 8N1
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

  unsigned char buffer[18] = "UART - INITIATED\r\n";
  uart_transmit(buffer, 18);
}



void uart_transmit (unsigned char* data, uint8_t length) {
    if (uart_enabled == 0) {
      uart_init();
    } else {
      uint8_t i = 0;
      for (i = 0; i < length; i++) {
        while (!( UCSR0A & (1<<UDRE0)));            // wait while register is free
        UDR0 = data[i];                             // load data in the register
      }
      while (!( UCSR0A & (1<<TXC0)));
      UCSR0A |= (1<<TXC0);
    }
}
