/*
 * yzhan644_lab3_part3.c
 *
 * Created: 4/10/2019 1:10:23 AM
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
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	// initialize to 0s
	unsigned char tmpA = 0x00; // intermediate variable used for port updates
	unsigned char tmpC = 0x00;
	unsigned char ignition = 0x00;
	unsigned char driverSeated = 0x00;
	unsigned char driverSeatbelt = 0x00;
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpA = PINA;
		tmpC = 0x00;
		ignition = GetBit(tmpA, 4);
		driverSeated = GetBit(tmpA, 5);
		driverSeatbelt = GetBit(tmpA, 6);
		// 2) Perform Computation
		// if PA0 is 1, set PB1PB0=01, else =10
		if (ignition && driverSeated && !driverSeatbelt)
		{
			tmpC = SetBit(tmpC, 7, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 7, 0);
		}
		
		if (tmpA < 0x05)
		{
			tmpC = SetBit(tmpC, 6, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 6, 0);
		}
		
		if (tmpA > 0x00)
		{
			tmpC = SetBit(tmpC, 5, 1);
		}
		if (tmpA > 0x02)
		{
			tmpC = SetBit(tmpC, 4, 1);
		}
		if (tmpA > 0x04)
		{
			tmpC = SetBit(tmpC, 3, 1);
		}
		if (tmpA > 0x06)
		{
			tmpC = SetBit(tmpC, 2, 1);
		}
		if (tmpA > 0x09)
		{
			tmpC = SetBit(tmpC, 1, 1);
		}
		if (tmpA > 0x0C)
		{
			tmpC = SetBit(tmpC, 0, 1);
		}
		// 3) write results to port
		PORTC = tmpC;
	}
}
