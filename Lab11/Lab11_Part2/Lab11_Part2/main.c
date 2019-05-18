#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "io.c"
#include "queue.h"
/*unsigned char* QueuetoString(Queue Q)
{
	unsigned char* string = ;
	
	return string;
}*/

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRA = 0xFF; PORTA = 0x00; // LCD control lines
	unsigned char message[38] = "CS120B is Legend... wait for it DARY! ";
	//unsigned char display[38];
	Queue display;
	unsigned char count = 0;
	unsigned char digit = 16;
	LCD_init();
	LCD_ClearScreen();
	char c;

    while (1) 
    {
		LCD_ClearScreen();
		while(count < 17)
		{
			QueueEnqueue(display, message[count]);
			LCD_DisplayString(digit, display);
			digit--;
			count++;
		}
		
		while (count < 24)
		{
			QueueDequeue(display);
			QueueEnqueue(display, message[count]);
			LCD_DisplayString(digit, display);
			count++;
		}
		
		while (count < 38)
		{
			QueueDequeue(display);
			LCD_DisplayString(digit, display);
			count++;
		}
		
		count = 0;
		QueueMakeEmpty(display);
		
		// do the string shuffle
		//c = message[0]; // take copy of the left most character
		// shuffle all the others one place to the left
		//for (int i = 0; i < 38; i++) {
		//	message[i] = message[i + 1];
		//}
		// put the saved character on the end
		//message[38] = c;
		// just in case it's disturbed
		//message[38] = 0; // end of string marker
		_delay_ms(400);
    }
}
