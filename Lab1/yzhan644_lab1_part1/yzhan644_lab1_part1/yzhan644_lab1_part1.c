/*
 * yzhan644_lab1_part1.c
 *
 * Created: 4/7/2019 11:50:07 PM
 * Author : 2710230
 */ 

#include <avr/io.h>

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
    while(1)
    {
        // 1) Read input
        tmpA = PINA;// & 0x01;
        // 2) Perform computation
        //
        if (tmpA == 0x01) { //
            tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbb00
            // (clear rightmost 2 bits, then set to 00)
        }
        else {
            tmpB = (tmpB & 0xFC) | 0x00; // Sets tmpB to bbbbbb00
            // (clear rightmost 2 bits, then set to 00)
        }
        // 3) Write output
        PORTB = tmpB;
    }
    return 0;
}
