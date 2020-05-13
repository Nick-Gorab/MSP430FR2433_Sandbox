/* Timing.c
 * 
 * Author   -   Nick Gorab
 * Created  -   5/12/2020
 * Modified -   5/12/2020
 *
 * This file contains functions related to the embedded timers 
 *
 */
 #include "system.h"
 
 
 void Software_Trim();
 
 
 int clockInit (void) {
    // Initialize the MCLK to 8MHz and SMCLK to 1MHz
    __bis_SR_register(SCG0);    // disables FLL (Frequency Locked Loop)
    CSCTL3 |= SELREF__REFOCLK;  // set REFO as FLL reference source
    
    // DCOFTRIM = 3, DCO Range = 8MHz
    CSCTL1 = DCOFTRIMEN | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3; 
    CSCTL2 = FLLD_0 + 243;      // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);    // enable FLL
    Software_Trim();
    
    CSCTL4 = SELMS__DCOCLKDIV;   // sets MCLK source to DCOCLKDIV
    CSCTL5 |= DIVM_0 | DIVS_3;  // Sets MCLK = DCO = 8MHz and SMCLK = 1MHz
    
    return 0;
 }
 
 
 // TODO: Add funcitonality to change the frequency and mode
 int timerA1Init (void)
 {
     TA1CCTL0 = CCIE;
     TA1CCR0 = 10000;
     TA1CTL = TASSEL_2 | MC_1; // Sources SMCLK in continuous mode

     return 0;
 }
 
 
 
 // Software trim function copied from TI tutorials 
 // Consider modifying it slightly so i don't have to include the copyright 
 // Add a fail condition?
 void Software_Trim() {
    unsigned int oldDcoTap = 0xffff;
    unsigned int newDcoTap = 0xffff;
    unsigned int newDcoDelta = 0xffff;
    unsigned int bestDcoDelta = 0xffff;
    unsigned int csCtl0Copy = 0;
    unsigned int csCtl1Copy = 0;
    unsigned int csCtl0Read = 0;
    unsigned int csCtl1Read = 0;
    unsigned int dcoFreqTrim = 3;
    unsigned char endLoop = 0;

    do
    {
        CSCTL0 = 0x100;                         // DCO Tap = 256
        do
        {
            CSCTL7 &= ~DCOFFG;                  // Clear DCO fault flag
        }while (CSCTL7 & DCOFFG);               // Test DCO fault flag

        __delay_cycles((unsigned int)3000 * 8);// Wait FLL lock status (FLLUNLOCK) to be stable
                                                           // Suggest to wait 24 cycles of divided FLL reference clock
        while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));

        csCtl0Read = CSCTL0;                   // Read CSCTL0
        csCtl1Read = CSCTL1;                   // Read CSCTL1

        oldDcoTap = newDcoTap;                 // Record DCOTAP value of last time
        newDcoTap = csCtl0Read & 0x01ff;       // Get DCOTAP value of this time
        dcoFreqTrim = (csCtl1Read & 0x0070)>>4;// Get DCOFTRIM value

        if(newDcoTap < 256)                    // DCOTAP < 256
        {
            newDcoDelta = 256 - newDcoTap;     // Delta value between DCPTAP and 256
            if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)) // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim--;
                CSCTL1 = (csCtl1Read & (~(DCOFTRIM0+DCOFTRIM1+DCOFTRIM2))) | (dcoFreqTrim<<4);
            }
        }
        else                                   // DCOTAP >= 256
        {
            newDcoDelta = newDcoTap - 256;     // Delta value between DCPTAP and 256
            if(oldDcoTap < 256)                // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim++;
                CSCTL1 = (csCtl1Read & (~(DCOFTRIM0+DCOFTRIM1+DCOFTRIM2))) | (dcoFreqTrim<<4);
            }
        }

        if(newDcoDelta < bestDcoDelta)         // Record DCOTAP closest to 256
        {
            csCtl0Copy = csCtl0Read;
            csCtl1Copy = csCtl1Read;
            bestDcoDelta = newDcoDelta;
        }

    }while(endLoop == 0);                      // Poll until endLoop == 1

    CSCTL0 = csCtl0Copy;                       // Reload locked DCOTAP
    CSCTL1 = csCtl1Copy;                       // Reload locked DCOFTRIM
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
 }
 
 
 
 
 
 /*
 
 To maintain the power efficiency of the device while simulatneously allowing
 fast computation, the peripheral devices will be based off of the sub-main
 clock (SMCLK). This will operate at a frequency of 1MHz, while the main clock
 (MCLK) will operate at a frequency of 8MHz. 
 
 MCLK is disabled upon entering low power mode while SMCLK remains active.
 With the timers sourcing SMCLK, this means that timing and scheduling will 
 remain active when the device is in low-power mode, with the added benefit of
 power saving resulting from MCLK being disabled. Once a computation needs to be
 performed, the device will 'wake up' and processing will take place using the 
 8MHz MCLK.
 
 Considerations might be made into increasing the frequency of MCLK to 16MHz and
 SMCLK to 2MHz, but the speed/power-consumption tradeoff might not be reasonable
 with the typical processing taking place on the microcontroller. This may be 
 subject to change based on the nature of the subsystem (ex. payload requiring
 significant math operations). 
 
 */
