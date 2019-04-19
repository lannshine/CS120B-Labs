/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #5 Exercise #1
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
	DDRD = 0xFF; PORTD = 0x00; // Configure PORTB as outputs, initialize to 0s
	
	unsigned char led = 0x00; // intermediate variables used for port updates
	unsigned char tmpA = 0x00; 
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpA = ~PINA;

		// 2) Perform Computation
		if (tmpA == 0)
		{
			led = 0x40;
		}
		else if ((tmpA == 0x01) || (tmpA == 0x02))
		{
			led = 0x60;
		}
		else if ((tmpA == 0x03) || (tmpA == 0x04))
		{
			led = 0x70;
		}
		else if ((tmpA == 0x05) || (tmpA == 0x06))
		{
			led = 0x38;
		}
		else if ((tmpA == 0x07) || (tmpA == 0x08) || (tmpA == 0x09))
		{
			led = 0x3C;
		}
		else if ((tmpA == 0x0A) || (tmpA == 0x0B) || (tmpA == 0x0C))
		{
			led = 0x3E;
		}
		else if ((tmpA == 0x0D) || (tmpA == 0x0E) || (tmpA == 0x0F))
		{
			led = 0x3F;
		}
		else
		{
			led = 0x40;
		}
		
		// 3) write results to port
		PORTD = led;
	}
}
