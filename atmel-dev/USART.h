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
#define UBRR1_VALUE			(((F_CPU / (USART_BAUDRATE * 2UL))) - 1)

void USART_Init(void);
unsigned char GPS_Receive(void);
unsigned char PING_Receive(void);
void PING_Transmit(unsigned char);

#endif /* USART_H_ */
