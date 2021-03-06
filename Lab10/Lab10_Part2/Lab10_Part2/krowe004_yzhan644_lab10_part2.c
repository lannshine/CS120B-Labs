/*	Karina Rowe (krowe004@ucr.edu) 
 *	Yulin Zhang (yzhan644@ucr.edu)
 *	Lab Section: CS 120B 021
 *	Assignment: Lab #10 Exercise #2
 *  
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#define A0 (~PINA & 0x01)
#define F_CPU 8000000UL
#include <util/delay.h>

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

enum TL_States { TL_SMStart, TL_Seq0, TL_Seq1, TL_Seq2 } TL_State;
enum BL_States { BL_SMStart, BL_LEDOff, BL_LEDOn } BL_State;
void BlinkLED_Tick()
{
	unsigned char B012 = (PORTB & 0x07);
	switch (BL_State)
	{
		case BL_SMStart:
			PORTB = B012;
			BL_State = BL_LEDOn;
			break;
		
		case BL_LEDOn:
			PORTB = B012 | 0x08;
			BL_State = BL_LEDOff;
			break;
		
		case BL_LEDOff:
			PORTB = B012;
			BL_State = BL_LEDOn;
			break;
		
		default:
			BL_State = BL_SMStart;
	}
}

void ThreeLED_Tick()
{
	unsigned char B4 = (PORTB & 0x08);
	switch (TL_State)
	{
		case TL_SMStart:
			PORTB = B4 | 0x00;
			TL_State = TL_Seq0;
			break;
		
		case TL_Seq0:
			PORTB = B4 | 0x01;
			TL_State = TL_Seq1;
			
			break;
		
		case TL_Seq1:
			PORTB = B4 | 0x02;
			TL_State = TL_Seq2;
			break;
		
		case TL_Seq2:
			PORTB = B4 | 0x04;
			TL_State = TL_Seq0;
			break;
		
		default:
			TL_State = TL_SMStart;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // B output
	TimerSet(5); //TimerSet(1) or TimerSet(5) do not work
                  //TimerSet(10), TimerSet(100) and TimerSet(1000) work
	TimerOn();
	BL_State = BL_SMStart;
	TL_State = TL_SMStart;
	unsigned char tled = 0;
	unsigned char bled = 0;

	while(1)
	{
		
		if (tled >= 200)
		{
			ThreeLED_Tick();
			tled = 0;
		}
		
		if (bled >= 200)
		{
			BlinkLED_Tick();
			bled = 0;
		}
		
		while(!TimerFlag);
		tled++;
		bled++;
		TimerFlag = 0;
	}
}