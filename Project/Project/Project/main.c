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
