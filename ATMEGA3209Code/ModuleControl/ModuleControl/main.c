/*
 * ModuleControl.c
 *
 * Created: 3/16/2019 10:25:59 PM
 * Author : NPham
 */ 

#include <avr/io.h>

#include <avr/delay.h>
#include "constants.h"
#include "initialize.h"
#include "utilities.h"
#include "fsms.h"


/* this is to run the full system: just call this once in main */
void runModuleControl(void) {
	
	/* First initialize the two receiver and UI FSMs */
	struct receiverFSM receiver1 = initReceiver(DETECT0_1_PIN,
												DETECT1_1_PIN,
												VCC_SLT0_1_PIN,
												VCC_SLT1_1_PIN,
												VCC_9V_1_PIN,
												VCC_12V_1_PIN,
												RLY_1_NUM);
	
	struct receiverFSM receiver2 = initReceiver(DETECT0_2_PIN,
												DETECT1_2_PIN,
												VCC_SLT0_2_PIN,
												VCC_SLT1_2_PIN,
												VCC_9V_2_PIN,
												VCC_12V_2_PIN,
												RLY_2_NUM);
	
	struct singleOutputUI out1UI = initUI(UI_SW1_PIN,
												UI_SW0_PIN,
												UI_LED0_PIN,
												UI_LED4_PIN,
												RLY1,
												RLY2);
	
	struct singleOutputUI out2UI = initUI(UI_SW3_PIN,
												UI_SW1_PIN,
												UI_LED2_PIN,
												UI_LED5_PIN,
												RLY4,
												RLY3);
	
	// in loop: just call update for the FSMs
	while (1) {
		updateReceiver(&receiver1);
		updateReceiver(&receiver2);
		updateUIFSM(&out1UI);
		updateUIFSM(&out2UI);
	}
}

/*
	Test code:
		1) confirm receivers can output 9, 12, 18 volts
		2) confirm receivers can switch relay
		3) confirm routing relays switch
		4) confirm LEDs can be controlled
*/

void testReceiverVCC(uint8_t voltage) {
	// set both receiver voltage regulators to the desired voltage
	setVcc(VCC_9V_1_PIN, VCC_12V_2_PIN, voltage);
	setVcc(VCC_9V_2_PIN, VCC_12V_2_PIN, voltage);
}

void testReceiverRelays(enum relayState rly_state) {
	// set both receivers to the desired state
	setRelay(5, rly_state);
	setRelay(6, rly_state);
}

void testRoutingRelays(enum stateUI state) {
	/* Set outputs for both routing circuits based on state */
	switch(state) {
		case UI_STRAIGHT:
		setRelay(1, RLY_BYPASS);
		setRelay(2, RLY_BYPASS);
		setLED(UI_LED0_PIN, 1);
		setLED(UI_LED4_PIN, 0);
		
		setRelay(4, RLY_BYPASS);
		setRelay(3, RLY_BYPASS);
		setLED(UI_LED2_PIN, 1);
		setLED(UI_LED5_PIN, 0);
		break;
		
		case UI_CROSS:
		setRelay(1, RLY_BYPASS);
		setRelay(2, RLY_ACTIVE);
		setLED(UI_LED0_PIN, 0);
		setLED(UI_LED4_PIN, 1);
		
		setRelay(4, RLY_BYPASS);
		setRelay(3, RLY_ACTIVE);
		setLED(UI_LED2_PIN, 0);
		setLED(UI_LED5_PIN, 1);
		break;
		
		case UI_SUM:
		setRelay(1, RLY_ACTIVE);
		// no need to change the abSltRly
		setLED(UI_LED0_PIN, 1);
		setLED(UI_LED4_PIN, 1);
		
		setRelay(4, RLY_ACTIVE);
		// no need to change the abSltRly
		setLED(UI_LED2_PIN, 1);
		setLED(UI_LED5_PIN, 1);
		break;
		default:
		break;
	}
}


int main(void) {	
	/* Configure LED0 pin as output */
	for (uint8_t i = 0; i < 6; i++) {
		PINS[UILEDs[i]]->p_port->DIRSET = 1<<(PINS[UILEDs[i]]->num);
	}
	
	uint16_t delay_time = 500;	// ms
	
	while(1) {
		for (uint8_t i = 0; i < 6; i++) {
			PINS[UILEDs[i]]->p_port->OUTSET = 1<<(PINS[UILEDs[i]]->num);
			_delay_ms(delay_time);
			PINS[UILEDs[i]]->p_port->OUTCLR = 1<<(PINS[UILEDs[i]]->num);
			_delay_ms(delay_time);
		}
	}
}


