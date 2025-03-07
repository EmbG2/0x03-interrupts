/*
 * File:   newmainXC16.c
 * Author: lenovo
 *
 * Created on March 4, 2025, 6:19 PM
 */


#include "xc.h"
#include "timer.h"

#define ASSIGNMENT_N 2
#define FCY 72000000UL

int a = 0;

void part_1();
void part_2();

int main(void) {
    // Deactivate analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    // Set LEDs as outputs
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    // Set port RPI20 as input
    TRISEbits.TRISE8 = 1;
    
    switch(ASSIGNMENT_N){
        case 1: {
            part_1();
            break;
        }
        case 2: {
            part_2();
            break;
        }
        default: {
            break;
        }
    }
    return 0;
}

void part_1(){
    // ASSIGNMENT PART 1
    // Set the TIMER1 for LED1
    tmr_setup(TIMER1, 200);
    // Set the TIMER2 for LED2
    tmr_setup(TIMER2, 200);
    
    IFS0bits.T2IF = 0;              // Reset the interrupt's flag
    IEC0bits.T2IE = 1;              // Activate TIMER2's interrupt
    
    tmr_start(TIMER1);
    tmr_start(TIMER2);              // Activate TIMER2
    while(1){
        LATAbits.LATA0 ^= 1;
        tmr_wait_period(TIMER1);
    }
}

void part_2(){
    // Set the TIMER2 for LED2
    tmr_setup_period(TIMER2, 200);
    tmr_setup(TIMER1, 200);
    RPINR0bits.INT1R = 88;
    INTCON2bits.INT1EP = 1;         // 1 = Falling edge; 0 = Rising edge

    IFS1bits.INT1IF = 0;              // Reset the interrupt's flag
    IEC1bits.INT1IE = 1;              // Activate external interrupt
    
    while(1){
        a++;
        if (a == 20) {
            LATGbits.LATG9 ^= 1;
            a = 0;
        }
        tmr_wait_period(TIMER2);
    }
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;              // Reset the flag of the interrupt
    a++;
    
    if (a == 5){
        LATGbits.LATG9 ^= 1;
        a = 0;
    }
}

void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void) {
    tmr_start(TIMER1);
    tmr_wait_period(TIMER1);
    tmr_stop(TIMER1);
    IFS1bits.INT1IF = 0; // Reset the interrupt flag
    LATGbits.LATG9 ^= 1; // Toggle LED
}
