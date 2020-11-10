/*
   Description:
   The goal of this program is to interface the atmega1284 micro controller with a GPS and LCD to display the longitude and latitude.
   This will be used to see if the micro controller is actually communicating with the GPS receiver and later be used to store on a SD card
   Notes:
   - The latitude and longitude values written to the SD card are in the form 'ddmm.mmmm', known as degree and decimal
     minute (DMM) format.
   IDE: Atmel Studio 7 
*/

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU					8000000UL
#define DataBusRegisters		PORTC
#define DataDirBusRegisters		DDRC
#define DataDirControl			DDRD
#define en						PORTD5
#define rs						PORTD6
#define rw						PORTD7

void Commands(unsigned char command);
void sendCharacter(char character);
void blink(void);
void init_lcd(void);
void sendString(char *);

int main(void)
{		
	DDRA = 0xFF; //first two registers as outputs
	DDRD = 0xE0; //pd5,pd6,pd7 as outputs 
	PORTD = 0; //setting control to low
	
	init_lcd();	
	sendString("Testing...");
	
	return 0;
}

void init_lcd(void){
	char cmds[5] = {0x38, 0x0F, 1, 2, 0x14};
	for (int i=0;i<5;i++){
		Commands(cmds[i]);
		_delay_ms(2);
	}
}
	

void Commands(unsigned char command){
	
	PORTA = command;
	PORTD &= ~((1<<rs) | (1<<rw)); //read write and register select are low
	PORTD |= 1<<en;
	asm volatile("nop");
	asm volatile("nop");
	PORTD &= ~(1<<en);
}

void sendCharacter(char character)
{
	PORTA = character;
	PORTD &= ~(1<<rw); //turn off RW (write mode)
	PORTD |= (1<<rs); //turn on RS (character display mode)
	PORTD |= (1<<en);
	asm volatile("nop");
	asm volatile("nop");
	PORTD &= ~1<<en;
	
}

void sendString(char *str){
	
	while(*str != '\0'){
		sendCharacter(*str);
		_delay_ms(2);
		str++;
	}
	
}





