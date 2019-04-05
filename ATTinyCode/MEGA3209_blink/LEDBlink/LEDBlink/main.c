/*
 * LEDBlink.c
 *
 * Created: 3/15/2019 10:26:18 AM
 * Author : NPham
 */ 

#define F_CPU 3200000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

/*******************************************************************************************************
	MACROS and DEFINES
*******************************************************************************************************/

/* Pin and Port definitions */
#define K1P_PIN						3
#define K1P_PORT					PORTC
#define K1N_PIN						2
#define K1N_PORT					PORTC

#define K2P_PIN						1
#define K2P_PORT					PORTD
#define K2N_PIN						0
#define K2N_PORT					PORTD

#define K3P_PIN						7
#define K3P_PORT					PORTD
#define K3N_PIN						6
#define K3N_PORT					PORTD

#define K4P_PIN						3
#define K4P_PORT					PORTE
#define K4N_PIN						2
#define K4N_PORT					PORTE

#define R1KP_PIN					3
#define R1KP_PORT					PORTD
#define R1KN_PIN					2
#define R1KN_PORT					PORTD

#define R2KP_PIN					5
#define R2KP_PORT					PORTD
#define R2KN_PIN					4
#define R2KN_PORT					PORTD

#define VCC_9V_1_PIN				0
#define VCC_9V_1_PORT				PORTC
#define VCC_12V_1_PIN				1
#define VCC_12V_1_PORT				PORTC

#define VCC_9V_2_PIN				0
#define VCC_9V_2_PORT				PORTE
#define VCC_12V_2_PIN				1
#define VCC_12V_2_PORT				PORTE

#define DETECT0_1_PIN				6
#define DETECT0_1_PORT				PORTC
#define DETECT1_1_PIN				7
#define DETECT1_1_PORT				PORTC

#define DETECT0_2_PIN				4
#define DETECT0_2_PORT				PORTF
#define DETECT1_2_PIN				5
#define DETECT1_2_PORT				PORTF

/* Bit masks */
#define BOTTOM_STRAIGHT_BTN_NUM		0
#define BOTTOM_STRAIGHT_BTN_MASK	0x01
#define TOP_STRAIGHT_BTN_NUM		3
#define TOP_STRAIGHT_BTN_MASK		0x08
#define UP_STRAIGHT_BTN_NUM			1	
#define UP_CROSS_BTN_MASK			0x02
#define DOWN_CROSS_BTN_NUM			2
#define DOWN_CROSS_BTN_MASK			0x04

/* Bit Utilities */
#define bitON(BYTE, BIT)			BYTE |= (1<<BIT)
#define bitOFF(BYTE, BIT)			BYTE &= ~(1<<BIT)
#define bitREAD(BYTE, BIT)			((BYTE & (1<<BIT)) >> BIT)
#define gpioON(GPIO_PORT, BIT)		GPIO_PORT.OUTSET = 1<<BIT
#define gpioOFF(GPIO_PORT, BIT)		GPIO_PORT.OUTCLR = 1<<BIT
#define gpioREAD(GPIO_PORT, BIT)	((GPIO_PORT.IN & (1<<BIT)) >> BIT)

/*******************************************************************************************************
	GLOBALS
*******************************************************************************************************/

/* Declare Global Variables */
uint8_t switch_vals;
uint8_t bottom_straight_button_state;
uint8_t top_straight_button_state;
uint8_t up_cross_button_state;
uint8_t down_cross_button_state;

uint8_t bottom_straight_button_prevstate;
uint8_t top_straight_button_prevstate;
uint8_t up_cross_button_prevstate;
uint8_t down_cross_button_prevstate;

/*******************************************************************************************************
	ENUMS
*******************************************************************************************************/

enum UIposition {BOTTOM_STRAIGHT, TOP_STRAIGHT, UP_CROSS, DOWN_CROSS};
enum relayState {RELAY_ACTIVE, RELAY_BYPASS, RELAY_IDLE};
enum routingState {ROUTING_STATE_STRAIGHT, ROUTING_STATE_CROSS, ROUTING_STATE_SUM};
enum routingID {ROUTING_ID_TOP, ROUTING_ID_BOTTOM};
enum relayID {K1, K2, K3, K4, R1K, R2K};
enum receiverID {RECEIVER_1, RECEIVER_2};

