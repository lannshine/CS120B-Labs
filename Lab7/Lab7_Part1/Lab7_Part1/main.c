/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #7 Exercise #1
 *  
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.c"
#define F_CPU 8000000UL // Atmega 1284 default 8.0MHz without using CKDIV8
#include <util/delay.h>

enum States { Start, Wait, Increment, Decrement, Reset } state;

int main(void)
{
	
    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRA = 0x00; PORTA = 0xFF; // Configure port A as inputs
    //unsigned char* message = "Hello World";
	   
	state = Start;
	unsigned char tmpA = 0x00;
	unsigned char A0 = 0;
	unsigned char A1 = 0;
	unsigned char count = 0;
	
    // Initializes the LCD display
    LCD_init();
	
	while (1)
	{
		tmpA = ~PINA;
		A0 = (tmpA & 0x01);
		A1 = (tmpA & 0x02);
		switch(state)
		{
			case Start:
			count = 0;
			LCD_WriteData(count + '0'); // will display 0 on the LCD
			state = Wait;
			break;
			       
			case Wait:
			if (!A1 && A0)
			{
				if (count < 9)
				{
					count++;
					LCD_ClearScreen();
					LCD_Cursor('1');
					LCD_WriteData(count + '0'); // will display count on the LCD
				}
				state = Increment;
			}
			else if (A1 && !A0)
			{
				if (count > 0)
				{
					count--;
					LCD_ClearScreen();
					LCD_Cursor('1');
					LCD_WriteData(count + '0'); // will display count on the LCD
				}
				state = Decrement;
			}
			else if (A1 && A0)
			{
				count = 0;
				LCD_ClearScreen();
				LCD_Cursor('1');
				LCD_WriteData(count + '0'); // will display 0 on the LCD
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
				LCD_ClearScreen();
				LCD_Cursor('1');
				LCD_WriteData(count + '0'); // will display count on the LCD
				state = Decrement;
			}
			else if (A1 && A0)
			{
				count = 0;
				LCD_ClearScreen();
				LCD_Cursor('1');
				LCD_WriteData(count + '0'); // will display 0 on the LCD
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
				LCD_ClearScreen();
				LCD_Cursor('1');
				LCD_WriteData(count + '0'); // will display count on the LCD
				state = Increment;
			}
			else if (A1 && !A0)
			{
				state = Decrement;
			}
			else if (A0 && A1)
			{
				count = 0;
				LCD_ClearScreen();
				LCD_Cursor('1');
				LCD_WriteData(count + '0'); // will display 0 on the LCD
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
			   
		_delay_ms(50);
	}
}
