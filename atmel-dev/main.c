/*
   Description:
   The goal of this program is to interface the atmega1284 micro controller with a GPS and LCD to display the longitude and latitude.
   This will be used to see if the micro controller is actually communicating with the GPS receiver and later be used to store on a SD card
   Notes:
   - The latitude and longitude values written to the SD card are in the form 'ddmm.mmmm', known as degree and decimal
     minute (DMM) format.
   IDE: Atmel Studio 7 
*/

#define F_CPU				8000000 //MCU operates at 8MHz

/* LIBRARIES NEEDED */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "lcd.h"
#include "USART.h"
#include "gps_data.h"

bool finish = false;

volatile uint8_t index = 0;

unsigned char buffer[12];
unsigned char cmds[12] = {0x42, 0x52, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0xa1, 0x00};
unsigned char *test = "TESTING";
int main(void) {

	/* Set data direction registers for what I need */
	DataDirRegisters = 0xFF; //first two registers as outputs
	DataDirControl = 0xE0; //pd5,pd6,pd7 as outputs 
	ControlRegisters = 0; //setting control to low
	DDRA = 0xFF;

	/* Send initialization commands to setup LCD */
	init_lcd();
	_delay_ms(100);

	/* Send initialization commands to setup USART communication */
	USART_Init();

	//sendString(test);

	sei();
	//cli();
	UDR0 = cmds[0];
	while(1){
	}
	
	return 0;
}

ISR(USART0_RX_vect){
	sendCharacter('!');
}

ISR(USART0_TX_vect){
	sendCharacter('i');
}

ISR(USART0_UDRE_vect){

	sendCharacter('c');
	
	while (!( UCSR0A & (1<<UDRE0))){
		PORTA = 0x01;
	}
	PORTA = 0;
	UDR0 = cmds[index];
	//if(index==11){
		//sendCharacter('1');
		//UCSR0A |= 1<<TXC0;
		//UCSR0B |= 0<<UDRIE0;
		//return;
	//}
	buffer[index] = UDR0;
	index++;
	

	
	
}