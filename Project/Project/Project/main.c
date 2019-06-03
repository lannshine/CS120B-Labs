#include <avr/io.h>
#define Motion (~PINB & 0x04)

int main(void)
{
    DDRB = 0x00; PORTB = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char led = 0x00;
    while (1) 
    {
		if (!Motion)
		{
			led = 0xFF;
		}
		else
		{
			led = 0x00;
		}
		
		PORTD = led;
    }
}

