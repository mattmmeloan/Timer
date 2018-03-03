/*
 * TimerTest.c
 *
 * Created: 3/1/2018 2:31:17 PM
 *  Author: meloanmm
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* ONTIME is 75ms; PERIOD is ~333.3ms; 
   START is Timer register 1's TOP value - PERIOD */
#define ONTIME  1171
#define PERIOD  5208
#define START   (65535 - PERIOD)

#define OUTPUT_PIN  (1 << PINB7)
#define PRE_SCALING ((1 << CS12) | (1 << CS10)) // clk/1024

/* initialize timer 1 for normal mode 
 */
void setup() {
	cli();         // disable global interrupts	
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	TIMSK1 |= (1 << TOIE1);		// enable Timer1 overflow interrupt
	TIMSK1 |= (1 << OCIE1A);	// enable Timer1 compare A interrupt
	
	TCNT1 = START;
	TCCR1B |= PRE_SCALING;
	
	OCR1A = START + ONTIME;		//set up match for 75ms after start value
	DDRB |= OUTPUT_PIN;
	PORTB |= OUTPUT_PIN;		// LED starts on
	
	sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	PORTB ^= OUTPUT_PIN; // turn off light after 75ms
}

ISR(TIMER1_OVF_vect) {
	PORTB ^= OUTPUT_PIN; // turn on light again
	TCNT1 = START;		 // reload the timer
}
