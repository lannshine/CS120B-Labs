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
        else if ((tmpA == 0x0001) || (tmpA == 0x0010) || (tmpA == 0x0100) || (tmpA == 0x1000)) {
            tmpC = (tmpC & 0xF8) | 0x03; // Sets tmpC to bbbbb011
            // (clear rightmost 3 bits, then set to 011)
        }
        else if ((tmpA == 0x0011) || (tmpA == 0x0101) || (tmpA == 0x1001) || (tmpA == 0x0110) || (tmpA == 0x1010)
        || (tmpA == 0x1100)) {
            tmpC = (tmpC & 0xF8) | 0x02; // Sets tmpC to bbbbb010
            // (clear rightmost 3 bits, then set to 010)
        }
        else if ((tmpA == 0x0111) || (tmpA == 0x1011) || (tmpA == 0x1110)) {
            tmpC = (tmpC & 0xF8) | 0x01; // Sets tmpC to bbbbb010
            // (clear rightmost 3 bits, then set to 010)
        }
        else {
            tmpC = (tmpC & 0xF8) | 0x00; // Sets tmpC to bbbbb000
            // (clear rightmost 3 bits, then set to 000)
        }
        // 3) Write output
        PORTC = tmpC;
    }
    return 0;
}
