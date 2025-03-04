/*
 * File:   newmainXC16.c
 * Author: lenovo
 *
 * Created on March 4, 2025, 6:19 PM
 */


#include "xc.h"
#include "timer.h"

#define ASSIGNMENT_N 1

int main(void) {
    // Deactivate analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    // Set LEDs as outputs
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    // Set port RPI20 as input
    TRISAbits.TRISA4 = 1;
    // Activate the interrupt functionality for the port RPI<20>
    RPINR11bits.OCFBR = 20;
    // ASSIGNMENT PART 1
    tmr_setup_period(TIMER1, 200);
    int count = 0;
    
    IEC
    while(ASSIGNMENT_N == 1){
        LATAbits.LATA0 ^= 1;
        if(count == 5){
            // set int flag to 1
            
            count = 0;
        }
        tmr_wait_period(TIMER1);
        count++;
    }
    
    return 0;
}
