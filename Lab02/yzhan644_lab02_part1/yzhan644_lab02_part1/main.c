/*
 * yzhan644_lab02_part1.c
 *
 * Created: 4/3/2019 3:01:03 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpA = 0x00;
	
	while (1) 
    {
		tmpA = PINA & 0x01;
		if (tmpA == 0x01) {
			tmpB = (tmpB & 0xFC) | 0x01;
		}
		else {
			tmpB = (tmpB & 0xFC) | 0x02;
		}

		PORTB = tmpB;
    }
	
	return 0;
}