/*******************************************************************************************************
	LOW LEVEL INTERFACE FUNCTIONS
*******************************************************************************************************/

// LED functions:
void setLED(enum UIposition LED, uint8_t state) {
	uint8_t data = 0;
	switch(LED) {
		case BOTTOM_STRAIGHT:
		data = 1<<0;
		break;
		
		case TOP_STRAIGHT:
		data = 1<<1;
		break;
		
		case UP_CROSS:
		data = 1<<5;
		break;
		
		case DOWN_CROSS:
		data = 1<<4;
		break;
	}
	
	if (state) {
		PORTA.OUTSET = data;
	}
	else {
		PORTA.OUTCLR = data;
	}
};

void setDebugLED(enum receiverID receiver, uint8_t state) {
	if (receiver == RECEIVER_1) {
		if (state) {
			gpioON(PORTA, 2);
		} else {
			gpioOFF(PORTA, 2);
		}
	} else {
		if (state) {
			gpioON(PORTA, 3);
			} else {
			gpioOFF(PORTA, 3);
		}
	}
}

// check switches
uint8_t readSwitches(void) {
	uint8_t portb_in = (~VPORTB.IN) & 0x0F;
	return portb_in;
}

// set all leds
void setAllLEDs(uint8_t value) {
	setLED(BOTTOM_STRAIGHT, value);
	setLED(TOP_STRAIGHT, value);
	setLED(UP_CROSS, value);
	setLED(DOWN_CROSS, value);
}
/*
// read VCC switches
uint8_t readVCCswitches(enum receiverID receiver) {
	
}
*/

// set receiver VCC
void setReceiverVCC(enum receiverID receiver, uint8_t voltage) {
	if (receiver == RECEIVER_1)	{
		switch(voltage) {
			case 18:
			gpioOFF(VCC_9V_1_PORT, VCC_9V_1_PIN);
			gpioOFF(VCC_12V_1_PORT, VCC_12V_1_PIN);
			break;
			
			case 12:
			gpioOFF(VCC_9V_1_PORT, VCC_9V_1_PIN);
			gpioON(VCC_12V_1_PORT, VCC_12V_1_PIN);
			
			default:	// 9
			gpioON(VCC_9V_1_PORT, VCC_9V_1_PIN);
			gpioOFF(VCC_12V_1_PORT, VCC_12V_1_PIN);
			break;
		}
	}
	else if (receiver == RECEIVER_2) {
		switch(voltage) {
			case 18:
			gpioOFF(VCC_9V_2_PORT, VCC_9V_2_PIN);
			gpioOFF(VCC_12V_2_PORT, VCC_12V_2_PIN);
			break;
			
			case 12:
			gpioOFF(VCC_9V_2_PORT, VCC_9V_2_PIN);
			gpioON(VCC_12V_2_PORT, VCC_12V_2_PIN);
			
			default:	// 9
			gpioON(VCC_9V_2_PORT, VCC_9V_2_PIN);
			gpioOFF(VCC_12V_2_PORT, VCC_12V_2_PIN);
			break;
		}
	}
}

// check plate detect
uint8_t readPlateSensor(enum receiverID receiver) {
	uint8_t sensorReading = 0;
	if (receiver == RECEIVER_1)	 {
		sensorReading = ((~PORTC.IN) & 0xC0) >> DETECT0_1_PIN;
	} else if (receiver == RECEIVER_2) {
		sensorReading = ((~PORTF.IN) & 0x30) >> DETECT0_2_PIN;
	}
	
	// now should just be first two bits, active high
	return sensorReading;
}

// check if plate has changed
uint8_t checkForPlateChange(enum receiverID receiver, uint8_t prevKnownState) {
	uint8_t newState = readPlateSensor(receiver);

	if (newState != prevKnownState) {
		return newState;
	}
	
	// if no change, return -1
	return UINT8_MAX;
}

inline enum relayState decodePlateState(uint8_t plateState) {
	return plateState == 0x03 ? RELAY_ACTIVE : RELAY_BYPASS;	
}



