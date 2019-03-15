#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <avr/io.h>

/*
	uC settings
*/
#define F_CPU 16000000UL  // 16 MHz


/*
	enums
*/
enum relayState		{RLY_BYPASS, RLY_ACTIVE, RLY_IDLE};
enum pinConfig		{INPUT_PIN, OUTPUT_PIN, PULLUP_PIN};
enum receiverState	{RECEIVER_BYPASS, RECEIVER_ACTIVE};
enum stateUI		{UI_STRAIGHT, UI_CROSS, UI_SUM};
enum debounceState	{DEBOUNCE_RESET, DEBOUNCE_WAIT};
enum btnOutputs		{BTN_IDLE, BTN_RISING, BTN_FALLING};	// RISING = depress, FALLING = release




/*
	To access the microcontroller's pin and port number, there are two arrays,
	"pins" and "ports" which contain at index i the pin number and port number
	of i, respectively.  To access i, the functional name of the pins are defined
	according to their physical pin locations (1:48).  
*/

/************************************************************************
 Pin/Port Definitions
	Note: order of arrays is important and can be used for access
 ***********************************************************************/

// UI Switches
#define UI_SW0_PIN		4
#define UI_SW1_PIN		5
#define UI_SW2_PIN		6
#define UI_SW3_PIN		7
const uint8_t UIswitches[] = {UI_SW0_PIN, UI_SW1_PIN, UI_SW2_PIN, UI_SW3_PIN};

// UI LEDs
#define UI_LED0_PIN		44
#define UI_LED1_PIN		45
#define UI_LED2_PIN		46
#define UI_LED3_PIN		47
#define UI_LED4_PIN		48
#define UI_LED5_PIN		1
const uint8_t UILEDs[] = {UI_LED0_PIN, UI_LED1_PIN, UI_LED2_PIN, UI_LED3_PIN, UI_LED4_PIN, UI_LED5_PIN};

// Routing (Relays)
#define K1P_PIN			13
#define K1N_PIN			12
#define K2P_PIN			21
#define K2N_PIN			20
#define K3P_PIN			27
#define K3N_PIN			26
#define K4P_PIN			33
#define K4N_PIN			32
const uint8_t routingPins[] = {K1P_PIN, K1N_PIN, K2P_PIN, K2N_PIN, K3P_PIN, K3N_PIN, K4P_PIN, K4N_PIN};

// Receiver 1
#define VCC_9V_1_PIN	10
#define VCC_12V_1_PIN	11
#define RLYP_1_PIN		23
#define RLYN_1_PIN		22
#define VCC_SLT0_1_PIN	16
#define VCC_SLT1_1_PIN	17
#define DETECT0_1_PIN	18
#define DETECT1_1_PIN	19
const uint8_t r1outPins[]	= {VCC_9V_1_PIN, VCC_12V_1_PIN, RLYP_1_PIN, RLYN_1_PIN};
const uint8_t r1inPins[]	= {VCC_SLT0_1_PIN, VCC_SLT1_1_PIN, DETECT0_1_PIN, DETECT1_1_PIN};


// Receiver 2
#define VCC_9V_2_PIN	30
#define VCC_12V_2_PIN	31
#define RLYP_2_PIN		25
#define RLYN_2_PIN		24
#define VCC_SLT0_2_PIN	34
#define VCC_SLT1_2_PIN	35
#define DETECT0_2_PIN	38
#define DETECT1_2_PIN	39
const uint8_t r2outPins[]	= {VCC_9V_2_PIN, VCC_12V_2_PIN, RLYP_2_PIN, RLYN_2_PIN};
const uint8_t r2inPins[]	= {VCC_SLT0_2_PIN, VCC_SLT1_2_PIN, DETECT0_2_PIN, DETECT1_2_PIN};

// I2C
#define SDA_PIN			36
#define SCL_PIN			37
const uint8_t i2cPins[] = {SDA_PIN, SCL_PIN};

// Unused connectors
#define MISC0_PIN		2		// P16 pin 1
#define MISC1_PIN		3		// P16 pin 2
#define MISC2_PIN		8		// P15 pin 1
#define MICS3_PIN		9		// P15 pin 2
const uint8_t unusedPins[] = {MISC0_PIN, MISC1_PIN, MISC2_PIN, MICS3_PIN};

// pin info: array of structs which each contain pin offset, port, ...
typedef struct pin_t{
	uint8_t num;	// the pin number used for accessing within port
	PORT_t *p_port;	// a pointer to the port (PORTA is #defined as dereferrencing the value at address 0x400 as a PORT_t*, so pass a reference to this address instead its contents)
} pin_t;

