#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "io.c"
#define B0 (~PINB & 0x01)
#define B1 (~PINB & 0x02)
#define PORTA_HIGH(PORT) PORTA |= (1 << PORT)
#define PORTA_LOW(PORT) PORTA &= ~(1 << PORT)
#include "ssd1306xled.c"
//#include "ssd1306xledtx.c"
//#include "font6x8.h"

unsigned char button;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
//enum States { Wait, Increment, Decrement } state;
enum PedestrianLight_States { Walk, Transit, Stop } P_State;
enum CarLight_States { Green, Red, Yellow } C_State;


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

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR0A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR0A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3B = 0x00;
	TCCR3B = 0x00;
}

void PL_Tick()
{
	switch (P_State)
	{
		case Walk:
			PORTA_HIGH(2);
			PORTA_LOW(3);
			TimerSet(5000);
			P_State = Stop;
			LCD_DisplayString(1, "Cross with Care");
			break;
		
		case Transit:
			P_State = Walk;
			PORTA_HIGH(3);
			PORTA_LOW(2);
			LCD_DisplayString(1, "Wait");

		case Stop:
			P_State = Stop;
			PORTA_HIGH(3);
			PORTA_LOW(2);
			LCD_DisplayString(1, "Wait, Press Button to Walk");
			break;
		
		default:
			P_State = Stop;
	}
}

void CL_Tick()
{
	switch (C_State)
	{
		case Green:
			C_State = Green;
			PORTA_HIGH(4);
			PORTA_LOW(5);
			PORTA_LOW(6);
			break;
		
		case Red:
			C_State = Green;
			TimerSet(5000);
			PORTA_HIGH(6);
			PORTA_LOW(5);
			PORTA_LOW(4);
			break;
		
		case Yellow:
			C_State = Red;
			TimerSet(3000);
			PORTA_HIGH(5);
			PORTA_LOW(4);
			PORTA_LOW(6);
			break;
		
		default:
			C_State = Green;
	}
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; // LCD data lines
	DDRB = 0x00; PORTB = 0xFF; // Inputs
	DDRB |= (1 << 6);
	PORTB &= ~(1 << 6);
	DDRC = 0xFF; PORTC = 0x00; // OLED serial control
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	ssd1306_init();
	LCD_init();
	TimerSet(1000);
	TimerOn();
	P_State = Stop;
	C_State = Green;
	unsigned short pCounter = 0;
	unsigned short cCounter = 0;
	unsigned char walk = 0;
	PORTA_HIGH(3);
	PORTA_HIGH(4);
	LCD_DisplayString(1,"Wait, Press Button to Walk");
	PWM_on();
	//--------
	//LCD_Cursor(1);
	//LCD_WriteData(0x05);
	//------
	
	while(1)
	{	
		if (B1)
		{
			set_PWM(500);
			_delay_ms(100);
			set_PWM(0);
		}
		if (walk)
		{
			P_State = Walk;
			C_State = Red;
			walk = 0;
		}
		
		if(B0)
		{
			P_State = Transit;
			C_State = Yellow;
			walk = 1;
		}
		
		if (pCounter >= 1)
		{
			PL_Tick();
			pCounter = 0;
		}
		
		if (cCounter >= 1)
		{
			CL_Tick();
			cCounter = 0;
		}
		
		while(!TimerFlag);
		pCounter++;
		cCounter++;
		TimerFlag = 0;
	}
	
}