// set relay
void setRelay(enum relayID relay, enum relayState state) {
	uint16_t pulse_width = 10;
	switch(relay) {
		case K1:
			switch(state) {
				case RELAY_ACTIVE:
					gpioON(K1P_PORT, K1P_PIN);
					gpioOFF(K1N_PORT, K1N_PIN);
					break;
			
				case RELAY_BYPASS:
					gpioOFF(K1P_PORT, K1P_PIN);
					gpioON(K1N_PORT, K1N_PIN);
					break;
				default:
					break;
			}
			_delay_ms(pulse_width);
			gpioON(K1P_PORT, K1P_PIN);
			gpioON(K1N_PORT, K1N_PIN);
			break;
		
		case K2:
			switch(state) {
				case RELAY_ACTIVE:
				gpioON(K2P_PORT, K2P_PIN);
				gpioOFF(K2N_PORT, K2N_PIN);
				break;
				
				case RELAY_BYPASS:
				gpioOFF(K2P_PORT, K2P_PIN);
				gpioON(K2N_PORT, K2N_PIN);
				break;
				
				default:
				break;
			}
			_delay_ms(pulse_width);
			gpioON(K2P_PORT, K2P_PIN);
			gpioON(K2N_PORT, K2N_PIN);
			break;
		break;
		
		case K3:
			switch(state) {
				case RELAY_ACTIVE:
				gpioON(K3P_PORT, K3P_PIN);
				gpioOFF(K3N_PORT, K3N_PIN);
				break;
				
				case RELAY_BYPASS:
				gpioOFF(K3P_PORT, K3P_PIN);
				gpioON(K3N_PORT, K3N_PIN);
				break;
				
				default:
				break;
			}
			_delay_ms(pulse_width);
			gpioON(K3P_PORT, K3P_PIN);
			gpioON(K3N_PORT, K3N_PIN);
			break;
		break;
		
		case K4:
			switch(state) {
				case RELAY_ACTIVE:
				gpioON(K4P_PORT, K4P_PIN);
				gpioOFF(K4N_PORT, K4N_PIN);
				break;
				
				case RELAY_BYPASS:
				gpioOFF(K4P_PORT, K4P_PIN);
				gpioON(K4N_PORT, K4N_PIN);
				break;
				
				default:
				break;
			}
			_delay_ms(pulse_width);
			gpioON(K4P_PORT, K4P_PIN);
			gpioON(K4N_PORT, K4N_PIN);
			break;
		break;
		
		case R1K:
			switch(state) {
				case RELAY_ACTIVE:
				gpioON(R1KP_PORT, R1KP_PIN);
				gpioOFF(R1KN_PORT, R1KN_PIN);
				setDebugLED(RECEIVER_1, 1);
				break;
				
				case RELAY_BYPASS:
				gpioOFF(R1KP_PORT, R1KP_PIN);
				gpioON(R1KN_PORT, R1KN_PIN);
				setDebugLED(RECEIVER_1, 0);
				break;
				
				default:
				break;
			}
			_delay_ms(pulse_width);
			gpioON(R1KP_PORT, R1KP_PIN);
			gpioON(R1KN_PORT, R1KN_PIN);
			break;
		break;
		
		case R2K:
			switch(state) {
				case RELAY_ACTIVE:
				gpioON(R2KP_PORT, R2KP_PIN);
				gpioOFF(R2KN_PORT, R2KN_PIN);
				setDebugLED(RECEIVER_2, 1);
				break;
				
				case RELAY_BYPASS:
				gpioOFF(R2KP_PORT, R2KP_PIN);
				gpioON(R2KN_PORT, R2KN_PIN);
				setDebugLED(RECEIVER_2, 0);
				break;
				
				default:
				break;
			}
			_delay_ms(pulse_width);
			gpioON(R2KP_PORT, R2KP_PIN);
			gpioON(R2KN_PORT, R2KN_PIN);
			break;
		break;	
	}
}


