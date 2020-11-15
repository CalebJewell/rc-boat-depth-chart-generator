#define F_CPU				8000000 //MCU operates at 8MHz

#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "lcd.h"

void get_position(){
	unsigned char latitude[16];
	unsigned char longitude[16];
	int comma = 0;
	uint8_t lat_post = 0, long_post = 0;
	
	unsigned char ch;
	unsigned char *gpgga = "$GPGGA,";
	unsigned char test_buffer[7];
	test_buffer[6] = '\0';
	int count = 0;

	for (int i=0;i<7;i++) {
	
		ch = GPS_Receive();	
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
		
		while(GPS_Receive()!= ',');
		comma = 0;
		
		//parse and store latitude data
		while(comma!=2) {
			ch = GPS_Receive();
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
			ch = GPS_Receive();
			if(ch != ',' && comma < 4) {
				longitude[long_post] = ch;
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
	}
	print_data(latitude,longitude);

}
