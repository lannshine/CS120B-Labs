/*
 * yzhan644_lab3_part1.c
 *
 * Created: 4/9/2019 10:13:50 PM
 * Author : 2710230
 */ 

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
							   // initialize to 0s
	unsigned char tmpA = 0x00; // intermediate variable used for port updates
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;

	while(1)
	{
		unsigned char ones = 0x00;

		// 1) Read Inputs and assign to variables
		tmpA = PINA;
		tmpB = PINB;
		
		for(unsigned char i = 0; i < 8; ++i)
		{
			if (GetBit(tmpA, i))
			{
				++ones;
			}
		}
	
		for(unsigned char i = 0; i < 8; ++i)
		{
			if (GetBit(tmpB, i))
			{
				++ones;
			}
		}
		
		// 2) Perform Computation
		// if PA0 is 1, set PB1PB0=01, else =10
		tmpC = (tmpC & 0x00) | ones; 
		// 3) write results to port
		PORTC = tmpC;
	}
}
