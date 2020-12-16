// Internal clock 8MHz
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
// Arduino library to grab Ping Sensor depth
#include "ping1d.h"
// Arduino library to write to the SD card
#include <SD.h>
#include <SPI.h>

// Set baud rate of UART0 to 9600, asynchronous mode
// NOTE: UART0 is to communicate with other AVR, UART1 for Ping Sensor
#define BAUDRATE  9600
#define UBRR_VALUE  (F_CPU/(BAUDRATE*16UL) - 1)

// Use UART1 for Ping Sensor
static Ping1D ping { Serial1 };
File myFile;

void USART_Init( void );
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char );
void sendData( int );

void setup() {
  // LEDs to debug the receive/transmit of a byte with other AVR
  DDRB |= 0x03;

  // Arduino function to set UART1 to 9600 baud
  Serial1.begin(9600);

  //Serial.begin(9600);
  PORTB |= 0x01;
  USART_Init();
  SD.begin(4);
  ping.initialize();
  _delay_ms(500);
  PORTB &= ~(0x01);
}

void loop()
{
  int i;
  int j = 0;
  unsigned long int distInt = 0;
  char distStr[5];
  char coord[23];
  unsigned char temp;

//  // memset
//  memset(coord, 0, sizeof(char)*23);

  // Wait for MCU1 to send start byte '*', meaning it's about to send a GPS coord
  PORTB &= ~(0x01);
  //temp = USART_Receive();
  while (USART_Receive() != '*');
  PORTB |= 0x01;

  // Receive latitude
  PORTB &= ~0x02;
  i = 0;
  while (1) {
    temp = USART_Receive();
    if (temp == '!') break;
    coord[i] = temp;
    i++;
  }
  PORTB |= (0x02);

  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    for (i = 0; i < 23; i++) {
      myFile.print(coord[i]);
    }
  }

  // Wait until Ping sensor has an update
  if (ping.update()) {
    distInt = ping.distance();
  }

  // Print depth to SD card
  myFile.println(distInt);
  //sprintf(distStr, "%d", distInt);
  myFile.close();

  // Transmit depth
  PORTB |= 0x01;
  sendData(distInt/10000);
  sendData((distInt/1000)%10);
  sendData((distInt/100)%10);
  sendData((distInt/10)%10);
  sendData(distInt%10);
  // Stop byte
  USART_Transmit(0x21);
  PORTB &= ~(0x01);
  //}

  //  if (ping.update()) {
  //    PORTB |= 0x01;
  //    sprintf(distStr, "%d", ping.distance());
  //
  //    for (i = 0; distStr[i] != '\0'; i++) {
  //      j++;
  //      PORTB |= 0x01;
  //      sendData((int)distStr[i]);
  //      PORTB &= ~(0x01);
  //    }

  //    while (j < 5) {
  //      USART_Transmit(0x2D);
  //      j++;
  //    }
  //    sendData(48); // 0
  //    sendData(49); // 1
  //    sendData(50); // 2
  //    sendData(51); // 3
  //    sendData(52); // 4
  //    // Send "stop-byte" --> '!'
  //    USART_Transmit(0x21);
  //    PORTB &= ~(0x01);
  //  }
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
    case 0:
      //Serial.print(0x30);
      USART_Transmit(0x30);
      break;
    case 1:
      //Serial.print(0x31);
      USART_Transmit(0x31);
      break;
    case 2:
      //Serial.print(0x32);
      USART_Transmit(0x32);
      break;
    case 3:
      //Serial.print(0x33);
      USART_Transmit(0x33);
      break;
    case 4:
      //Serial.print(0x34);
      USART_Transmit(0x34);
      break;
    case 5:
      //Serial.print(0x35);
      USART_Transmit(0x35);
      break;
    case 6:
      //Serial.print(0x36);
      USART_Transmit(0x36);
      break;
    case 7:
      //Serial.print(0x37);
      USART_Transmit(0x37);
      break;
    case 8:
      //Serial.print(0x38);
      USART_Transmit(0x38);
      break;
    case 9:
      //Serial.print(0x39);
      USART_Transmit(0x39);
      break;
    default:
      //Serial.print(0x21);
      USART_Transmit(0x2D);
      break;
  }
}
