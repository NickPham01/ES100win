/*
 * LEDBlink.c
 *
 * Created: 3/15/2019 10:26:18 AM
 * Author : NPham
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "constants.h"
#include "initialize.h"

int main(void)
{
	// Set LED GPIOs to be outputs
	for (uint8_t i = 0; i < sizeof(UILEDs); i++) {
		uint8_t pin = UILEDs[i];
		bitON(PINS[pin]->p_port->DIR, PINS[pin]->num);	// set the Direction register bits
	}
	
	
    /* Loop */
    while (1) 
    {
		for (uint8_t i = 0; i < sizeof(UILEDs); i++) {
			uint8_t pin = UILEDs[i];
			gpioON(pin);
		}
		_delay_ms(100);
		for (uint8_t i = 0; i < sizeof(UILEDs); i++) {
			uint8_t pin = UILEDs[i];
			gpioOFF(pin);
		}
		_delay_ms(100);
    }
}

