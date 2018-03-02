/*
 * Timer1.c
 *
 * Created: 3/1/2018 2:30:27 PM
 * Author : meloanmm
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TimerTest.h"

ISR(TIMER1_COMPA_vect) {
	//PORTB ^= 0x80; // turn off light after 75ms
}

ISR(TIMER1_OVF_vect) {
	PORTB ^= 0x80; // turn on light again
	TCNT1=0xAE9E; // reload the timer
}

int main(void)
{
    setup();
	DDRB = 0x80;
	PORTB = 0x00;
	
	/* Replace with your application code */
    while (1) {
		
		
    }
}

