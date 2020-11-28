// Internal clock 8MHz
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
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

void setup() {
  // LED to debug the receive/transmit of a byte with other AVR
  DDRB |= 0x03;
  PORTB = 0x00;

  // Arduino function to set UART1 to 9600 baud
  Serial1.begin(9600);

  USART_Init();
  while (!ping.initialize()) {
    PORTB |= 0x01;
  }
  
  PORTB &= ~(0x01);
}

void loop()
{
  // Variable to store depth in meters
  int depth_m;
  // 
  unsigned char go_ahead = USART_Receive();
  PORTB |= 0x01;

  if (ping.update()) {
    depth_m = ping.distance() / 1000;

    switch (depth_m) {
      case 0:
        //USART_Transmit(0x30);
        USART_Transmit(0x30);
        break;
      case 1:
        //USART_Transmit(0x30);
        USART_Transmit(0x31);
        break;
      case 2:
        //USART_Transmit(0x30);
        USART_Transmit(0x32);
        break;
      case 3:
        //USART_Transmit(0x30);
        USART_Transmit(0x33);
        break;
      case 4:
        //USART_Transmit(0x30);
        USART_Transmit(0x34);
        break;
      case 5:
        //USART_Transmit(0x30);
        USART_Transmit(0x35);
        break;
      case 6:
        //USART_Transmit(0x30);
        USART_Transmit(0x36);
        break;
      case 7:
        //USART_Transmit(0x30);
        USART_Transmit(0x37);
        break;
      case 8:
        //USART_Transmit(0x30);
        USART_Transmit(0x38);
        break;
      case 9:
        //USART_Transmit(0x30);
        USART_Transmit(0x39);
        break;
      default:
        USART_Transmit(0x30);
        //USART_Transmit(0x2D);
        break;
    }
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
    PORTB = 0x00;
  }

  return UDR0;
}

void USART_Transmit( unsigned char data )
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

//void setup(){
//  //int depth_mm;
//
//  // put your setup code here, to run once:
//  Serial1.begin(9600);
//  Serial.begin(9600);
//  ping.initialize();
//  SD.begin();
//  pinMode(15, OUTPUT);
//}
//
//void loop(){
//  if (ping.update()) {
//      //depth_mm = ping.distance();
//      //Serial.write(ping.distance());
//      sensorData = SD.open("testFile.txt", FILE_WRITE);
//      sensorData.println(ping.distance()/305);
//      sensorData.close();
//  }
//}
