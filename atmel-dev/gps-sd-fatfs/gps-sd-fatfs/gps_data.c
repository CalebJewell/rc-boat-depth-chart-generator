#define F_CPU				8000000 //MCU operates at 8MHz

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <util/delay.h>
#include "ff.h"
#include "diskio.h"
#include "USART.h"
#include "lcd.h"

FIL file;                                               /* Opened file object */
FATFS fatfs;                                            /* File system object */
DIR dir;                                                /* Directory object   */
FRESULT errCode;                                        /* Error code object  */
FRESULT res;                                            /* Result object      */
UINT bytesRead;                                         /* Bytes read object  */
UINT read;                                              /* Read bytes object  */

int result=1;
//char buff[] = "hellooooooo";
int buff_stat;

void fat_init(void){
	errCode = -1;

	while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
		errCode = f_mount(0, &fatfs);                   //mount drive number 0
		//errCode = f_opendir(&dir, "/");				    	//root directory
		//
		//errCode = f_open(&file, "/data.txt", FA_OPEN_ALWAYS | FA_WRITE);
		//f_lseek(&file, f_size(&file)); // f_size(&file)
		if(errCode != FR_OK){
			result=0; //used as a debugging flag
		}
	}
}

void sd_write(unsigned char *latitude, unsigned char *longitude){
	
		errCode = f_opendir(&dir, "/");
		errCode = f_open(&file, "/data.txt", FA_OPEN_ALWAYS | FA_WRITE);
		f_lseek(&file, f_size(&file)); // f_size(&file)
		errCode = f_write(&file, latitude, 15, &bytesRead);
		errCode = f_write(&file, longitude, 15, &bytesRead);
		errCode = f_close(&file);
}

void get_position()
{
	unsigned char latitude[11];
	unsigned char longitude[12];
	unsigned char position[30];
	unsigned char depth[2];
	unsigned int send = 0x42;
	int comma = 0;
	int write_position = 0;
	uint8_t lat_post = 0, long_post = 0;
	
	unsigned char ch;
	unsigned char *gpgga = "$GPGGA,";
	unsigned char test_buffer[7];
	test_buffer[6] = '\0';
	int count = 0;
	
	unsigned char ct[2];

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
				//position[write_position] = ch;
				lat_post++;
				//write_position++;
			}
			if (ch == ',' && comma != 2){
				comma++;
			}
		}
		
		position[write_position] = ' ';
		
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
		//position[write_position] ='\0';
		lat_post = 0;
		long_post = 0;
		write_position = 0;
		
		PING_Transmit(send);
		ct[0] = PING_Receive();
		ct[1] = '\0';
		_delay_ms(1500);	
		
		errCode = f_opendir(&dir, "/");
		errCode = f_open(&file, "/data.txt", FA_OPEN_ALWAYS | FA_WRITE);
		f_lseek(&file, f_size(&file)); // f_size(&file)
		errCode = f_write(&file, latitude, 11, &bytesRead);
		errCode = f_write(&file, longitude, 12, &bytesRead);
		errCode = f_write(&file, ct, 2, &bytesRead);
		errCode = f_close(&file);
		
	}
	

	
//	print_data(latitude,longitude);
	
}
