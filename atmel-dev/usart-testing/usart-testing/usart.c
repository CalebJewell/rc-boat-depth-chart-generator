/*
* usart.c
*
* Created: 11/29/20 9:19:09 AM
*  Author: johnn
*/

#include <avr/io.h>
#include "usart.h"

void usart_init( unsigned int ubrr )
{
	/*
	* Set Baud Rate to 9600, Asynchronous Normal Mode
	* UBRRnH/L: Baud Rate n Register Low and High byte
	*/
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	/*
	* Enable receiver
	* UCSRnB: Control and Status Register n B
	*		Bit 4 - RXEN0: Receiver Enable
	*/
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/*
	* Set frame format: 8 data bits, 1 stop bit, 0 parity bits
	* UCSRnC: Control and Status Register n C
	*		Bit 1 - UCSZ0n: Character Size
	*/
	UCSR0C = (3 << UCSZ00);
	
	// Baud Rate 9600, Async Normal Mode
	UBRR1H = (unsigned char)(ubrr >> 8);
	UBRR1L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);
	// 8 data bits, 1 stop bit, 0 parity bits
	UCSR1C = (3 << UCSZ10);
	
}

void usart1_transmit( unsigned char data )
{
	/*
	* Wait for empty transmit buffer
	* UCSRnA: Control and Status Register n A
	*		Bit 5 - UDREn: Data Register Empty (indicates if transmit buffer (UDRn) ready to receive new data
	*/
	while (!(UCSR1A & (1 << UDRE1)));
	/*
	* Put data into buffer, sends the data
	* UDRn: Data Register n
	*/
	UDR1 = data;
}

unsigned char usart1_receive( void )
{
	/*
	* Wait for data to be received
	*		Bit 7 - RXCn: Receive Complete (set when unread data in buffer, cleared when buffer is empty)
	*/
	while (!(UCSR1A & (1 << RXC1)));
	// Get and return received data from buffer
	return UDR1;
}

void usart0_transmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
}

unsigned char usart0_receive( void )
{
	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)));
	// Get and return received data from buffer
	return UDR0;
}
