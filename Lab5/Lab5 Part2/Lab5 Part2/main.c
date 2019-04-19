/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #5 Exercise #2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

enum States { Start, Wait, Increment, Decrement, Reset } state;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D as outputs
	state = Start;
	unsigned char tmpA = 0x00;
	unsigned char A0 = 0;
	unsigned char A1 = 0;
	unsigned char count = 0;

    while (1)
    {
		tmpA = ~PINA;
		A0 = (tmpA & 0x01);
		A1 = (tmpA & 0x02);
		switch(state)
		{
			case Start:
				count = 0;
				state = Wait;
				break;
			
			case Wait:
				if (!A1 && A0)
				{
					if (count < 9)
					{
						count++;
					}
					state = Increment;
				}
				else if (A1 && !A0)
				{
					if (count > 0)
					{
						count--;
					}
					state = Decrement;
				}
				else if (A1 && A0)
				{
					count = 0;
					state = Reset;
				}
				else
				{
					state = Wait;
				}
				break;
				
			case Increment:
				if (!A1 && A0)
				{
					state = Increment;
				}
				else if (A1 && !A0)
				{
					count--;
					state = Decrement;
				}
				else if (A1 && A0)
				{
					count = 0;
					state = Reset;
				}
				else
				{
					state = Wait;
				}
				break;
				
			case Decrement:
			if (!A1 && A0)
			{
				count++;
				state = Increment;
			}
			else if (A1 && !A0)
			{
				state = Decrement;
			}
			else if (A0 && A1)
			{
				count = 0;
				state = Reset;
			}
			else
			{
				state = Wait;
			}
			break;
			
			case Reset:
			if (!A0 && !A1)
			{
				state = Wait;
			}
			else
			{
				state = Reset;
			}
			break;
			
		default:
			state = Wait;
		}
		
		PORTD = count;
		_delay_ms(50);
	}
}
