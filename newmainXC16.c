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

void part_1(){
    // ASSIGNMENT PART 1
    // Set the TIMER1 for LED1
    tmr_setup_period(TIMER1, 200);
    // Set the TIMER2 for LED2
    T2CONbits.TON = 0;
    
    int period_Hz = 1000;           // Required frequency (1 Hz) to turn on the LED2
    int prescaler_type = 3;         // Set the type of the prescaler as 3 -> 256
    unsigned int prescaler = 256;   // Number of layers of the prescaler
    
    
    int num_base_periods = period_Hz / 200;
    // int rest_period = period_Hz - num_base_periods * 200; // Implement period non multiple of the limit 200 ms?
    T2CONbits.TCKPS = prescaler_type;
    PR2 = ((double)FCY / (prescaler * 1000)) * 200;
    TMR2 = 0;
    
    IFS0bits.T2IF = 0;              // Reset the interrupt's flag
    IEC0bits.T2IE = 1;              // Activate TIMER2's interrupt
    
    T2CONbits.TON = 1;              // Activate TIMER2
    while(1){
        LATAbits.LATA0 ^= 1;
        tmr_wait_period(TIMER1);
    }
}

void part_2(){
    // ASSIGNMENT PART 1
    // Set the TIMER2 for LED2
    tmr_setup_period(TIMER2, 200);
    RPINR0bits.INT1R = 88;
    INTCON2bits.INT1EP = 0;         // 1 = Falling edge; 0 = Rising edge
    
    IFS1bits.INT1IF = 0;              // Reset the interrupt's flag
    IEC1bits.INT1IE = 1;              // Activate TIMER2's interrupt
    
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
    IFS1bits.INT1IF = 0;            // Reset the flag of the interrupt
    t2_status = PORTEbits.RE8;
    if (t2_status && !t2_status_prev){
        LATGbits.LATG9 ^= 1;
    }
    t2_status_prev = t2_status;
}
