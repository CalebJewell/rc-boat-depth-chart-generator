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
	
	UBRR1H = (unsigned char)(UBRR0_VALUE>>8);
	UBRR1L = (unsigned char)UBRR0_VALUE;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8 data bit, 1 stop bit */
	UCSR1C = (3<<UCSZ10);
}

void PING_Transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while (!(UCSR1A & (1<<UDRE1))){
		//PORTA = 0x03;
	}
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

unsigned char PING_Receive(void){
	/* Wait for data to be received */
	while (!(UCSR1A & (1<<RXC1))){
		//PORTA = 0x02;
	}
	/* Get and return received data from buffer */
	return UDR1;
}

unsigned char USART_Receive(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}