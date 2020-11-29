// Internal clock 8MHz
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
// Arduino library to grab Ping Sensor depth
#include "ping1d.h"

// Set baud rate of UART0 to 9600 and asynchronous mode
// NOTE: UART0 is to communicate with other AVR, UART1 for Ping Sensor
#define BAUDRATE  9600
#define UBRR_VALUE  (F_CPU/(BAUDRATE*16UL) - 1)

// Use UART1 for Ping Sensor
static Ping1D ping { Serial1 };

void USART_Init( void );
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char );
void sendData( int );

void setup() {
  // LEDs to debug the receive/transmit of a byte with other AVR
  DDRB |= 0x03;
  PORTB = 0x00;

  // Arduino function to set UART1 to 9600 baud
  Serial1.begin(9600);
  
  //Serial.begin(9600);
  USART_Init();
  while (!ping.initialize()) {
    PORTB |= 0x01;
  }
  PORTB &= ~(0x01);
}

void loop()
{
  int i;
  char distStr[10];

  PORTB |= 0x02;
  unsigned char go_ahead = USART_Receive();
  PORTB &= ~(0x02);

  if (ping.update()) {
    sprintf(distStr, "%d", ping.distance());
    
    for (i = 0; distStr[i] != '\0'; i++) {
      PORTB |= 0x01;
      sendData((int)distStr[i]);
      PORTB &= ~(0x01);
    }
    
    // Send "stop-byte" --> '*'
    PORTB |= 0x01;
    USART_Transmit(0x2A);
    PORTB &= ~(0x01); 
    
    
//    Serial.println("*");
//    Serial.println(ping.distance());
//    Serial.println();
  }
}

void USART_Init()
{
  UBRR0H = (unsigned char)UBRR_VALUE >> 8;
  UBRR0L = (unsigned char)UBRR_VALUE;

  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (3 << UCSZ00);
}

unsigned char USART_Receive()
{
  while (!(UCSR0A & (1 << RXC0))) {
    PORTB |= 0x02;
  }

  return UDR0;
}

void USART_Transmit( unsigned char data )
{
  while (!(UCSR0A & (1 << UDRE0))) {
    PORTB |= 0x01;
  }
  UDR0 = data;
}

void sendData( int d ) {
  switch (d) {
    case 48:
      //Serial.print(0x30);
      USART_Transmit(0x30);
      break;
    case 49:
      //Serial.print(0x31);
      USART_Transmit(0x31);
      break;
    case 50:
      //Serial.print(0x32);
      USART_Transmit(0x32);
      break;
    case 51:
      //Serial.print(0x33);
      USART_Transmit(0x33);
      break;
    case 52:
      //Serial.print(0x34);
      USART_Transmit(0x34);
      break;
    case 53:
      //Serial.print(0x35);
      USART_Transmit(0x35);
      break;
    case 54:
      //Serial.print(0x36);
      USART_Transmit(0x36);
      break;
    case 55:
      //Serial.print(0x37);
      USART_Transmit(0x37);
      break;
    case 56:
      //Serial.print(0x38);
      USART_Transmit(0x38);
      break;
    case 57:
      //Serial.print(0x39);
      USART_Transmit(0x39);
      break;
    default:
      //Serial.print(0x21);
      USART_Transmit(0x21);
      break;
  }
}
