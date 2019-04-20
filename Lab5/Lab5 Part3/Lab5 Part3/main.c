/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #5 Exercise #3
 *	Description: Create your own festive lights display with 6 LEDs connected
 *  to port PD5..PD0, lighting in some attractive sequence.
 *  Design: Pressing the button on PA0 cycles through the LEDs, from PD0 to PD5,
 *  Then back to PD0 and repeat.
 *  
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#define F_CPU 8000000UL // Atmega 1284 default 8.0MHz without using CKDIV8
#include <util/delay.h>
#define A0 ~PINA & 0x01
enum States { Start, Wait, Light, Dim } state;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D as outputs
	state = Start;
	unsigned char count = 0;
	unsigned char light = 0;

    while (1)
    {
		switch(state)
		{
			case Start:
				count = 0;
				light = 1;
				state = Wait;
				break;
			
			case Wait:
				if (A0)
				{
					if (light)
					{
						if (count == 0)
						{
							count = 1;
						}
						else
						{
							count = count << 1;	
						}
					state = Light;
					}
					else
					{
						if (count > 1)
						{
							count = count >> 1;
							state = Dim;
						}
						else
						{
							count = count << 1;
							state = Light;
						}
					}
				}
				else
				{
					state = Wait;
				}
				break;
				
			case Light:
				if (A0)
				{
					state = Light;
				}
				else
				{
					if (count < 20)
					{
						light = 1;
					}
					else
					{
						light = 0;
					}
					state = Wait;
				}
				break;
				
			case Dim:
			if (A0)
			{
				state = Dim;
			}
			else
			{
				if (count > 0)
				{
					light = 0;
				}
				else
				{
					light = 1;
				}
				state = Wait;
			}
			break;
			
			default:
				state = Wait;
		}
		
		PORTD = count;
		_delay_ms(50);
	}
}