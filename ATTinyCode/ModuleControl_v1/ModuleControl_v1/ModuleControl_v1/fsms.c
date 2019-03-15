#include <util/atomic.h>
#include "constants.h"
#include "utilities.h"
#include "fsms.h"

/***************************************************************
	DEBOUNCE
****************************************************************/
/*
	Function: initialize debounce routine
		Input:	button pin to control
		Output:	none
		Return:	button debounce struct for FSM
					starts in WAIT state to allow buttons to calm before anything is set
		
*/
struct debounceBTN initDebounce(uint8_t button_pin) {
	struct debounceBTN btn;
	btn.pin = button_pin;
	btn.curButtonState = (readUISW() & (1<<btn.pin)) >> btn.pin;	// just get this pin
	btn.prevButtonState = btn.curButtonState;						// set the current and previous states to the same value
	btn.debounceTime = 10;											// set debounce time to be 10ms: can change later	
	btn.debounceStartTime = millis();								// start debounce counter
	btn.state = DEBOUNCE_WAIT;										// start in the wait state
	btn.isDone = 0;

	btn.output = BTN_IDLE;
	return btn;
}

/*
	Function: update for debounce routine
		Input:	btn struct to update
		Output:	mutates struct values.  Can read output in btn.output.
		Return:	none
*/
void updateDebounce(struct debounceBTN btn) {
	/* Read button and record reading time */
	btn.prevButtonState = btn.curButtonState;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		btn.curButtonState = gpioREAD(btn.pin);
		btn.currentTime = millis();
	}
	btn.isDone = (btn.currentTime - btn.debounceStartTime >= btn.debounceTime);
			
	/* Update next state based on current state and input+isDone */
	switch(btn.state) {
		case DEBOUNCE_RESET:
			btn.output = BTN_IDLE;								// turn off the output assertion
			// if the button is pressed
			if (btn.curButtonState != btn.prevButtonState) {
				btn.debounceStartTime = btn.currentTime;	// mark the debounce start
				btn.isDone = 0;								// set debounce wait flag
				btn.state = DEBOUNCE_WAIT;					// go to WAIT state
			}
			// else do nothing
			break;
		case DEBOUNCE_WAIT:
			// if the debounce time has elapsed without change
			if (btn.isDone) {
				// set output to either rising or falling based on new position after debounce
				btn.output = btn.curButtonState ? BTN_RISING : BTN_FALLING;
				btn.state = DEBOUNCE_RESET;										// go back to reset state
			}
			// if the button position has changed before debounce time is over
			else if (btn.curButtonState != btn.prevButtonState) {
				btn.debounceStartTime = btn.currentTime;				// reset the debounce timer
			}
			// else keep waiting
			break;
	}
}


/***************************************************************
	UI/Routing
****************************************************************/

/*
	Function: initialize UI FSM
*/
struct singleOutputUI initUI(uint8_t straight_button,
							 uint8_t cross_button,
							 uint8_t straight_LED,
							 uint8_t cross_LED,
							 uint8_t sum_rly,
							 uint8_t ab_rly) {
	struct singleOutputUI uiFSM;
	uiFSM.straightButton	= straight_button;
	uiFSM.crossButton		= cross_button;
	uiFSM.straightLED		= straight_LED;
	uiFSM.crossLED			= cross_LED;
	uiFSM.sumSltRly			= sum_rly;
	uiFSM.abSltRly			= ab_rly;
	
	uiFSM.straightBtn	= initDebounce(uiFSM.straightButton);
	uiFSM.crossBtn		= initDebounce(uiFSM.crossButton);
	
	uiFSM.state = UI_STRAIGHT;
	setUIFSMOutputs(uiFSM);
	
	return uiFSM;
}
	

/*
	Function: set FSM outputs based on the current state
		Input:	struct containing fsm info
		Output:	set LEDs and relays
		Return: none
*/
void setUIFSMOutputs(struct singleOutputUI fsm) {
	/* Set outputs based on state */
	switch(fsm.state) {
		case UI_STRAIGHT:
			setRelay(fsm.sumSltRly, RLY_BYPASS);
			setRelay(fsm.abSltRly, RLY_BYPASS);
			setLED(fsm.straightLED, 1);
			setLED(fsm.crossLED, 0);
			break;
		case UI_CROSS:
			setRelay(fsm.sumSltRly, RLY_BYPASS);
			setRelay(fsm.abSltRly, RLY_ACTIVE);
			setLED(fsm.straightLED, 0);
			setLED(fsm.crossLED, 1);
			break;
		case UI_SUM:
			setRelay(fsm.sumSltRly, RLY_ACTIVE);
			// no need to change the abSltRly
			setLED(fsm.straightLED, 1);
			setLED(fsm.crossLED, 1);
			break;
		default:
			break;
	}
}

/*
	Function: read the inputs and update the state machine
		Input:	struct containing fsm info
		Output: mutates fsm struct with input info
		Return: none
		
*/	
void updateUIFSM(struct singleOutputUI fsm) {
	/* Check inputs for falling edge (button is released) */
	updateDebounce(fsm.straightBtn);
	updateDebounce(fsm.crossBtn);
	
	uint8_t straight = (fsm.straightBtn.output == BTN_FALLING);
	uint8_t cross = (fsm.crossBtn.output == BTN_FALLING);
	
	uint8_t isStateChanged = 0;	// flag to determine if outputs need to update
	/* Set next state and set flag if state has changed */
	switch(fsm.state) {
		case UI_STRAIGHT:
			if (straight & (!cross)) {
				fsm.state = UI_CROSS;
				isStateChanged = 1;
			}
			else if ((!straight) & cross) {
				fsm.state = UI_SUM;
				isStateChanged = 1;
			}
			break;
		case UI_CROSS:
			if (straight & (!cross)) {
				fsm.state = UI_SUM;
				isStateChanged = 1;
			}
			else if ((!straight) & cross) {
				fsm.state = UI_STRAIGHT;
				isStateChanged = 1;
			}
			break;
		case UI_SUM:
			if ((!straight) & cross) {
				fsm.state = UI_STRAIGHT;
				isStateChanged = 1;
			}
			else if (straight & (!cross)) {
				fsm.state = UI_CROSS;
				isStateChanged = 1;
			}
			break;
	}
	
	/* if state has changed, update outputs */
	if (isStateChanged) {
		setUIFSMOutputs(fsm);
	}
}


/***************************************************************
	Receiver
****************************************************************/

/*
	Function: initialize receiver
*/
struct receiverFSM initReceiver(uint8_t detect0_pin,
								uint8_t detect1_pin,
								uint8_t vcc_slt0_pin,
								uint8_t vcc_slt1_pin,
								uint8_t vcc9_pin,
								uint8_t vcc12_pin,
								uint8_t rly_pin) {
	struct receiverFSM receiver;
	receiver.detect0	= detect0_pin;
	receiver.detect1	= detect1_pin;
	receiver.vcc_slt0	= vcc_slt0_pin;
	receiver.vcc_slt1	= vcc_slt1_pin;
	receiver.vcc9		= vcc9_pin;
	receiver.vcc12		= vcc12_pin;
	receiver.rly		= rly_pin;
	
	receiver.detect0BTN = initDebounce(receiver.detect0);
	receiver.detect1BTN = initDebounce(receiver.detect1);
	
	receiver.detect0State; // = READ CURRENT POSITION
	receiver.detect1State; // = READ CURRENT POSITION
	
	receiver.state; // FIGURE OUT STATE
	
	return receiver;
}
