/*
 * usart-testing.c
 *
 * Created: 11/29/20 9:13:35 AM
 * Author : johnn
 */ 

// Internal clock speed of 8MHz
#define F_CPU 8000000
#define BAUDRATE 9600
// Baud Rate Register as Asynchronous Normal Mode
#define MYUBRR F_CPU/(BAUDRATE*16UL) - 1

#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "sd.h"


int main(void)
{
	unsigned char rec0, rec1;
	//unsigned char *str = "Hello";
	unsigned char str[6]; 
	unsigned char c;
	unsigned int x = 0x42;
	int i;
	
	// Initialize indicator LEDs
	DDRB |= 0x03;
	
	PORTB |= 0x01;
	usart_init(MYUBRR);
	sd_init();
	_delay_ms(1000);
	PORTB &= ~(0x01);
	
    while (1) 
    {
		PORTB |= 0x02;
		usart1_transmit(x);
		_delay_ms(500);
		PORTB &= ~(0x02);
		
		PORTB |= 0x01;
		i = 0;
		do {
			c = usart1_receive();
			str[i] = c;
			i++;
		} while (c != '+');
		
		sd_write(str, i);
		_delay_ms(500);
		PORTB &= ~(0x01);
		
    }
}

