/*
 * PlatePrototypeControl_V1.c
 *
 * Created: 1/12/2019 3:27:07 PM
 * Author : NPham
 */ 

#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/************************************************************************/
/* Pin Definitions                                                                     */
/************************************************************************/

#define LEDPIN				PINC0

#define VCC_SLT_9V_PIN		PINA1
#define VCC_SLT_12V_PIN		PINA0
#define RLY_IN1_PIN			PINA3
#define RLY_IN2_PIN			PINA2
#define P_DETECT0_PIN		PINA7
#define P_DETECT1_PIN		PINB0
#define VCC_SLT_IN0_PIN		PINA5
#define VCC_SLT_IN1_PIN		PINA6

#define VCC_SLT_9V_PORT		PORTA
#define VCC_SLT_12V_PORT	PORTA
#define RLY_IN1_PORT		PORTA
#define RLY_IN2_PORT		PORTA
#define P_DETECT0_PORT		PORTA
#define P_DETECT1_PORT		PORTB
#define VCC_SLT_IN0_PORT	PORTA
#define VCC_SLT_IN1_PORT	PORTA

/************************************************************************/
/* Utility Macros                                                                     */
/************************************************************************/

#define bitON(REGISTER, BIT)	REGISTER |= (1<<BIT)
#define bitOFF(REGISTER, BIT)	REGISTER &= ~(1<<BIT)

/************************************************************************/
/* LED Control Utilities                                                                     */
/************************************************************************/

void initializeBlinker(void) {
	DDRC |= (1<<LEDPIN);	// set pin PC0 to output
	
	TCCR1A |= (1<<TCW1);	// set timer 1 width to 16 bit mode
	TCCR1A |= (1<<CS11);	// set timer 1 clock divider = 8
	
	OCR1B = 0xF0;			// set ouptut comparison high byte first
	OCR1A = 0x00;			// set output comparison low byte 
							//   -> this triggers both bytes to be
							//      written to the 16-bit register
}

uint8_t updateBlink(void) {
	/* Returns 1 if the state changes, else 0 */
	if (TIFR & (1<<OCF1A)) {		// check if the comparison flag has been set
		TIFR	|= (1<<OCF1A);			// reset the flag
		TCNT1H	= 0x00;					// reset the counter (high bit first)
		TCNT1L	= 0x00;					// reset the counter (low bit second)
		PORTC	^= (1<<LEDPIN);			// toggle the LED
		return 1;
	}
	return 0;
}

void setLED(uint8_t status) {
	if (status) {
		bitON(PORTC, LEDPIN);
	}
	else {
		bitOFF(PORTC, LEDPIN);
	}
}

/************************************************************************/
/* uC Initializations                                                                     */
/************************************************************************/

void initializePins(void) {
	/* Set Vcc Select pins data direction to outputs */
	bitON(DDRA, VCC_SLT_9V_PIN);
	bitON(DDRA, VCC_SLT_12V_PIN);
	
	/* Set relay driver pins data direction to outputs */
	bitON(DDRA, RLY_IN1_PIN);
	bitON(DDRA, RLY_IN2_PIN);
	
	/* Set plate detect pins data direction to inputs and enable pullups */
	bitOFF(DDRA, P_DETECT0_PIN);
	bitOFF(DDRB, P_DETECT1_PIN);
	
	bitON(PUEA, P_DETECT0_PIN);
	bitON(PUEB, P_DETECT1_PIN);
}

/************************************************************************/
/* Vcc Control                                                                     */
/************************************************************************/
void setVcc(uint8_t voltage) {
	/* Set pins PA0 and PA1 (Vcc select) */
	switch(voltage) {
		case 9:							// can be combined with default
			// (PA0, PA1) = (0, 1)
			bitOFF(VCC_SLT_12V_PORT, VCC_SLT_12V_PIN);
			bitON(VCC_SLT_9V_PORT, VCC_SLT_9V_PIN);
			break;
		case 12:
			// (PA0, PA1) = (1, 0)
			bitON(VCC_SLT_12V_PORT, VCC_SLT_12V_PIN);
			bitOFF(VCC_SLT_9V_PORT, VCC_SLT_9V_PIN);
			break;
		case 18:
			bitOFF(VCC_SLT_12V_PORT, VCC_SLT_12V_PIN);
			bitOFF(VCC_SLT_9V_PORT, VCC_SLT_9V_PIN);
			break;
		default:
			// default to 9V for now
			bitOFF(VCC_SLT_12V_PORT, VCC_SLT_12V_PIN);
			bitON(VCC_SLT_9V_PORT, VCC_SLT_9V_PIN);
	}
}

