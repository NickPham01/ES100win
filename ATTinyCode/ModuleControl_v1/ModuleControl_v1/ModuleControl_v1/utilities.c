#include "utilities.h"
#include "constants.h"



/*
	Function: read the VCC SLT DIP switch from a receiver
		Input:	receiver (= 1 or 2)
		Output:	none
		Return: uint8_t voltage setting
*/
uint8_t readVccSwitch(uint8_t receiver) {
	/* Get the pins corresponding to the given receiver */
	uint8_t vcc_slt_0;
	uint8_t vcc_slt_1;
	switch (receiver) {
		case 1:
			vcc_slt_0 = VCC_SLT0_1_PIN;
			vcc_slt_1 = VCC_SLT1_1_PIN;
			break;
		case 2:
			vcc_slt_0 = VCC_SLT0_2_PIN;
			vcc_slt_1 = VCC_SLT1_2_PIN;
			break;
	}
	
	/* Read the sensors */
	uint8_t data = 0x00;
	data |= gpioREAD(vcc_slt_0) << 0;	// set 0th bit
	data |= gpioREAD(vcc_slt_1) << 1;	// set 1st bit
	
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

/*
	Function: Set VCC
		Input:	receiver (=1 or 2)
				voltage (9, 12, or 18)
		Output:	sets GPIO to control adjustable regulator VCC output
		Return: none
*/
void setVcc(uint8_t receiver, uint8_t voltage) {
	/* Get the PORT and pinMacros associated with the receiver */
	uint8_t vcc_9;
	uint8_t vcc_12;
	switch(receiver){
		case 1:
			vcc_9	= VCC_9V_1_PIN;
			vcc_12	= VCC_12V_1_PIN;
			break;
		case 2:
			vcc_9	= VCC_9V_2_PIN;
			vcc_12	= VCC_12V_2_PIN;
			break;
	}
	
	/* Set pins PA0 and PA1 (Vcc select) */
	switch(voltage) {
		case 12:
			// (9, 12) = (0, 1)
			gpioOFF(vcc_9);
			gpioON(vcc_12);
			break;
		case 18:
			// (12, 9) = (0, 0)
			gpioOFF(vcc_9);
			gpioOFF(vcc_12);
			break;
		default:
			// (9, 12) = (1, 0) Output 9V by default
			gpioON(vcc_9);
			gpioOFF(vcc_12);
			break;
	}
}

/*
	Function: activate relay: actually control the outputs to the relay driver
		Input:	relay Pins (+ and -) to drive
				desired relay state (= BYPASS, ACTIVE, IDLE)
		Output:	sets relay driver pins
		Return: none
*/

void activateRelay(uint8_t rly_p, uint8_t rly_n, uint8_t relayState) {
	switch(relayState) {
		case BYPASS:
			// (p, n) = (1, 0)
			gpioON(rly_p);
			gpioOFF(rly_n);
			break;
		case ACTIVE:
			// (p, n) = (0, 1)
			gpioOFF(rly_p);
			gpioON(rly_n);
			break;
		default:
			// includes IDLE
			// (p, n) = (1, 1)
			gpioON(rly_p);
			gpioON(rly_n);
	}
}

/*
	Function: main function to switch the relays.  call this one to set state to bypass or active
		Input:	relay to switch (1,2,3,4,5,6) where 5 is for receiver 1 and 6 is for receiver 2
				desired relay state (enum BYPASS or ACTIVE)
		Output:	sets single-coil latching relay by asserting the set/reset signal for 15 ms then asserting idle
		Return:	none
*/
void setRelay(uint8_t relayNum, uint8_t relayState) {
	/* Get the pins corresponding to the desired relay */
	uint8_t rly_p;
	uint8_t rly_n;
	switch (relayNum) {
		case 1:
			rly_p = K1P_PIN;
			rly_n = K1N_PIN;
			break;
		case 2:
			rly_p = K2P_PIN;
			rly_n = K2N_PIN;
			break;
		case 3:
			rly_p = K3P_PIN;
			rly_n = K3N_PIN;
			break;
		case 4:
			rly_p = K4P_PIN;
			rly_n = K4N_PIN;
			break;
		case 5:
			rly_p = RLYP_1_PIN;
			rly_n = RLYN_1_PIN;
			break;
		case 6:
			rly_p = RLYP_2_PIN;
			rly_n = RLYN_2_PIN;
			break;
	}
	
	/* Assert the switch signal, block for 15ms, then assert idle */
	activateRelay(rly_p, rly_n, relayState);
	
	unsigned long startTime = millis();
	while (millis() - startTime < 15) {}
		
	activateRelay(rly_p, rly_n, IDLE);
}

/* 
	Function: read the plate sensor
		Input:	receiver to check (1 or 2)
		Output: none
		Return:	1 if both pins are connected, else 0
*/
uint8_t readPlateDetect(uint8_t receiver) {
	/* Get the pins corresponding to the desired receiver */
	uint8_t detect0;
	uint8_t detect1;
	switch(receiver) {
		case 1:
			detect0 = DETECT0_1_PIN;
			detect1 = DETECT1_1_PIN;
			break;
		case 2:
			detect0 = DETECT0_2_PIN;
			detect1 = DETECT1_2_PIN;
			break;
	}
	
	/* determine if the pins are inserted.  These will be active low values as the GPIO are pulled low */
	uint8_t data =  0x00;
	data |= gpioREAD(detect0) << 0;	// set 0th bit
	data |= gpioREAD(detect1) << 1;	// set 1st bit
	
	/*	The empty receiver state has both pins pulled high, so data = 0x03.
		When a plate is inserted and pulls the pins low, data = 0x00 */
	
	/* return 1 if the data is 0 else return 0 */
	return data == 0x00 ? 1 : 0;
}

/* 
	Function: read the 4 UI switches
		Input:	none
		Output: none
		Return: data byte corresponding to assertions of switches:
					sw[7:4] = 0
					sw[3] = BottomSW
					sw[2] = CrossDownSW
					sw[1] = CrossUPSW
					sw[0] = TopSW
*/
uint8_t readUISW(void) {
	uint8_t data = 0x00;
	
	for (uint8_t i = 0; i < sizeof(UIswitches), i++) {
		uint8_t pin = UIswitches[i];
		data |= gpioREAD(pin) << i;	// set ith bit
	}
	
	return data;	
}

/* 
	Function: set a single LED on or off
		Input:	LED number (1:6)
				LED state (0 or 1)
		Output:	set LEDn to state
		Return: none
*/
void setLED(uint8_t led, uint8_t state) {
	uint8_t pin = UILEDs[led];
	if state {
		gpioON(PINS[pin]);
	}
	else {
		gpioOFF(PINS[pin])
	}
}