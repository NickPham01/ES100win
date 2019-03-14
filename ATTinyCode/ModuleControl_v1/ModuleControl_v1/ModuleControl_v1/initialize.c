#include "constants.h"
#include "initialize.h"


/************************************************************************
PIN INITIALIZATIONS
*************************************************************************/
/*
	Function: set data direction DDRx and pullup enable PUEx registers for an array of pins
		Input:	array of pin_t pointers (related to physical pin number), desired setting:
					- second argument can be INPUT (= 0) or OUTPUT (= 1)
					- third argument can be NORMAL (= 0) or PULLUP (= 1)
		Output:	sets i/o settings
		Return:	none
*/
void setPinInitializationRegisters(pin_t *pinArray[], enum pinConfig configuration) {
	for (uint8_t i = 0; i < sizeof(pinArray), i++) {
	
		/* Set the configuration registers for the pin */
		switch(configuration) {
			case INPUT:
				bitOFF(PINS[i]->p_port->DIR, PINS[i]->num);					// set the correct bit of the data direction register to input
				break;
			case PULLUP:
				bitOFF(PINS[i]->p_port->DIR, PINS[i]->num);					// set the correct bit of the data direction register to input
				bitON(*(&(PINS[i]->p_port->PIN0CTRL) + PINS[i]->num), 3);		// find the correct pin control register by dereferencing address of first ctrl register plus an offset and set the 3rd bit to enable pullup
				break;
			case OUTPUT:
				bitON(PINS[i]->p_port->DIR, PINS[i]->num);						// set the correct bit of the data direction register to output
				break;	
		};
	}
}

/*
	Function: set data direction registers (DDRx) and pull ups (PUEx)
		All settings defined in function.
*/
void initializePins(void) {
	setPinInitializationRegisters(UIswitches[], PULLUP);	// set UI switches to input with pullup enabled
	setPinInitializationRegisters(UILEDs[], OUTPUT);		// set UI LEDs to output
	setPinInitializationRegisters(routingPins[], OUTPUT);	// set routing pins (relay control) to output
	setPinInitializationRegisters(r1inPins[], PULLUP);		// set inputs to receiver 1 to input with pullup enabled
	setPinInitializationRegisters(r1outPins[], OUTPUT);		// set outputs from receiver 1 to output
	setPinInitializationRegisters(r2inPins[], PULLUP);		// set inputs to receiver 2 to input with pullup enabled
	setPinInitializationRegisters(r2outPins[], OUTPUT);		// set outputs from receiver 2 to output	
}

/************************************************************************
TIMER SETUP
*************************************************************************/
/*
	Function: set up Timer A to generate an interrupt every 1ms
*/
void setupMillis(void) {
	TCA_SINGLE_struct.CTRLA		|= 0x09;	// turn on timerA with 1/64 prescalar (f = 16MHz/64 = 250kHz)
	TCA_SINGLE_struct.INTCTRL	|= 0x10;	// enable compare channel 0 interrupt
	
	TCA_SINGLE_struct.CMP0H		|= 0x00;	// set the high byte of the compare register to 0x00
	TCA_SINGLE_struct.CMP0L		|= 0xFA;	// set the low byte of the compare register to 0xFA = 250
}