/*
 * yzhan644_lab4_part2.c
 *
 * Created: 4/10/2019 3:06:30 PM
 * Author : 
 */ 

#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x07; // Configure port C's 8 pins as outputs,
							   // initialize PORTC to 7
	unsigned char tmpC = 0x00; // intermediate variable used for port updates
	unsigned char incre = 0x00;
	unsigned char decre = 0x00;
	unsigned char reset = 0x00;
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpC = PORTC;
		incre = GetBit(PINA, 0);
		decre = GetBit(PINA, 1);
		reset = (incre && decre);
		
		// 2) Perform Computation
		if (incre && (PORTC < 0x09))
		{
			tmpC = PORTC + 1;	
		}
		if (decre && (PORTC > 0x00))
		{
			tmpC = PORTC -1;
		}
		if(reset)
		{
			tmpC = 0x00;
		}
		
		// 3) write results to port
		PORTC = tmpC;
	}
}