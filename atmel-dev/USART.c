#define F_CPU				8000000 //MCU operates at 8MHz
#include <avr/io.h>
#include "USART.h"
#include <util/delay.h>

void USART_Init(void){
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR1_VALUE>>8);
	UBRR0L = (unsigned char)UBRR1_VALUE;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<UDRIE0)|(1<<TXCIE0)|(1<<RXCIE0);
	/* Set frame format: 8 data bit, 1 stop bit */
	UCSR0C = (3<<UCSZ00)|(1<<UMSEL00);

	////setting registers up for ping sensor RX/TX communication
	//UBRR1H = (unsigned char)(UBRR1_VALUE>>8);
	//UBRR1L = (unsigned char)UBRR1_VALUE;
	///* Enable receiver and transmitter */
	//UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<TXCIE1)|(1<<UDRIE1);
	///* Set frame format: 8 data bit, 1 stop bit */
	//UCSR1C = (1<<UCSZ11)|(1<<UCSZ10)|(1<<UMSEL10)|(0<<UMSEL11);
}

unsigned char GPS_Receive(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0))){
	}
	/* Get and return received data from buffer */
	return UDR0;
}

unsigned char PING_Receive() {
	/* Wait for data to be received */
	while (!(UCSR1A & (1<<RXC1)));
	/* Get and return received data from buffer */
	return UDR1;
}

void PING_Transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0))){
		
	}
	/* Put data into buffer, sends the data */
	UDR0 = data;
}