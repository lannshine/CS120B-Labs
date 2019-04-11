/*
 * yzhan644_lab4_part1.c
 *
 * Created: 4/10/2019 2:24:19 PM
 * Author : 
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00;
	DDRB = 0xFF;
	
	PORTB = 0x01;
	/* Replace with your application code */
	while (1)
	{
		if((PINA & 0x01) == 0x01){
			if(PORTB == 0x01){
				PORTB = 0x02;
			}
			else{
				PORTB = 0x01;
			}
		}
		else{PORTB=PORTB;}
	}
}