/************************************************************************/
/* Relay Control                                                                     */
/************************************************************************/
enum {BYPASS, ACTIVE, IDLE} relayState;

/*
 * Helper function for performing the relay switching.
 * Sets the voltage across the relay coil.
 */
void activateRelay(uint8_t relayState) {
	switch(relayState) {
		case BYPASS:
			// (PA2, PA3) = (1, 0)
			bitON(RLY_IN2_PORT, RLY_IN2_PIN);
			bitOFF(RLY_IN1_PORT, RLY_IN1_PIN);
			break;
		case ACTIVE:
			// (PA2, PA3) = (0, 1)
			bitOFF(RLY_IN2_PORT, RLY_IN2_PIN);
			bitON(RLY_IN1_PORT, RLY_IN1_PIN);
			break;
		case IDLE:						// can be combined with default
			// (PA2, PA3) = (1, 1)
			bitON(RLY_IN2_PORT, RLY_IN2_PIN);
			bitON(RLY_IN1_PORT, RLY_IN1_PIN);
			break;
		default:
			// same as idle
			bitON(RLY_IN2_PORT, RLY_IN2_PIN);
			bitON(RLY_IN1_PORT, RLY_IN1_PIN);
	}
}

/*
 * Main relay switching function.  Call this one to set the relay
 */
void setRelay(uint8_t relayState) {
	activateRelay(relayState);	// engerize the relay coil in the derired direction
	_delay_ms(20);				// wait to ensure coil has been activated properly
	activateRelay(IDLE);		// set coil back to idle
	setLED(relayState);
}

/************************************************************************/
/* Plate Detection                                                                 */
/************************************************************************/

uint8_t readVccSwitch(void) {
	/* Read the sensors */
	uint8_t data = 0x00;
	data |= (PINA & (1<<VCC_SLT_IN0_PIN))>>VCC_SLT_IN0_PIN;			// set 0th bit
	data |= ((PINA & (1<<VCC_SLT_IN1_PIN))>>VCC_SLT_IN1_PIN)<<1;	// set 1st bit
	
	/* Decode data into desired voltage */
	switch(data) {
		case 0x00:
		return 9;
		break;
		case 0x01:
		return 12;
		break;
		case 0x02:
		return 18;
		break;
		default:
		return 9;
	}
}

uint8_t readPlateSensor(void) {
	
	/* Initialize and read the ports into the two lowest bits */
	uint8_t data = 0x00;
	data |= (PINA & (1<<P_DETECT0_PIN))>>P_DETECT0_PIN;			// set 0th bit
	data |= ((PINB & (1<<P_DETECT1_PIN))>>P_DETECT1_PIN)<<1;	// set 1st bit

	/*  The lowest two bits contain active low values of the plate sensor
		i.e. data = 0x03 if both switches are triggered and a plate is
		present.  Now invert these bits with XOR */
	data ^= 0x03;
	return data;
}
	
uint8_t checkForPlateChange(uint8_t prevKnownState) {
	/*	There are only 2 sensors, using the 2 lowest bits of the state
		variable, so checkForPlateChange() returns UINT8_MAX (255) if the state
		hasn't changed. */
	
	uint8_t newState = readPlateSensor();

	if (newState != prevKnownState) {
		return newState;
	}
	return UINT8_MAX;
}

inline uint8_t decodePlateState(uint8_t plateState) {
	return plateState == 0x03 ? ACTIVE : BYPASS;
}

inline void updateOutputs(uint8_t plateState) {
	setVcc(readVccSwitch());		// no need to de-bounce, since we've already waited for the plate detect pins to bounce
	setRelay(decodePlateState(plateState));
}


/************************************************************************/
/* Main Function                                                                     */
/************************************************************************/
int main(void) {
	initializeBlinker();
	PORTC = 1<<LEDPIN;
	
	initializePins();
	setVcc(9);

	uint8_t relayState = BYPASS;
	setRelay(relayState);

	/* Indicate that the chip has been flashed */
	for (uint8_t i = 0; i < 4; i++) {
		setLED(1);
		_delay_ms(100);
		setLED(0);
		_delay_ms(100);
	}

	uint8_t prevPlateState = UINT8_MAX - 1;
	uint8_t newPlateState = 0;

	while(1) {
		// check if the state of the pins has changed
		newPlateState = checkForPlateChange(prevPlateState);
		if (newPlateState != UINT8_MAX) {
			prevPlateState = newPlateState;
		
			// if so, decode and set the new outputs
			updateOutputs(newPlateState);
		}
	}

		
	return(1);
}

