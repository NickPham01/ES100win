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

int main (void)
{
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
	}}

