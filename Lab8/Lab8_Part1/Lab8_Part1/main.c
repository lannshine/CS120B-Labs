/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #8 Exercise #3
 *  
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	ADC_init();
	unsigned short x = 0x00;
	unsigned char tmpB = 0x00;	
	while (1) 
	{
		x = ADC;  // Value of ADC register now stored in variable x.
		
		if (x < 0x2E)
		{
			tmpB = 0x01;
		}
		else if (x < 0x35)
		{
			tmpB = 0x03;
		}
		else if (x < 0x3C)
		{
			tmpB = 0x07;
		}
		else if (x < 0x43)
		{
			tmpB = 0x0F;
		}
		else if (x < 0x4A)
		{
			tmpB = 0x1F;
		}
		else if (x < 0x51)
		{
			tmpB = 0x3F;
		}
		else if (x < 0x58)
		{
			tmpB = 0x7F;
		}
		else
		{
			tmpB = 0xFF;
		}
		
		PORTB = tmpB;
	}
}

//ADC_max = 00 0101 1111 = 0x5F
//ADC_min = 00 0010 0111 = 0x27