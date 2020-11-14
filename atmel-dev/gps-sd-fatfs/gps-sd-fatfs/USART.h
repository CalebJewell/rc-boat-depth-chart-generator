/*
 * USART.h
 *
 * Created: 11/13/2020 4:47:44 PM
 *  Author: jewel
 */ 


#ifndef USART_H_
#define USART_H_

#define F_CPU				8000000 //MCU operates at 8MHz
#define USART_BAUDRATE			9600
#define UBRR0_VALUE			(((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

unsigned char USART_Receive(void);
void USART_Init(void);
void USART_Transmit(unsigned char data);

#endif /* USART_H_ */
