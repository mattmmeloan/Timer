/*
 * TimerTest.c
 *
 * Created: 3/1/2018 2:31:17 PM
 *  Author: meloanmm
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* ONTIME is 75ms; PERIOD is ~333.3ms; */
#define ONTIME  1171
#define PERIOD  5208

/* ontime = 3s, period = 4s (for testing) */
//#define ONTIME  46875
//#define PERIOD  62500

/* pre-scaling for clk/1024 */
#define PRE_SCALING ((1 << CS12) | (1 << CS10))
#define OUTPUT_PIN  (1 << PINB7)

/* START is Timer register 1's TOP value - PERIOD */
#define NORMAL_START

/* 0 = normal; 1 = CTC; 2 = phase and frequency correct PWM mode */
#define MODE 1

/* settings for leapfrogging in normal mode */
#if MODE == 0

void setup() {
	cli();         // disable global interrupts
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	TCNT1 = NORMAL_START;
	TCCR1B |= PRE_SCALING;

	TIMSK1 |= (1 << TOIE1);		// enable Timer1 overflow interrupt
	TIMSK1 |= (1 << OCIE1A);	// enable Timer1 compare A interrupt
	
	OCR1A = NORMAL_START + ONTIME;	//set up match for 75ms after start value
	DDRB |= OUTPUT_PIN;
	PORTB |= OUTPUT_PIN;		// LED starts on
	
	sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	PORTB ^= OUTPUT_PIN; // turn off light after 75ms
}

ISR(TIMER1_OVF_vect) {
	PORTB ^= OUTPUT_PIN;  // turn on light again
	TCNT1 = NORMAL_START; // reload the timer
}
#endif

/* settings for CTC mode */
#if MODE == 1

void setup() {
	cli();         // disable global interrupts
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	TCCR1B |= (1 << WGM13) | (1 << WGM12);	// CTC mode
	TCCR1B |= PRE_SCALING;
	
	TIMSK1 |= (1 << OCIE1A);	// enable Timer1 compare A interrupt
	TIMSK1 |= (1 << ICIE1);	    // enable Timer1 input capture interrupt
	
	ICR1 = PERIOD;				// set TOP value to PERIOD
	OCR1A = ONTIME;				// set up match for 75ms
	DDRB |= OUTPUT_PIN;
	PORTB |= OUTPUT_PIN;		// LED starts on
	
	sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	PORTB ^= OUTPUT_PIN; // toggle LED state
}

ISR_ALIAS(TIMER1_CAPT_vect, TIMER1_COMPA_vect);
#endif
