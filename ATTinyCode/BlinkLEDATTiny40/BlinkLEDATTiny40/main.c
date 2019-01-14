/*
 * BlinkLEDATTiny40.c
 *
 * Created: 1/12/2019 3:27:07 PM
 * Author : NPham
 */ 

#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	/* Set PC0 to be an output */
	DDRC |= 0b00000001;
	
	
    while (1) 
    {
		/* Set PC0 high (on) */
		PORTC = 0b00000001;
		_delay_ms(500);
		
		/* Set PC0 low (off) */
		PORTC = 0b00000000;
		_delay_ms(500);
    }
	
	return(1);
}

