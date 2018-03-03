/*
 * TimerTest.c
 *
 * Created: 3/2/2018 9:14:31 PM
 *  Author:  meloanmm, tarravechiarj
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* ONTIME is 75ms; PERIOD is ~333.3ms; */
#define ONTIME  1171
#define PERIOD  5208

/* pre-scaling for clk/1024 */
#define PRE_SCALING ((1 << CS12) | (1 << CS10))
#define OUTPUT_PIN  (1 << PINB7)

/* 0 = normal; 1 = CTC; 2 = phase and frequency correct PWM mode */
#define MODE 2



/* settings for leapfrogging in normal mode */
#if MODE == 0

#define START (65535 - PERIOD)

void setup() {
	cli();         // disable global interrupts	
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	TCNT1 =START;
	TCCR1B |= PRE_SCALING;

	TIMSK1 |= (1 << TOIE1);		// enable Timer1 overflow interrupt
	TIMSK1 |= (1 << OCIE1A);	// enable Timer1 compare A interrupt
	
	OCR1A = START + ONTIME;		//set up match for 75ms after start value
	DDRB |= OUTPUT_PIN;
	PORTB |= OUTPUT_PIN;		// LED starts on
	
	sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	PORTB ^= OUTPUT_PIN; // turn off light after 75ms
}

ISR(TIMER1_OVF_vect) {
	PORTB ^= OUTPUT_PIN;  // turn on light again
	TCNT1 = START;		  // reload the timer
}



/* settings for CTC mode */
#elif MODE == 1

void setup() {
	cli();         // disable global interrupts	
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	TCCR1B |= (1 << WGM13) | (1 << WGM12);	// CTC mode
	TCCR1B |= PRE_SCALING;
	
	TIMSK1 |= (1 << OCIE1A);     // enable Timer1 compare A interrupt
	TIMSK1 |= (1 << ICIE1);     // enable Timer1 input capture interrupt
	
	ICR1 = PERIOD;                  // set TOP value to PERIOD
	OCR1A = ONTIME;                 // set up match for 75ms 
	DDRB |= OUTPUT_PIN;
	PORTB |= OUTPUT_PIN;		// LED starts on
	
	sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	PORTB ^= OUTPUT_PIN; // toggle LED state
}

ISR_ALIAS(TIMER1_CAPT_vect, TIMER1_COMPA_vect);



/* settings for PWM mode 8 */
#elif MODE == 2

void setup() {
	cli();         // disable global interrupts
	TCCR1A = 0;    // set entire TCCR1A register to 0
	TCCR1B = 0;    // set entire TCCR1A register to 0

	/* PWM mode 8: LED on when counting up; off when counting down */
	TCCR1B |= (1 << WGM13);
	TCCR1A |= (1 << COM1C1) | (1 << COM1C0);
	TCCR1B |= PRE_SCALING;
	
	ICR1 = PERIOD/2;                        // set TOP value to PERIOD/2
	OCR1C = (PERIOD/2) - (ONTIME/2) + 1;	// set match for TOP - ONTIME/2
	
	DDRB |= OUTPUT_PIN;
	
	sei(); // enable global interrupts
}

#endif

