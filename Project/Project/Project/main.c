#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "ssd1306xled.c"
#include "ssd1306xledtx.c"
#include "font6x8.h"

#define TESTING_DELAY 500


int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	//DDRD = 0xFF; PORTD = 0x00;
	
	_delay_ms(100);
	ssd1306_init();
	ssd1306_setpos(0,10);
	ssd1306tx_string("Hello");
	
	while (1)
	{
	}
}
