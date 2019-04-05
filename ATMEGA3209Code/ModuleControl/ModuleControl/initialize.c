/* initialize.c */
#include <stdint.h>
#include "constants.h"
#include "initialize.h"


/************************************************************************
PIN INITIALIZATIONS
*************************************************************************/
/*
	Function: set data direction DDRx and pullup enable PUEx registers for an array of pins
		Input:	array of uint8_t pin numbers, desired setting:
					- second argument can be INPUT (= 0) or OUTPUT (= 1)
					- third argument can be NORMAL (= 0) or PULLUP (= 1)
		Output:	sets i/o settings
		Return:	none
*/
void setPinInitializationRegisters(const uint8_t pinArray[], uint8_t num_pins, enum pinConfig configuration) {
	for (uint8_t i = 0; i < num_pins; i++) {
		uint8_t pin = pinArray[i];
	
		/* Set the configuration registers for the pin */
		switch(configuration) {
			case INPUT_PIN:
				bitOFF(PINS[pin]->p_port->DIR, PINS[pin]->num);					// set the correct bit of the data direction register to input
				break;
			case PULLUP_PIN:
				bitOFF(PINS[pin]->p_port->DIR, PINS[pin]->num);					// set the correct bit of the data direction register to input
				bitON(*(&(PINS[pin]->p_port->PIN0CTRL) + PINS[pin]->num), 3);	// find the correct pin control register by dereferencing address of first ctrl register plus an offset and set the 3rd bit to enable pullup
				break;
			case OUTPUT_PIN:
				bitON(PINS[pin]->p_port->DIR, PINS[pin]->num);						// set the correct bit of the data direction register to output
				break;	
		};
	}
}

/*
	Function: set data direction registers (DDRx) and pull ups (PUEx)
		All settings defined in function.
*/
void initializePins(void) {
	setPinInitializationRegisters(UIswitches, numUIswitches, PULLUP_PIN);	// set UI switches to input with pullup enabled
	setPinInitializationRegisters(UILEDs, numUILEDs, OUTPUT_PIN);			// set UI LEDs to output
	setPinInitializationRegisters(routingPins, numRoutingPins, OUTPUT_PIN);	// set routing pins (relay control) to output
	setPinInitializationRegisters(r1inPins, numr1inPins, PULLUP_PIN);		// set inputs to receiver 1 to input with pullup enabled
	setPinInitializationRegisters(r1outPins, numr1outPins, OUTPUT_PIN);		// set outputs from receiver 1 to output
	setPinInitializationRegisters(r2inPins, numr2inPins, PULLUP_PIN);		// set inputs to receiver 2 to input with pullup enabled
	setPinInitializationRegisters(r2outPins, numr2outPins, OUTPUT_PIN);		// set outputs from receiver 2 to output	
}

/************************************************************************
TIMER SETUP
*************************************************************************/
/*
	Function: set up Timer A to generate an interrupt every 1ms
*/
void setupMillis(void) {
	TCA0.SINGLE.CTRLA		|= 0x09;	// turn on timerA with 1/64 prescalar (f = 16MHz/64 = 250kHz)
	TCA0.SINGLE.INTCTRL	|= 0x10;	// enable compare channel 0 interrupt
	
	TCA0.SINGLE.CMP0H		|= 0x00;	// set the high byte of the compare register to 0x00
	TCA0.SINGLE.CMP0L		|= 0xFA;	// set the low byte of the compare register to 0xFA = 250
}