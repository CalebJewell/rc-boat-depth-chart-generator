/*
 * usart.h
 *
 * Created: 11/29/20 9:39:27 AM
 *  Author: johnn
 */ 

#ifndef USART_H_
#define USART_H_

void usart_init( unsigned int );
// USART0 to receive GPS NMEA sentences
void usart0_transmit( unsigned char );
unsigned char usart0_receive( void );
// USART1 to receive Ping Sonar distances from other MCU
void usart1_transmit( unsigned char );
unsigned char usart1_receive( void );

#endif


