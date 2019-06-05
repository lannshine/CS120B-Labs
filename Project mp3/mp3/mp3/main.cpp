/*
 * mp3.cpp
 *
 * Created: 6/4/2019 11:29:16 PM
 * Author : 2710230
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

#include <avr/io.h>
#define Motion (PINB & 0x01)

int main(void)
{
	DDRB = 0x00; PORTB = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char led = 0x00;
	TimerSet(3000);
	TimerOn();
	
	while (1)
	{
		if (Motion)
		{
			led = 0x00;
		}
		else
		{
			led = 0xFF;

		}
		
		PORTD = led;
	}
}
