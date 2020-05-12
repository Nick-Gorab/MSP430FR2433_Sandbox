#include <msp430.h> 
#include "system.h"

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;


	__bis_SR_register(CPUOFF + GIE); // Enters LPM0 w/ interrupts enabled

	while(1) {};	// Loops forever, processing is interrupt based	
}



#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A(void)
{
	// Timer ISR Code
}
