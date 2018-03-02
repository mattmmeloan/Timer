/*
 * TimerTest.c
 *
 * Created: 3/1/2018 2:31:17 PM
 *  Author: meloanmm
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>



void setup() {
	// initialize Timer1
	cli();         // disable global interrupts
	
	
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	// enable Timer1 overflow interrupt:
	TIMSK1 |= (1 << TOIE1);
	// Pre-load with value 44702
	//use 64886 for 100Hz
	//use 64286 for 50 Hz
	//use 44702 for 3 Hz?
	//use 34286 for 2 Hz
	TCNT1=0xAE9E;
	TCCR1B |= (1 << CS12); // Sets bit CS12 in TCCR1B, prescaling at 256
	// This is achieved by shifting binary 1 (0b00000001)
	
	//set up match
	//4788 cycles after timer start
	//roughly 75ms
	OCR1A = 0xC0EE;

	// enable global interrupts
	sei();
}