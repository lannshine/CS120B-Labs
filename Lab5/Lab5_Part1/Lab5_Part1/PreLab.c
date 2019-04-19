/*
 * Lab5_PreLab.c
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
	DDRD = 0xFF; PORTD = 0x00; // Configure PORTB as outputs, initialize to 0s
	unsigned char led = 0x00;
	unsigned char button0 = 0x00;
	unsigned char button1 = 0x00;
	unsigned char button2 = 0x00;
	unsigned char button3 = 0x00;
	
	while(1)
	{
		// 1) Read inputs
		button0 = ~PINA & 0x01; // button is connected to A0
		button1 = ~PINA & 0x02; // button is connected to A1
		button2 = ~PINA & 0x04; // button is connected to A2
		button3 = ~PINA & 0x08; // button is connected to A3

		// 2) Perform Computation
		if (button0) { // True if button0 is pressed
			led = (led & 0xFE) | 0x01; // Sets B to bbbbbbb1
			// (set rightmost 1 bit to 1)
		}
		else {
			led = (led & 0xFE); // Sets B to bbbbbbb0
			// (set rightmost 1 bit to 0)
		}
		
		if (button1) { 
			led = (led & 0xFD) | 0x02; 
		}
		else {
			led = (led & 0xFD);
		}
		
		if (button2) {
			led = (led & 0xFB) | 0x04;
		}
		else {
			led = (led & 0xFB);
		}
		
		if (button3) {
			led = (led & 0xF7) | 0x08;
		}
		else {
			led = (led & 0xF7);
		}

		// 3) Write output
		PORTD = led;
	}
}
