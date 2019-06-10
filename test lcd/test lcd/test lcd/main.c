/*
 * test lcd.c
 *
 * Created: 6/9/2019 8:54:34 PM
 * Author : 2710230
 */ 

#include <avr/io.h>
#include "io.c"

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	

	LCD_init();
	
	LCD_customChar();
	/*
	LCD_ClearScreen();
	LCD_Cursor(5);
	LCD_WriteData(0x00);
	LCD_Cursor(10);
	LCD_WriteData(0x01);
	*/LCD_DisplayString(2, " Wait, Press   Button to Walk");
					LCD_Cursor(1);
					LCD_WriteData(0x00);
					LCD_Cursor('0');

	//LCD_Cursor(5);
	//LCD_WriteData(0x01);
	//LCD_DisplayString(1, "Hello World");

}

