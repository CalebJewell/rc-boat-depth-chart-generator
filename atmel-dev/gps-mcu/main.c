/*
 * Description:
 * This program interfaces with an ATmega1284 microcontroller to communicate with a GPS receiver and another ATmega1284 to send data back and forth. 
 * A stream of GPS data is parsed for $GPGGA sentences and then the latitude/longitude are grabbed and stored into a buffer which is then sent to MCU2.
 * Once that cycle completes the program also writes the depth data that is received from MCU2 and the GPS data to an LCD. 
 *   
 * Notes:
 * - The latitude and longitude values written to the SD card are in the form 'ddmm.mmmm', known as degree and decimal
 *   minute (DMM) format.
 * - MCU1 refers to the microcontroller interfacing with the GPS receiver and LCD display.
 * - MCU2 refers to the microcontroller interfacing with the Blue Robotics Ping Sensor and microSD card reader.
 *   
 * IDE:
 * Atmel Studio 7 
 */

#define F_CPU				8000000 //MCU operates at 8MHz

/* LIBRARIES NEEDED */
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "USART.h"
#include "gps_data.h"

int main(void) {

	DataDirRegisters = 0xFF; //DDRC pins to be outputs to send data to LCD
	DataDirControl = 0xE0; //pd5,pd6,pd7 as outputs 
	ControlRegisters = 0; //setting control to low
	DDRA = 0b0000011; //set outputs for LEDs 
	
	/* Send initialization commands to setup LCD */
	init_lcd();
	_delay_ms(16);

	/* Send initialization commands to setup USART communication */
	USART_Init();
	
	while(1){
		PORTA = 0x01;
		get_position();
	}
	return 0;
}