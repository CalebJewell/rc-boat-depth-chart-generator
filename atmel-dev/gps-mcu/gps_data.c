#define F_CPU				8000000 //MCU operates at 8MHz

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <util/delay.h>
#include <stdbool.h>
#include "USART.h"
#include "lcd.h"
#include <ctype.h>

int check_if_fix(){
	
	unsigned char ch;
	int comma = 0;
	int flag =0;
	
	while(USART_Receive() != '$');
		while(1){
			ch = USART_Receive();
			if(ch == ','){
				comma++;
			}
			if(comma == 5){
				comma = 0;
				ch = USART_Receive();
				if(ch == '1'){
					flag = 1;
					return flag;
				}
				else{
					flag = 0;
					return flag;
				}
			}
		}
		return 0;
}

void get_position(){
	
	unsigned char latitude[11];
	unsigned char longitude[12];
	unsigned char depth[6];
	int comma = 0;
	uint8_t lat_post = 0, long_post = 0;
	
	unsigned char ch;
	unsigned char *gpgga = "$GPGGA,";
	unsigned char test_buffer[7];
	test_buffer[6] = '\0';
	int count = 0;
	
	unsigned char chr;

	for (int i=0;i<7;i++) {
	
		ch = USART_Receive();	
		if (ch == gpgga[i]) {
			test_buffer[count] = ch;
			count++;
		}
		else {
			count = 0;
			break; //break out of for loop and "start over" 
		}
	}
	if (count == 7){
	
		while(USART_Receive()!= ',');
		comma = 0;
		//parse and store latitude data
		while(comma!=2) {
			ch = USART_Receive();
			if(ch != ',' && comma < 2) {
				latitude[lat_post] = ch;
				lat_post++;
			}
			if (ch == ',' && comma != 2){
				comma++;
			}
		}
		
		//parse and store longitude data
		while (comma >= 2 && comma <4){
			ch = USART_Receive();
			if(ch != ',' && comma < 4) {
				longitude[long_post] = ch;
				//position[write_position] = ch;
				//write_position++;
				long_post++;
			}
			if (ch == ',' && comma != 4){
				comma++;
			}
		}
			
		//null terminating and reseting variables 
		latitude[lat_post] = '\0';
		longitude[long_post] = '\0';
		lat_post = 0;
		long_post = 0;
		
		//If there is a fix latitude[0] will be a character 0-9
		if(isdigit(latitude[0])){
			
			PORTA = PORTA0;
			
			//send start byte '*'
			PING_Transmit(0x2A);
			
			//send latitude
			for(int i=0;i<11;i++){
				PING_Transmit(latitude[i]);
			}
			
			//send longitude
			for(int j=0;j<12;j++){
				PING_Transmit(longitude[j]);
			}
			
			//send stop byte '!' 
			PING_Transmit(0x21);
			
			count = 0;
			while(1){
				chr = PING_Receive();
				depth[count] = chr;
				count++;
				if(chr == '!'){
					depth[count - 1] = '\0';
					break;
				}
			}
			
			print_data(latitude,longitude,depth);
			_delay_ms(1000);
			PORTA = 0;
		}
		
	}
	else return;
}
