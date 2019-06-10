#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "io.c"
#define B0 (~PINB & 0x01)
#define B1 (~PINB & 0x02)
#define PORTA_HIGH(PORT) PORTA |= (1 << PORT)
#define PORTA_LOW(PORT) PORTA &= ~(1 << PORT)
#define P_Green 2
#define P_Red 3
#define C_Green 4
#define C_Yellow 5
#define C_Red 6
#define MinCarTime 5
#define WalkTime 20

unsigned char button;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
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

void PL_Tick(unsigned char pressed)
{
	static unsigned char p;
	static unsigned int carTime;
	switch (P_State)
	{
		case Stop:
			carTime++;
			PORTA_HIGH(P_Red);
			PORTA_LOW(P_Green);
			if (!pressed)
			{
				P_State = Stop;
			}
			else if (carTime > MinCarTime)
			{
				P_State = Transit;
				p = 0;
			}
			break;
		
		case Transit:
			p++;
			if (p < 5)
			{
				P_State = Transit;
			}
			else
			{
				P_State = Walk;
				p = WalkTime;
				LCD_DisplayString(2, " Cross         with Care");
				LCD_Cursor(1);
				LCD_WriteData(0x00); //custom character
				LCD_Cursor('0');
			}
			break;
			
		case Walk:
			PORTA_HIGH(P_Green);
			PORTA_LOW(P_Red);
			p--;
			
			if (p > 0)
			{
				P_State = Walk;
				if (p < 10)
				{
					LCD_Cursor(1);
					LCD_WriteData(p + '0');
					LCD_Cursor('0');
				}
			} 
			else
			{
				P_State = Stop;
				LCD_DisplayString(2, " Wait, Press   Button to Walk");
				LCD_Cursor(1);
				LCD_WriteData(0x01); //custom character
				LCD_Cursor('0');
				carTime = 0;
			}
			break;
		
		default:
		P_State = Stop;
	}
}

void CL_Tick(unsigned char pressed)
{
	static unsigned char c;
	static unsigned int carTime;
	switch (C_State)
	{
		case Green:
			PORTA_HIGH(C_Green);
			PORTA_LOW(C_Yellow);
			PORTA_LOW(C_Red);
			carTime++;
			if (!pressed)
			{
				C_State = Green;
			}
			else if (carTime > MinCarTime)
			{
				C_State = Yellow;
				c = 0;
				PORTA_HIGH(C_Yellow);
				PORTA_LOW(C_Green);
				//PORTA_LOW(C_Red);
			}
			break;
		
		case Yellow:
			c++;
			if (c < 5)
			{
				C_State = Yellow;
			}
			else
			{
				C_State = Red;
				c = 0;
				PORTA_HIGH(C_Red);
				//PORTA_LOW(C_Green);
				PORTA_LOW(C_Yellow);
			}
			break;
		
		case Red:
			c++;
			
			if (c < WalkTime)
			{
				C_State = Red;
			}
			else
			{
				C_State = Green;
				c = 0;
				carTime = 0;
			}
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
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	LCD_init();
	TimerSet(1000);
	TimerOn();
	P_State = Stop;
	C_State = Green;
	unsigned short counter = 0;
	unsigned char press = 0;
	unsigned char released = 0;
	PORTA_HIGH(3);
	PORTA_HIGH(4);
	LCD_customChar0();
	LCD_customChar1();
	LCD_DisplayString(2, " Wait, Press   Button to Walk");
	LCD_Cursor(1);
	LCD_WriteData(0x01); //custom character
	LCD_Cursor('0');
	PWM_on();

	while(1)
	{
		if (B1)
		{
			set_PWM(500);
			_delay_ms(100);
			set_PWM(0);
		}
		
		if(B0)
		{
			press = 1;
			released = 0;
		}
		else
		{
			press = 0;
		}
		
		if (counter >= 1)
		{
			PL_Tick(press);
			CL_Tick(press);
			counter = 0;
		}
		
		while(!TimerFlag);
		counter++;
		TimerFlag = 0;
	}
}