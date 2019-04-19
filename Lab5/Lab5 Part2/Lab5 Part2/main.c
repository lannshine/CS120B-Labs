/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #5 Exercise #2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
/*

#include <avr/io.h>
enum States { Start, zero, one, two, three, four, five, six, seven, eight, nine } state;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	state = Start;
	unsigned char tmpA;
	unsigned char tmpD;

    while (1)
    {
		tmpA = ~PINA;
		switch(state)
		{
			case Start:
				tmpD = 0x00;
				state = zero;
				break;
			
			case zero:
				if (tmpA == 0x01)
				{
					tmpD = 0x00;
					state = PoundPressed;
					break;
				}
				else
				{
					state = WaitPound;
					break;
				}
			
			case one:
				if (tmpA == 0x04)
				{
					tmpD = 0x00;
					state = PoundPressed;
					break;
				}
				else if (tmpA == 0x00) {
					tmpD = 0x00;
					state = WaitY;
					break;
				}
				else {
					tmpD = 0x00;
					state = WaitPound;
					break;
				}
			
			case WaitY:
				if (tmpA == 0x00)
				{
					tmpD = 0x00;
					state = WaitY;
					break;
				} 
				else if (tmpA == 0x02)
				{
					tmpD = 0x01;
					state = Unlock;
					break;
				}
				else
				{
					tmpD = 0x00;
					state = WaitPound;
					break;
				}
		
			case Unlock:
				if (tmpA == 0x80)
				{
					tmpD = 0x00;
					state = WaitPound;
					break;
				}
				else
				{
					tmpD = 0x01;
					state = Unlock;
					break;
				}
		}
			
		PORTD = tmpD;
    }
}

*/

#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize PORTD to 0
    unsigned char tmpA = 0x00; // intermediate variable used for port updates
    unsigned char tmpD = 0x00; 
	unsigned char lastA = 0x00;
    unsigned char incre = 0x00;
    unsigned char decre = 0x00;
    unsigned char reset = 0x00;
    
    while(1)
    {
        // 1) Read Inputs and assign to variables
		tmpA = ~PINA;
        if (tmpA != lastA)
        {
			tmpD = PORTD;
			incre = GetBit(tmpA, 0);
			decre = GetBit(tmpA, 1);
			reset = (incre && decre);
			
			// 2) Perform Computation
			if (incre && (tmpD < 0x09))
			{
    			tmpD = tmpD + 1;
			}
			if (decre && (tmpD > 0x00))
			{
    			tmpD = tmpD - 1;
			}
			if(reset)
			{
    			tmpD = 0x00;
			}
        }
        
        // 3) write results to port
		lastA = tmpA;
		reset = 0x00;
        PORTD = tmpD;
    }
}