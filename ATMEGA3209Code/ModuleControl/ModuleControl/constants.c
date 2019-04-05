/* constants.c */


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
const uint8_t UIswitches[]		= {UI_SW0_PIN, UI_SW1_PIN, UI_SW2_PIN, UI_SW3_PIN};
const uint8_t numUIswitches		= 4;

// UI LEDs
const uint8_t UILEDs[]			= {UI_LED0_PIN, UI_LED1_PIN, UI_LED2_PIN, UI_LED3_PIN, UI_LED4_PIN, UI_LED5_PIN};
const uint8_t numUILEDs			= 6;

// Routing (Relays)
const uint8_t routingPins[]		= {K1P_PIN, K1N_PIN, K2P_PIN, K2N_PIN, K3P_PIN, K3N_PIN, K4P_PIN, K4N_PIN};
const uint8_t numRoutingPins	= 8;

// Receiver 1
const uint8_t r1outPins[]		= {VCC_9V_1_PIN, VCC_12V_1_PIN, RLYP_1_PIN, RLYN_1_PIN};
const uint8_t r1inPins[]		= {VCC_SLT0_1_PIN, VCC_SLT1_1_PIN, DETECT0_1_PIN, DETECT1_1_PIN};
const uint8_t numr1outPins		= 4;
const uint8_t numr1inPins		= 4;


// Receiver 2
const uint8_t r2outPins[]		= {VCC_9V_2_PIN, VCC_12V_2_PIN, RLYP_2_PIN, RLYN_2_PIN};
const uint8_t r2inPins[]		= {VCC_SLT0_2_PIN, VCC_SLT1_2_PIN, DETECT0_2_PIN, DETECT1_2_PIN};
const uint8_t numr2outPins		= 4;
const uint8_t numr2inPins		= 4;

// I2C
const uint8_t i2cPins[]			= {SDA_PIN, SCL_PIN};
const uint8_t numI2CPins		= 2;

// Unused connectors
const uint8_t unusedPins[]		= {MISC0_PIN, MISC1_PIN, MISC2_PIN, MICS3_PIN};
const uint8_t numUnusedPins		= 4;

// pin info: array of structs which each contain pin offset, port, ...
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
