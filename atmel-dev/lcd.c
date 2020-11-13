#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

void print_data(unsigned char *latitude, unsigned char *longitude){
	
	sendString("Lat:");
	_delay_ms(2);
	sendString(latitude);
	_delay_ms(2);
	sendString("  ");
	_delay_ms(2);
	sendString("Long:");
	_delay_ms(2);
	sendString(longitude);
	_delay_ms(1200);
	Commands(2);
	_delay_ms(16);

}

void init_lcd(void){
	char cmds[5] = {0x38, 0x0E, 1, 2, 0x14};
	for (int i=0;i<5;i++){
		Commands(cmds[i]);
		_delay_ms(2);
	}
}
void Commands(unsigned char command){
	
	DataRegisters = command;
	ControlRegisters &= ~((1<<rs) | (1<<rw)); //read write and register select are low
	ControlRegisters |= 1<<en; //execute commands stored in data register
	_delay_us(5);
	ControlRegisters &= ~(1<<en);
	_delay_us(5);
}

void sendCharacter(unsigned char character){
	DataRegisters = character;
	ControlRegisters &= ~(1<<rw); //turn off RW (write mode)
	ControlRegisters |= (1<<rs); //turn on RS (character display mode)
	ControlRegisters |= (1<<en);
	_delay_us(5);
	ControlRegisters &= ~1<<en;
	_delay_us(5);
}

void sendString(unsigned char *str){
	
	while(*str != '\0'){
		sendCharacter(*str);
		_delay_ms(2);
		str++;
	}
	return;
}