const pin_t pinA0 = {0, &PORTA};
const pin_t pinA1 = {1, &PORTA};
const pin_t pinA2 = {2, &PORTA};
const pin_t pinA3 = {3, &PORTA};
const pin_t pinA4 = {4, &PORTA};
const pin_t pinA5 = {5, &PORTA};
const pin_t pinA6 = {6, &PORTA};
const pin_t pinA7 = {7, &PORTA};
	
const pin_t pinB0 = {0, &PORTB};
const pin_t pinB1 = {1, &PORTB};
const pin_t pinB2 = {2, &PORTB};
const pin_t pinB3 = {3, &PORTB};
const pin_t pinB4 = {4, &PORTB};
const pin_t pinB5 = {5, &PORTB};
	
const pin_t pinC0 = {0, &PORTC};
const pin_t pinC1 = {1, &PORTC};
const pin_t pinC2 = {2, &PORTC};
const pin_t pinC3 = {3, &PORTC};
const pin_t pinC4 = {4, &PORTC};
const pin_t pinC5 = {5, &PORTC};
const pin_t pinC6 = {6, &PORTC};
const pin_t pinC7 = {7, &PORTC};
	
const pin_t pinD0 = {0, &PORTD};
const pin_t pinD1 = {1, &PORTD};
const pin_t pinD2 = {2, &PORTD};
const pin_t pinD3 = {3, &PORTD};
const pin_t pinD4 = {4, &PORTD};
const pin_t pinD5 = {5, &PORTD};
const pin_t pinD6 = {6, &PORTD};
const pin_t pinD7 = {7, &PORTD};
	
const pin_t pinE0 = {0, &PORTE};
const pin_t pinE1 = {1, &PORTE};
const pin_t pinE2 = {2, &PORTE};
const pin_t pinE3 = {3, &PORTE};
	
const pin_t pinF0 = {0, &PORTF};
const pin_t pinF1 = {1, &PORTF};
const pin_t pinF2 = {2, &PORTF};
const pin_t pinF3 = {3, &PORTF};
const pin_t pinF4 = {4, &PORTF};
const pin_t pinF5 = {5, &PORTF};
const pin_t pinF6 = {6, &PORTF};

const pin_t *PINS[48] = {
	&pinA5,		// 1
	&pinA6,		// 2
	&pinA7,		// 3
	&pinB0,		// 4
	&pinB1,		// 5
	&pinB2,		// 6
	&pinB3,		// 7
	&pinB4,		// 8
	&pinB5,		// 9
	&pinC0,		// 10
	&pinC1,		// 11
	&pinC2,		// 12
	&pinC3,		// 13
	0,			// 14 VDD
	0,			// 15 GND
	&pinC4,		// 16
	&pinC5,		// 17
	&pinC6,		// 18
	&pinC7,		// 19
	&pinD0,		// 20
	&pinD1,		// 21
	&pinD2,		// 22
	&pinD3,		// 23
	&pinD4,		// 24
	&pinD5,		// 25
	&pinD6,		// 26
	&pinD7,		// 27
	0,			// 28 AVDD
	0,			// 29 GND
	&pinE0,		// 30
	&pinE1,		// 31
	&pinE2,		// 32
	&pinE3,		// 33
	&pinF0,		// 34
	&pinF1,		// 35
	&pinF2,		// 36
	&pinF3,		// 37
	&pinF4,		// 38
	&pinF5,		// 39
	&pinF6,		// 40
	0,			// 41 UPDI
	0,			// 42 VDD
	0,			// 43 GND
	&pinA0,		// 44
	&pinA1,		// 45
	&pinA2,		// 46
	&pinA3,		// 47
	&pinA4		// 48
};

/************************************************************************
 Utility Macros
 ***********************************************************************/

#define bitON(REGISTER, BIT)	REGISTER |= (1<<BIT)
#define bitOFF(REGISTER, BIT)	REGISTER &= ~(1<<BIT)
#define bitREAD(BYTE, BIT)		((BYTE & (1<<BIT)) >> BIT)

#define gpioON(PIN)				bitON(PINS[PIN]->p_port->OUT, PINS[PIN]->num)
#define gpioOFF(PIN)			bitOFF(PINS[PIN]->p_port->OUT, PINS[PIN]->num)
#define gpioREAD(PIN)			bitREAD(PINS[PIN]->p_port->IN, PINS[PIN]->num)

#endif