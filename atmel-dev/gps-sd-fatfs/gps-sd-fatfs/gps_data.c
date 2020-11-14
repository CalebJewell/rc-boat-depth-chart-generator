#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "USART.h"
#include "lcd.h"
#include "ff.h"
#include "diskio.h"

FIL file;                                               /* Opened file object */
FATFS fatfs;                                            /* File system object */
DIR dir;                                                /* Directory object   */
FRESULT errCode;                                        /* Error code object  */
FRESULT res;                                            /* Result object      */
UINT bytesRead;                                         /* Bytes read object  */
UINT read;                                              /* Read bytes object  */

int result=1;
char buff[] = "hellooooooo";
int buff_stat;

void fat_init(void){
	errCode = -1;

	while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
		errCode = f_mount(0, &fatfs);                       //mount drive number 0
		//errCode = f_opendir(&dir, "/");				    	//root directory
//
		//errCode = f_open(&file, "/data.txt", FA_OPEN_ALWAYS | FA_WRITE);
		//f_lseek(&file, f_size(&file)); // f_size(&file)
		if(errCode != FR_OK){
			result=0; //used as a debugging flag
		}
	}
}

void get_position()
{
	
	unsigned char latitude[11	];
	unsigned char longitude[12];
	int comma = 0;
	uint8_t lat_post = 0, long_post = 0;
	
	unsigned char ch;
	unsigned char *gpgga = "$GPGGA,";
	unsigned char test_buffer[7];
	test_buffer[6] = '\0';
	int count = 0;

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
		
		//parse and store latitide data
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
	
//	print_data(latitude,longitude);

	_delay_ms(2000);
	
	errCode = f_opendir(&dir, "/");				    	//root directory

	errCode = f_open(&file, "/data.txt", FA_OPEN_ALWAYS | FA_WRITE);
	f_lseek(&file, f_size(&file)); // f_size(&file)
	errCode = f_write(&file, latitude, 11, &bytesRead); // Will attempt to write string 'helloworld' to file (data.txt)
	errCode = f_write(&file, longitude, 12, &bytesRead);
	errCode = f_close(&file);

	
}
