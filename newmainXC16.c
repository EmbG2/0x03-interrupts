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
uint8_t t2_status, t2_status_prev;

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
        }
        default: {
            break;
        }
    }
    return 0;
}

// ASSIGNMENT PART 1
void part_1(){
    
    // Set the TIMER1 for LED1
    tmr_setup_period(TIMER1, 200);
    tmr_turn(TIMER1, 1);
    
    // Set the TIMER2 for LED2
    tmr_setup_period(TIMER2, 10);
        
    IFS0bits.T2IF = 0;              // Reset the interrupt's flag
    IEC0bits.T2IE = 1;              // Activate TIMER2's interrupt
    
    tmr_turn(TIMER2, 1);
    
    while(1){
        LATAbits.LATA0 ^= 1;
        tmr_wait_period(TIMER1);
    }
}

// ASSIGNMENT PART 2
void part_2(){
    tmr_setup_period(TIMER1, 10);
    
    // Set the TIMER2 for LED2    
    tmr_setup_period(TIMER2, 200);
    tmr_turn(TIMER2, 1);
    
    RPINR0bits.INT1R = 88;
    INTCON2bits.INT1EP = 0;         // 1 = Falling edge; 0 = Rising edge
    
    IFS1bits.INT1IF = 0;              // Reset the interrupt's flag
    IEC1bits.INT1IE = 1;              // Activate TIMER2's interrupt
    
    while(1){
        LATGbits.LATG9 ^= 1;
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
    IFS1bits.INT1IF = 0;            // Reset the flag of the interrupt
    IFS0bits.T1IF = 0;              // Reset the interrupt's flag
    IEC0bits.T1IE = 1;              // Activate TIMER1's interrupt
    tmr_turn(TIMER1, 1);
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;              // Reset the flag of the interrupt
    tmr_turn(TIMER1, 0);
    if (PORTEbits.RE8){
        LATAbits.LATA0 ^= 1;
    }
    IEC0bits.T1IE = 0;
}