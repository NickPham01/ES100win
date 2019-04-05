/* constants.h */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>
#include <avr/io.h>


/*
	uC settings
*/
#define F_CPU 1000000UL  // 1 MHz

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

// UI LEDs
#define UI_LED0_PIN		44
#define UI_LED1_PIN		45
#define UI_LED2_PIN		46
#define UI_LED3_PIN		47
#define UI_LED4_PIN		48
#define UI_LED5_PIN		1

// Routing (Relays)
#define K1P_PIN			13
#define K1N_PIN			12
#define K2P_PIN			21
#define K2N_PIN			20
#define K3P_PIN			27
#define K3N_PIN			26
#define K4P_PIN			33
#define K4N_PIN			32
#define RLY1			1
#define RLY2			2
#define RLY3			3
#define RLY4			4

// Receiver 1
#define VCC_9V_1_PIN	10
#define VCC_12V_1_PIN	11
#define RLYP_1_PIN		23
#define RLYN_1_PIN		22
#define VCC_SLT0_1_PIN	16
#define VCC_SLT1_1_PIN	17
#define DETECT0_1_PIN	18
#define DETECT1_1_PIN	19
#define RLY_1_NUM		5


// Receiver 2
#define VCC_9V_2_PIN	30
#define VCC_12V_2_PIN	31
#define RLYP_2_PIN		25
#define RLYN_2_PIN		24
#define VCC_SLT0_2_PIN	34
#define VCC_SLT1_2_PIN	35
#define DETECT0_2_PIN	38
#define DETECT1_2_PIN	39
#define RLY_2_NUM		6

// I2C
#define SDA_PIN			36
#define SCL_PIN			37

// Unused connectors
#define MISC0_PIN		2		// P16 pin 1
#define MISC1_PIN		3		// P16 pin 2
#define MISC2_PIN		8		// P15 pin 1
#define MICS3_PIN		9		// P15 pin 2

// pin info: array of structs which each contain pin offset, port, ...
typedef struct pin_t{
	uint8_t num;	// the pin number used for accessing within port
	PORT_t *p_port;	// a pointer to the port (PORTA is #defined as dereferrencing the value at address 0x400 as a PORT_t*, so pass a reference to this address instead its contents)
} pin_t;

pin_t *PINS[48];

/************************************************************************
 Pin/Port Definitions
	Note: order of arrays is important and can be used for access
 ***********************************************************************/

/************************************************************************
 Utility Macros
 ***********************************************************************/

#define bitON(REGISTER, BIT)	REGISTER |= (1<<BIT)
#define bitOFF(REGISTER, BIT)	REGISTER &= ~(1<<BIT)
#define bitREAD(BYTE, BIT)		((BYTE & (1<<BIT)) >> BIT)

#endif