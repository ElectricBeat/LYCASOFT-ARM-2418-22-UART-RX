#include <lpc214x.h>
#include "LPC2148_delay.h"
#include "LPC2148_LCD_4_BIT_HEADER_FILE.h"

void UART0_init(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	/* Enable UART0 Rx0 and Tx0 pins of UART0 */
	U0LCR = 0x83;										/* DLAB = 1, 1 stop bit, 8-bit character length */
	U0DLM = 0x00;										/* For baud rate of 9600 with Pclk = 15MHz */
	U0DLL = 0x61;										/* We get these values of U0DLL and U0DLM from formula */
	U0LCR = 0x03; 									/* DLAB = 0 */
}

void UART0_Tx(char x) 						/* A function to send a byte on UART0 */
{
	U0THR = x;
	while( (U0LSR & 0x40) == 0 );		/* Wait till THRE bit becomes 1 which tells that transmission is completed */
}

unsigned char UART0_Rx() 					/* A function to receive a byte on UART0 */
{
	while( (U0LSR & 0x01) == 0);		/* Wait till RDR bit becomes 1 which tells that receiver contains valid data */
		return U0RBR;
}

int main()
{
	PINSEL0 = 0x00000000;
	IODIR0 = 0x3F0;
	
	UART0_init();
	Lcd_Intialization();

	Lcd_Command(0x80);
	Lcd_String("LYCASOFT");
	
	while(1)
	{	
		char Ch = UART0_Rx();
		
		Lcd_Command(0xC0);
		Lcd_Data(Ch);
	}
}