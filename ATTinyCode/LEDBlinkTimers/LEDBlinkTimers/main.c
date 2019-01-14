/*
 * LEDBlinkTimers.c
 *
 * Created: 1/12/2019 3:27:07 PM
 * Author : NPham
 */ 

#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <util/delay.h>

const char LEDPIN = PINC0;

void initializeBlinker(const char pin) {
	DDRC |= (1<<pin);		// set pin PC0 to output
	
	TCCR1A |= (1<<TCW1);	// set timer 1 width to 16 bit mode
	TCCR1A |= (1<<CS11);	// set timer 1 clock divider = 8
	
	OCR1B = 0x80;			// set ouptut comparison high byte first
	OCR1A = 0x00;			// set output comparison low byte 
							//   -> this triggers both bytes to be
							//      written to the 16-bit register
}

void updateBlink(const char pin) {
	if (TIFR & (1<<OCF1A)) {		// check if the comparison flag has been set
		TIFR	|= (1<<OCF1A);			// reset the flag
		TCNT1H	= 0x00;					// reset the counter (high bit first)
		TCNT1L	= 0x00;					// reset the counter (low bit second)
		PORTC	^= (1<<pin);			// toggle the LED
	}
}

int main(void) {
	initializeBlinker(LEDPIN);
	while(1) {
		updateBlink(LEDPIN);
	}
}