// set routing relays
void setRoutingRelays(uint8_t output, enum routingState state) {
	switch(output) {
		case 1:
			switch(state) {
				case ROUTING_STATE_STRAIGHT:
					setRelay(K1, RELAY_BYPASS);
					setRelay(K2, RELAY_BYPASS);
					setLED(BOTTOM_STRAIGHT, 1);
					setLED(DOWN_CROSS, 0);
					break;
				case ROUTING_STATE_CROSS:
					setRelay(K1, RELAY_BYPASS);
					setRelay(K2, RELAY_ACTIVE);
					setLED(BOTTOM_STRAIGHT, 0);
					setLED(DOWN_CROSS, 1);
					break;
				case ROUTING_STATE_SUM:
					setRelay(K1, RELAY_ACTIVE);
					setLED(BOTTOM_STRAIGHT, 1);
					setLED(DOWN_CROSS, 1);
					break;
			}
			break;
		default:	// output 2
			switch(state) {
				case ROUTING_STATE_STRAIGHT:
					setRelay(K4, RELAY_BYPASS);
					setRelay(K3, RELAY_BYPASS);
					setLED(TOP_STRAIGHT, 1);
					setLED(UP_CROSS, 0);
					break;
				case ROUTING_STATE_CROSS:
					setRelay(K4, RELAY_BYPASS);
					setRelay(K3, RELAY_ACTIVE);
					setLED(TOP_STRAIGHT, 0);
					setLED(UP_CROSS, 1);
					break;
				case ROUTING_STATE_SUM:
					setRelay(K4, RELAY_ACTIVE);
					setLED(TOP_STRAIGHT, 1);
					setLED(UP_CROSS, 1);
					break;
			}
			break;	
	}
}

/*******************************************************************************************************
	INITIALIZATION
********************************************************************************************************/

// Initialize
void initialize(void) {
	// Set LEDs to output
	PORTA.DIRSET = 0x3F;	// 0b 0011 0011		all leds
	
	// Set Relays to output
	PORTC.DIRSET = 0x0C;	// 0b 0000 1100		K1 pins
	PORTD.DIRSET = 0xFF;	// 0b 1111 1111		K2, K3, K5, and K6 pins
	PORTE.DIRSET = 0x0C;	// 0b 0000 1100		K4 pins
	
	// Set VCC control pins to output
	PORTC.DIRSET = 0x03;	// 0b 0000 0011		Receiver 1 VCC control pins
	PORTE.DIRSET = 0x03;	// 0b 0000 0011		Receiver 2 VCC control pins
	
	// Set switches to input
	PORTB.DIRCLR = 0x0F;	// 0b 0000 1111		all UI switches

	// Set plate detect to input
	PORTC.DIRCLR = 0xC0;	// 0b 1100 0000		plate detect0 and 1 for Receiver 1
	PORTF.DIRCLR = 0x30;	// 0b 0011 0000		plate detect0 and 1 for Receiver 2
	
	// set switches to have pullups enabled
	PORTB.PIN0CTRL |= 0x08;
	PORTB.PIN1CTRL |= 0x08;
	PORTB.PIN2CTRL |= 0x08;
	PORTB.PIN3CTRL |= 0x08;
	
	// set plate detect pins to have pullups enabled
	PORTC.PIN6CTRL |= 0x08;
	PORTC.PIN7CTRL |= 0x08;
	PORTF.PIN4CTRL |= 0x08;
	PORTF.PIN5CTRL |= 0x08;

	// set up the timer
	//setupTimer();
	
	// initialize the values of the button states
	switch_vals = readSwitches();
	bottom_straight_button_state	= switch_vals & BOTTOM_STRAIGHT_BTN_MASK;
	top_straight_button_state		= switch_vals & TOP_STRAIGHT_BTN_MASK;
	up_cross_button_state			= switch_vals & UP_CROSS_BTN_MASK;
	down_cross_button_state			= switch_vals & DOWN_CROSS_BTN_MASK;

}

/*******************************************************************************************************
	FSMs
********************************************************************************************************/

enum routingState topRoutingUIState;
enum routingState bottomRoutingUIState;

