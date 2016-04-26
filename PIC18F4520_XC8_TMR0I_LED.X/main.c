/**
 * MPLABX IDE project example for blinking LED with Timer0 Interrupt
 * 
 * @author   Burak Enez
 * @email    burakenez@gmail.com
 * @website  http://burakenez.org
 * @ide      MPLAB X IDE
 * @compiler XC8 v1.34
 * @license  GNU GPL v3
 */
/* Include core modules */
#include <xc.h>
#include <plib/timers.h>
/* Include my libraries here */
#include "pic18f4520_config.h"
#include "pic18f4520_delay.h"

/* Timer0 interrupt value calculation
 * 
 * fcmd = fosc/4 = 32MHz/4 = 8Mhz
 * Tcmd = 1/fcmd = 1/8MHz = 1/(8*10^6) s = 0.125 us = 0.125*10^-6 s
 * TMR0_value = 2^TMR0bit - (DesiredInterruptTime/(Tcmd*Prescaler))
 * Prescaler = 128
 * TMR0bit = 16
 * DesiredInterruptTime = 1s
 * TMR0_value = 3036
 */

int TMR0_value = 3036; // Required value for 1 second Timer0 Interrupt
int flag = 0; // LED toogle control flag

void main(void) {
    
    TRISCbits.RC1 = 0; 
    LATCbits.LATC1 = 0;
    
    // Configure Timer0
    OpenTimer0(TIMER_INT_ON &  // Enable TIMER Interrupt
               T0_16BIT &      // Timer0 is configured as an 16-bit timer/counter
               T0_SOURCE_INT & // Internal instruction cycle clock (CLKO) acts as source of clock
               T0_PS_1_128);   // 1:128 Prescaler value
               
    WriteTimer0(TMR0_value); // Write TMR0 value to Timer0 to count

    INTCONbits.GIE = 1; // Enables all unmasked interrupts
    
    while(1) { 
        if(flag == 1) {
            LATCbits.LATC1 = ~LATCbits.LATC1;
            flag = 0;
        }
    }
}

void interrupt ISR(void) {

    if(INTCONbits.TMR0IF==1) { // if Timer0 Interrupt is occurred
        INTCONbits.TMR0IF = 0; // Clear the interrupt flag
        WriteTimer0(TMR0_value); // Update TMR0 value
        flag = 1; 
    }
}
