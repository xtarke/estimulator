/*
 * estimulador.c
 *
 *  Created on: Oct 4, 2017
 *      Author: xtarke
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>
#include <string.h>

#include "bits.h"
#include "mcp4921.h"

#define N 8 					// timer 1 prescaler
#define BURST_FUND_FREQ 50		// 50Hz as burst fundamental frequency
#define TIMER1_MAX ((F_CPU / (N*BURST_FUND_FREQ)) - 1)	//timer TOP

#define RELAY_TAP_PIN   PD4
#define RELAY_RCA_PIN 	PD3
#define RELAY_PORT		PORTD

#define PULSE_PIN_1   	PD6
#define PULSE_PIN_2  	PD5
#define PULSES_PORT		PORTD

#define LED_ON_PORT		PORTD
#define LED_ON			PD7
#define LED_RAMP_PORT	PORTB
#define LED_RAMP_ON		PB0

static inline void startBurst();
static inline void stopBurst();

static void rampUP();
static void rampDown();

struct states {
	union {
		uint8_t all;
		struct	{
			uint8_t on:1;
			uint8_t rampOn:1;
			uint8_t _trash:6;
		};
	} state;
	uint16_t rampValue;
	uint16_t amplitude;
	int16_t rampInc;
};

volatile struct states app;
volatile uint8_t pulses = 0;
volatile uint8_t rampIte = 0;
volatile uint8_t up = 1;


ISR(TIMER1_COMPA_vect){

	pulses++;

	if (pulses < 6){
		OCR1B = OCR1A + 700;
		OCR1A += 2000;
	}

	set_bit(PULSES_PORT, PULSE_PIN_1);
	set_bit(PULSES_PORT, PULSE_PIN_2);

	up = 1;

	// cpl_bit(PORTD, PD4);
}

ISR(TIMER1_COMPB_vect){

	static uint8_t phase_ctrl = 0;

	if (up == 1){
		OCR1B += 600;
		up = 0;
	}

	phase_ctrl++;

	if (phase_ctrl <= 2)
		cpl_bit(PULSES_PORT, PULSE_PIN_1);
	else if(phase_ctrl > 2)
		cpl_bit(PULSES_PORT, PULSE_PIN_2);

	if (phase_ctrl == 4)
		phase_ctrl = 0;

}

ISR(TIMER1_CAPT_vect){

	OCR1A = 2000;
	OCR1B = 700;

	pulses = 0;

	set_bit(PULSES_PORT, PULSE_PIN_2);
	set_bit(PULSES_PORT, PULSE_PIN_1);

	up = 1;
	TCNT1 = 0;
}


/* DAC_set_voltage is non-preemptive *
 *
 * This IRQ should be preemptable     */
ISR(TIMER0_COMPA_vect){

	DAC_set_voltage(app.rampValue);

	app.rampValue += app.rampInc;
	rampIte++;

	if (rampIte > 100){
		/* Disable timer clock */
		TCCR0B = 0;
		app.state.rampOn = 0;
		set_bit(LED_RAMP_PORT, LED_RAMP_ON);
	}
}


void estStart(){

	if (app.state.on == 0) {
		app.state.on = 1;

		/* Turn on Relay: Se descomentado, onda fica monofásica. */
		set_bit(RELAY_PORT, RELAY_TAP_PIN);
		clr_bit(RELAY_PORT, RELAY_RCA_PIN);

		clr_bit(LED_ON_PORT, LED_ON);

		/* Burst generation enable */
		startBurst();

		rampUP();
	}
}

void estStop(){

	if (app.state.on == 1){
		app.state.on = 0;

		rampDown();
		while (app.state.rampOn);

		/* Burst generation enable */
		stopBurst();

		/* Turn off Relay */
		clr_bit(RELAY_PORT, RELAY_TAP_PIN);
		clr_bit(RELAY_PORT, RELAY_RCA_PIN);

		set_bit(LED_ON_PORT, LED_ON);
	}

}


void estInit(){

	/* All states cleared */
	memset((void *)&app, 0, sizeof(app));

	/* PINs */
	DDRB  = 0xFF; 	// Set to ouput
	PORTB = 0b00000001;
	DDRD = 0xFF;	// Set to ouput
	PORTD = 0b10000000;

	/* DAC disable */
	DAC_set_voltage(0);

	/* Timer 0 in CTC mode: ramps up and down */
	TCCR0A = (1 << WGM01);
	TIMSK0 = (1 << OCIE0A);
	/* CTC mode with 1024 prescaler and TOP = 156 gives 10 ms IRQs */
	OCR0A = 156;

	/* Timer 1 used to 1Khz burst @ 50Hz */
	/* Compare and capture IRQs */
	TCCR1A = 0;
	/* CTC mode with ICR1 as TOP */
	TCCR1B =  (1 << WGM13) | (1 << WGM12);
	TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B) | (1 <<  ICIE1);
	ICR1 = TIMER1_MAX;

	/* Stop Burst */
	stopBurst();

	app.amplitude = 4000;

}

static inline void startBurst(){
	/* Timer 1 used to 1Khz burst @ 50Hz */
	set_bit(PORTD, PULSE_PIN_1);
	set_bit(PORTD, PULSE_PIN_2);

	TCNT1 = 0;

	/* Compare start time */
	OCR1A = 2000;
	OCR1B = 700;

	set_bit(TCCR1B, CS11);

}

static inline void stopBurst(){
	/* Disable timer 1*/
	clr_bit(TCCR1B, CS11);
}


static void rampUP(){
	app.state.rampOn = 1;
	app.rampValue = 0;
	app.rampInc = (app.amplitude  <=  100) ? 1 : app.amplitude / 100;
	rampIte = 0;

	TCCR0B = (1<< CS02) | (1 << CS00);
	TCNT0 = 0;

	clr_bit(LED_RAMP_PORT, LED_RAMP_ON);
}

static void rampDown(){
	app.state.rampOn = 1;
	app.rampValue = app.amplitude;
	app.rampInc = (app.amplitude  <=  100) ? 1 : app.amplitude / 100;
	app.rampInc = -app.rampInc;
	rampIte = 0;

	TCCR0B = (1<< CS02) | (1 << CS00);
	TCNT0 = 0;

	clr_bit(LED_RAMP_PORT, LED_RAMP_ON);
}


void estSetAmplitute(uint16_t data){

	/* Wait while wamping is on */
	while (app.state.rampOn == 1);

	data = data & 0x0fff;
	app.amplitude = data;

	if (app.state.on)
		DAC_set_voltage(data);
}

