/*
   Description:
   The goal of this program is to interface the atmega1284 micro controller with a GPS and LCD to display the longitude and latitude.
   This will be used to see if the micro controller is actually communicating with the GPS receiver and later be used to store on a SD card
   Notes:
   - The latitude and longitude values written to the SD card are in the form 'ddmm.mmmm', known as degree and decimal
     minute (DMM) format.
   IDE: Atmel Studio 7 
*/

/* LIBRARIES NEEDED */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

/* USEFUL DEFINES */
#define F_CPU					8000000 //MCU operates at 8MHz
#define USART_BAUDRATE			9600
#define UBRR0_VALUE				(((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define DataRegisters			PORTA
#define DataDirRegisters		DDRA
#define DataDirControl			DDRD
#define ControlRegisters		PORTD
#define en						PORTD5
#define rs						PORTD6
#define rw						PORTD7
#define BUFFER_SIZE				83

/* FUNCTION PROTOTYPES */
void Commands(unsigned char);
void sendCharacter(unsigned char);
void blink(void);
void init_lcd(void);
void sendString(unsigned char *);
char getChar(void);
char nextChar(void);
unsigned char USART_Receive(void);
void USART_Init(void);
void USART_Transmit(unsigned char data);
void USART_Flush(void);
bool check_gpgga(char *);
void get_latitude(char *);
void get_logitude(char *);

int main(void) {
	
	unsigned char latitude[16];
	unsigned char longitude[16];
	int comma = 0;
	uint8_t lat_post = 0, long_post = 0;
	
	unsigned char ch;
	unsigned char *gpgga = "$GPGGA,";
	unsigned char test_buffer[7];
	test_buffer[6] = '\0';

	/* Set data direction registers for what I need */
	DataDirRegisters = 0xFF; //first two registers as outputs
	DataDirControl = 0xE0; //pd5,pd6,pd7 as outputs 
	ControlRegisters = 0; //setting control to low
	DDRB = 0b0000011;

	/* Send initialization commands to setup LCD */
	init_lcd();
	_delay_ms(100);

	/* Send initialization commands to setup USART communication */
	USART_Init();

	cli(); 
	
	while(1){
		int count = 0;
		
		for (int i=0;i<7;i++) {
			ch = USART_Receive();
				
			if (ch == gpgga[i]) {
				//sendCharacter(ch);
				test_buffer[count] = ch;
				count++;
			}
			else {	
				count = 0;
				break; //break out of for loop
			}
		}
		if (count == 7){
			while(USART_Receive()!= ',');
			comma = 0;
			while(comma!=2) {
				ch = USART_Receive();
				if(ch != ',' && comma < 2) {
					latitude[lat_post] = ch;
					lat_post++;
				}
				if (ch == ',' && comma != 2){
					//latitude[lat_post] = ;
					//lat_post++;
					comma++;
				}
			}
		
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
			
			latitude[lat_post] = '\0';
			longitude[long_post] = '\0';
			lat_post = 0;
			long_post = 0;
		}
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
		//Commands(1);
		//_delay_ms(16);
		Commands(2);
		_delay_ms(16);
	}

}

/* FUNCTION TO INTERFACE RX COM. WITH THE GPS */
void USART_Init(void){
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR0_VALUE>>8);
	UBRR0L = (unsigned char)UBRR0_VALUE;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0);//|(1<<RXCIE0);
	/* Set frame format: 8 data bit, 1 stop bit */
	UCSR0C = (3<<UCSZ00);//|(1<<USBS0);
}

unsigned char USART_Receive(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

/* BELOW ARE FUNCTIONS WRITTEN FOR THE LCD DISPLAY */
void init_lcd(void){
	char cmds[5] = {0x38, 0x0F, 1, 2, 0x14};
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
/*-------------------------------------------------*/



