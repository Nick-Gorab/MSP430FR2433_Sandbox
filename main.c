#include <msp430.h> 
#include "system.h"

void ToggleLED(void) {
	P1OUT ^= BIT0;
}

void ToggleLED2(void) {
	P1OUT ^= BIT1;
}
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	if(clockInit()) {
	    return 1;
	}

	if(timerA1Init()) {
	   return 1;
	}


	P1DIR |= BIT0;

	Schedule_Task(ToggleLED, 10000, 3);
	Schedule_Task(ToggleLED2, 20000, 5);

	__no_operation();

	Remove_Task(1);

	__no_operation();


	__bis_SR_register(CPUOFF + GIE); // Enters LPM0 w/ interrupts enabled
}



#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A(void)
{
	// Timer ISR Code
    P1OUT ^= BIT0;
}
