#define F_CPU				8000000 //MCU operates at 8MHz
#include <avr/io.h>
#include "USART.h"
#include <util/delay.h>

void USART_Init(void){
	
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR0_VALUE>>8);
	UBRR0L = (unsigned char)UBRR0_VALUE;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);//|(1<<UDRIE0);//|(1<<RXCIE0);
	/* Set frame format: 8 data bit, 1 stop bit, synchronous mode */
	UCSR0C = (3<<UCSZ00);//|(1<<UMSEL00);
}

unsigned char GPS_Receive(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0))){
		PORTA = 0;
	}
	/* Get and return received data from buffer */
	return UDR0;
}

void PING_Transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0))){
		PORTA =  0x07;
	}
	/* Put data into buffer, sends the data */
	UDR0 = data;
}