#include <avr/io.h>
#include "USART.h"

void USART_Init(void){
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR0_VALUE>>8);
	UBRR0L = (unsigned char)UBRR0_VALUE;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0);//|(1<<RXCIE0);
	/* Set frame format: 8 data bit, 1 stop bit */
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_Receive(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}