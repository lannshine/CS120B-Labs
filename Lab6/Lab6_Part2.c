/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #6 Exercise #2
 *  
 *  Description: Create a simple light game that requires pressing a button
 *  on PA0 while the middle of three LEDs on PB0, PB1, and PB2 is lit. The
 *  LEDs light for 300 ms each in sequence. When the button is pressed, the
 *  currently lit LED stays lit. Pressing the button again restarts the game.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define A0 (~PINA & 0x01)

enum States { Start, D0, D1, D2, Stay, Wait } state;
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

int main(void)
{
	DDRD = 0xFF; // Set port D to output
	PORTD = 0x00; // Init port D to 0s
	DDRA = 0x00;
	PORTA = 0xFF;
	TimerSet(500);
	TimerOn();
	unsigned char tmpD = 0x00;
	unsigned char released = 1;
	
	while(1) {
		switch (state)
		{
			case Start:
				TimerOn();
				state = D0;
				tmpD = 0x01;
				if (!A0)
				{
					released = 1;
				}
				//TimerFlag = 0;
				break;
			
			case D0:
				state = D1;
				tmpD = 0x02;
				while (!TimerFlag)
				{
					if (!A0)
					{
						released = 1;
					}
					if (A0 && released)
					{
						//_delay_ms(50);
						state = Stay;
						tmpD = 0x01;
						released = 0;
						//tmpD = 0x20;
						//break;
					}
				};
				TimerFlag = 0;
				break;
			
			case D1:
				state = D2;
				tmpD = 0x04;
				while (!TimerFlag)
				{
					if (!A0)
					{
						released = 1;
					}
					if (A0 && released)
					{
//						_delay_ms(50);
						state = Stay;
						tmpD = 0x02;
						released = 0;
						//break;
					}
				};
				TimerFlag = 0;
				break;
			
			case D2:
				state = D0;
				tmpD = 0x01;
				while (!TimerFlag)
				{
					if (!A0)
					{
						released = 1;
					}
					if (A0 && released)
					{
						//_delay_ms(50);
						state = Stay;
						tmpD = 0x04;
						released = 0;
						//break;
					}
				};
				TimerFlag = 0;				
				break;
			
			case Stay:
				TimerOff();
				if (A0)
				{
					state = Stay;
					//_delay_ms(50);
					//tmpD = 0x10;
				}
				else
				{
					state = Wait;
					//tmpD = 0x20;
					//_delay_ms(50);
				}
				//TimerFlag = 0;
				break;
			
			case Wait:
				//_delay_ms(50);
				if (A0)
				{
					state = Start;
					released = 0;
					//_delay_ms(100);
				} 
				else
				{
					state = Wait;
					//tmpD = 0x20;
				}
				//TimerFlag = 0;
				break;
			
			default:
				state = Start;
				tmpD = 0x00;
				//TimerFlag = 0;
				break;
		}
		
		PORTD = tmpD;
		//TimerFlag = 0;
	}
}
