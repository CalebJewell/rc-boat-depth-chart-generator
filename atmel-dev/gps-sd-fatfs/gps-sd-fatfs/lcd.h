#ifndef LCD_H_
#define LCD_H_

#define DataRegisters			PORTA
#define DataDirRegisters		DDRA
#define DataDirControl			DDRD
#define ControlRegisters		PORTD
#define en				PORTD5
#define rs				PORTD6
#define rw				PORTD7

void print_data(unsigned char *,unsigned char *);
void Commands(unsigned char);
void sendCharacter(unsigned char);
void init_lcd(void);
void sendString(unsigned char *);
void USART_Transmit(unsigned char data);

#endif /* LCD_H_ */