void updateRoutingState(enum routingState *currentState, uint8_t new_straight_button_state, uint8_t new_cross_button_state) {
	enum routingState newState = *currentState;
	
	switch(*currentState) {
		case ROUTING_STATE_STRAIGHT:
			if (new_straight_button_state) {
				newState = ROUTING_STATE_CROSS;
			}
			else if (new_cross_button_state) {
				newState = ROUTING_STATE_SUM;
			}
			break;
			
		case ROUTING_STATE_CROSS:
			if (new_straight_button_state) {
				newState = ROUTING_STATE_SUM;
			}
			else if (new_cross_button_state) {
				newState = ROUTING_STATE_STRAIGHT;
			}
			break;
		
		case ROUTING_STATE_SUM:
			if (new_straight_button_state) {
				newState = ROUTING_STATE_CROSS;
			}
			else if (new_cross_button_state) {
				newState = ROUTING_STATE_STRAIGHT;
			}
			break;
	}
	
	if (newState != *currentState) {
		setRoutingRelays(1, newState);
		*currentState = newState;
	}
}

void updateReceiver(enum receiverID receiver, uint8_t *prevstate, uint8_t *newstate) {
	*newstate = checkForPlateChange(receiver, *prevstate);
	
	// setReceiverVCC(receiver, readVCCswitches(receiver));
	
	if (*newstate != UINT8_MAX) {
		*prevstate = *newstate;
		
		if (receiver == RECEIVER_1) {
			setRelay(R1K, decodePlateState(*newstate));
		} else if (receiver == RECEIVER_2) {
			setRelay(R2K, decodePlateState(*newstate));
		}
	}

}

/*******************************************************************************************************
	MAIN
********************************************************************************************************/


int main (void) {
	initialize();
	setReceiverVCC(RECEIVER_1, 9);
	setReceiverVCC(RECEIVER_2, 9);
	
	//uint8_t switch_vals;	uint8_t receiver1_prevstate = UINT8_MAX - 1;	uint8_t receiver1_newstate	= 0;	uint8_t receiver2_prevstate = UINT8_MAX - 1;	uint8_t receiver2_newstate	= 0;		setRoutingRelays(1, ROUTING_STATE_CROSS);	setRoutingRelays(2, ROUTING_STATE_CROSS);		_delay_ms(500);		setRoutingRelays(1, ROUTING_STATE_SUM);	setRoutingRelays(2, ROUTING_STATE_SUM);		_delay_ms(500);	setRoutingRelays(1, ROUTING_STATE_STRAIGHT);	setRoutingRelays(2, ROUTING_STATE_STRAIGHT);		_delay_ms(500);		while(1) {				updateReceiver(RECEIVER_1, &receiver1_prevstate, &receiver1_newstate);		updateReceiver(RECEIVER_2, &receiver2_prevstate, &receiver2_newstate);				switch_vals = readSwitches();		bottom_straight_button_state	= bitREAD(switch_vals, 0);		up_cross_button_state			= bitREAD(switch_vals, 1);		down_cross_button_state			= bitREAD(switch_vals, 2);		top_straight_button_state		= bitREAD(switch_vals, 3);						//updateRoutingState(&topRoutingUIState, top_straight_button_state, up_cross_button_state);		updateRoutingState(&bottomRoutingUIState, bottom_straight_button_state, down_cross_button_state);				_delay_ms(100);	}
}





/* Interrupts for timing 
volatile unsigned long timer_millis = 0;

void setupTimer(void) {
	TCA0.SINGLE.CTRLA	|= 0x09;	// turn on timer A with 1/64 prescalar (250kHz)
	TCA0.SINGLE.INTCTRL	|= 0x10;	// enable compare channel 0 interrupt
	TCA0.SINGLE.CMP0H	=  0x00;	// set high byte of compare register to 0x00
	TCA0.SINGLE.CMP0L	=  0xFA;	// set low byte of compare register to 250
}

ISR(__vectorCMP0) {
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		timer_millis++;
		TCA0.SINGLE.CNTH = 0x00;
		TCA0.SINGLE.CNTL = 0x00;
		TCA0.SINGLE.INTFLAGS |= (1<<4);	// clear flag by writing 1 to its location
	}
}

unsigned long get_time_ms(void) {
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = timer_millis;
	}
	return millis_return;
}
*/