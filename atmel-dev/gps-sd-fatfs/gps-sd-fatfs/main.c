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
#include "lcd.h"
#include "USART.h"
#include "gps_data.h"

int main(void) {

	/* Set data direction registers for what I need */
	DataDirRegisters = 0xFF; //first two registers as outputs
	DataDirControl = 0xE0; //pd5,pd6,pd7 as outputs 
	ControlRegisters = 0; //setting control to low
	DDRA = 0b0000011;
	
	/* Send initialization commands to setup LCD */
	init_lcd();
	_delay_ms(100);

	/* Send initialization commands to setup USART communication */
	USART_Init();
	
	while(1){
		fat_init();
		PORTA = 0x01;
		get_position();	
	}
	return 0;
}