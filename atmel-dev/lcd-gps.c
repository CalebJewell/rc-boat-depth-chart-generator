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

/* USEFUL DEFINES */
#define F_CPU				8000000UL //MCU operates at 8MHz
#define USART_BAUDRATE			9600
#define UBRR0_VALUE			((F_CPU/16/USART_BAUDRATE)-1)
#define DataRegisters			PORTA
#define DataDirRegisters		DDRA
#define DataDirControl			DDRD
#define ControlRegisters		PORTD
#define en				PORTD5
#define rs				PORTD6
#define rw				PORTD7
#define BUFFER_SIZE			64

/* FUNCTION PROTOTYPES */
void Commands(unsigned char);
void sendCharacter(char);
void blink(void);
void init_lcd(void);
void sendString(char *);
char getChar(void);
char nextChar(void);
unsigned char USART_Receive(void);
void USART_Init(void);
void USART_Transmit(unsigned char data);
void USART_Flush(void);

/* GLOBAL VARIABLES */
char data_buffer[BUFFER_SIZE];
uint8_t read_position = 0;
bool stop_flag = false;


int main(void)
{		
	/* Set data direction registers for what I need */
	DataDirRegisters = 0xFF; //first two registers as outputs
	DataDirControl = 0xE0; //pd5,pd6,pd7 as outputs 
	ControlRegisters = 0; //setting control to low
	DDRB = 1<<DDRB0;
	
	/* Send initialization commands to setup LCD */
	init_lcd();
	_delay_ms(100);
	
	/* Send initialization commands to setup USART communication */
	USART_Init();
	
	sei(); //enable global interrupts, this call is defined inside avr/interrupt.h file
	
	while(1){
		USART_Receive();
		if(stop_flag) break;
	}
	

	
	return 0;
}


/* INTERRUPTS TO BE USED FOR THE PROGRAM */
ISR(USART0_UDRE_vect){ //might get rid of this interrupt
	
	USART_Receive();
	
}

ISR(USART0_RX_vect){
	
	data_buffer[read_position] = UDR0;
	read_position++;

	if(read_position >= BUFFER_SIZE){
		read_position = 0;
		sendString(data_buffer);
		stop_flag = true;
		USART_Flush();
	}
}
/*--------------------------------------*/


/* FUNCTION TO INTERFACE RX COM. WITH THE GPS */
void USART_Init(void){
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR0_VALUE>>8);
	UBRR0L = (unsigned char)UBRR0_VALUE;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8 data, 1 stop bit */
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Transmit(unsigned char data){
	DDRB = 0x01;
	PORTB = 1<<PORTB0;
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0))){
		PORTB |= (~1<<PORTB0);
	}
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_Flush(void){
	unsigned char dummy;
	while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}
/*-------------------------------------------*/


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
	asm volatile("nop");
	asm volatile("nop");
	ControlRegisters &= ~(1<<en);
}

void sendCharacter(char character){
	DataRegisters = character;
	ControlRegisters &= ~(1<<rw); //turn off RW (write mode)
	ControlRegisters |= (1<<rs); //turn on RS (character display mode)
	ControlRegisters |= (1<<en);
	asm volatile("nop");
	asm volatile("nop");
	ControlRegisters &= ~1<<en;
}

void sendString(char *str){
	
	while(*str != '\0'){
		sendCharacter(*str);
		_delay_ms(2);
		str++;
	}
}
/*-------------------------------------------------*/



