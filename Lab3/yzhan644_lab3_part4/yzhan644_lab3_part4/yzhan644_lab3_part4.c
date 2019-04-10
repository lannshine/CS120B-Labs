/*
 * yzhan644_lab3_part4.c
 *
 * Created: 4/10/2019 1:20:31 AM
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
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	// initialize to 0s
	unsigned char tmpA = 0x00; // intermediate variable used for port updates
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpA = PINA;
				
		// Take the upper nibble of PINA and map it to the lower nibble of PORTB
		// Take the lower nibble of PINA and map it to the upper nibble of PORTC
		// 2) Perform Computation
		
		tmpB = SetBit(tmpB, 3, GetBit(tmpA, 7));
		tmpB = SetBit(tmpB, 2, GetBit(tmpA, 6));
		tmpB = SetBit(tmpB, 1, GetBit(tmpA, 5));
		tmpB = SetBit(tmpB, 0, GetBit(tmpA, 4));
		
		tmpC = SetBit(tmpC, 7, GetBit(tmpA, 3));
		tmpC = SetBit(tmpC, 6, GetBit(tmpA, 2));
		tmpC = SetBit(tmpC, 5, GetBit(tmpA, 1));
		tmpC = SetBit(tmpC, 4, GetBit(tmpA, 0));
		// 3) write results to port
		PORTB = tmpB;
		PORTC = tmpC;
	}
}
