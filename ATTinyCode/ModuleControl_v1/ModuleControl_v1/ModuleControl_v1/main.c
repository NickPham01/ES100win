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
#include "fsms.h"



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
