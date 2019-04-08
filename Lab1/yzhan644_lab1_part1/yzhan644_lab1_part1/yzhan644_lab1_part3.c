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
    DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpC = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
    unsigned char cntavail;
    while(1)
    {
        // 1) Read input
        tmpA = PINA;// & 0x01;
        // 2) Perform computation
        //
        if (tmpA == 0x0000) { //
            tmpC = (tmpC & 0xF8) | 0x04; // Sets tmpC to bbbbb100
            // (clear rightmost 3 bits, then set to 100)
        }
        else if ((tmpA == 0x01) || (tmpA == 0x02) || (tmpA == 0x04) || (tmpA == 0x08)) {
            tmpC = (tmpC & 0xF8) | 0x03; // Sets tmpC to bbbbb011
            // (clear rightmost 3 bits, then set to 011)
        }
        else if ((tmpA == 0x03) || (tmpA == 0x05) || (tmpA == 0x09) || (tmpA == 0x06) || (tmpA == 0x0A)
        || (tmpA == 0x0C)) {
            tmpC = (tmpC & 0xF8) | 0x02; // Sets tmpC to bbbbb010
            // (clear rightmost 3 bits, then set to 010)
        }
        else if ((tmpA == 0x07) || (tmpA == 0x0B) || (tmpA == 0x0E)) {
            tmpC = (tmpC & 0xF8) | 0x01; // Sets tmpC to bbbbb001
            // (clear rightmost 3 bits, then set to 010)
        }
        else {
            tmpC = (tmpC & 0xF8) | 0x80; // Sets tmpC to 1000000
            // (clear rightmost 3 bits, then set to 000)
        }
        // 3) Write output
        PORTC = tmpC;
    }
    return 0;
}
