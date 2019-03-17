/*
 * ModuleControl.c
 *
 * Created: 3/16/2019 10:25:59 PM
 * Author : NPham
 */ 

#include <avr/io.h>

#include "constants.h"
#include "initialize.h"
#include "utilities.h"
#include "fsms.h"

int main(void)
{
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

