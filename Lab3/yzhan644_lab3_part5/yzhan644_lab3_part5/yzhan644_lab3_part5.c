/*
 * yzhan644_lab3_part5.c
 *
 * Created: 4/10/2019 1:29:52 AM
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
	DDRD = 0x00; PORTD = 0xFF; // Configure port D's 8 pins as inputs
	DDRB = 0xFE; PORTB = 0x01; // Configure port B's pin 1 to 7 as outputs, pin 0 as input
	
	// initialize to 0s
	unsigned char tmpD = 0x00; // intermediate variable used for port updates
	unsigned char tmpB = 0x00;
	unsigned int weight = 0;	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpD = PIND;
		tmpB = GetBit(PINB, 0);
		weight = (tmpD << 1) + tmpB;
		
		// 2) Perform Computation
		if (weight >= 70)
		{
			tmpB = SetBit(tmpB, 1, 1);
		}
		else if (weight > 5)
		{
			tmpB = SetBit(tmpB, 2, 1);
		}
		else
		{
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 0);
		}
		
		// 3) write results to port
		PORTB = tmpB;
		
	}
}