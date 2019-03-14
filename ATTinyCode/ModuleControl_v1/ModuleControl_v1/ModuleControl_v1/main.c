/*
 * ModuleControl_v1.c
 *
 * Created: 3/11/2019 10:43:33 PM
 * Author : NPham
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "constants.h"
#include "initialize.h"
#include "utilities.h"

/******************************************************************************
TIMER AND MILLIS SETUP
	timer_millis stores the ms since the program started (global).
	ISR is called when timerA matches CMP0 register (set to 250 which is equivalent to 1ms).
	In the ISR, timer_millis is incremented, the timer is reset, and the interrupt flag is cleared.
	millis() should be called to get the time, which accesses the 32 bit value in an atomic access.
*******************************************************************************/	
volatile unsigned long timer_millis = 0;	// good for 49 days before overflow

ISR(__vectorCMP0) {
	timer_millis++;							// increment the ms timer counter
	TCA0.SINGLE.CNTH = 0x00;			// reset the counter
	TCA0.SINGLE.CNTL = 0x00;
	TCA0.SINGLE.INTFLAGS |= (1<<4);	// reset the interrupt flag
}

unsigned long millis(void) {
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = timer_millis;
	}
	return millis_return;
}

/*******************************************************************************
Set up FSMs
********************************************************************************/

/* Structs to store FSM */


int main(void)
{

	/* Run initializations */
	setupMillis();
	initializePins();
	
	/* Initialize FSMs */
	
    while (1) {
		/* Update FSMs */
    }
}
