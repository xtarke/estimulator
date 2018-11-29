/*
 * main.c
 *
 *  Created on: Sep 19, 2017
 *      Author: xtarke
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include <stdio.h>
#include "avr_adc.h"
#include "bits.h"

#include "comm.h"
#include "ad_data.h"

#include "TWI_slave.h"

void init_ad();

volatile adData_t adData;

#define TWI_CMD_MASTER_WRITE 0x10
#define TWI_CMD_MASTER_READ  0x20

int main(){
	memset((void *)&adData, 0, sizeof(adData));

	init_ad();
	initCommModules();

	DDRD = 0xff;

	sei();


	// This loop runs forever.
	for(;;)
	{
		printf("oi -");
		_delay_ms(1000);

	}


	return 0;
}

void init_ad(){

	/* Ref AVCC, Canal 0
	 * Bits mais significativos desprezados (ADLAR) */
	ADCS->AD_MUX = SET(ADLAR) | SET(REFS0);
	/* Habilita AD:
	 * Conversão contínua
	 * IRQ
	 * Prescaler = 2  */
	ADCS->ADC_SRA = SET(ADEN) | SET(ADSC) | SET(ADATE) |
					SET(ADPS0) | SET(ADPS1) | SET(ADPS2) | SET(ADIE);

	/* Desabilita parte digital de PC0 */
	ADCS->DIDr0.BITS.ADC0 = 1;

}

ISR(ADC_vect)
{
	cpl_bit(PORTD,PD2);

	uint8_t sreg;
	uint8_t data = ADCH;

	sreg = SREG;
	clr_bit(ADCSRA, ADIE);
	sei();

	adData.raw = data;
	adData.filtered = adData.sum >> ADWINDOW_SHIFT;
	adData.sum = adData.sum - adData.data[adData.i] + data;
	adData.data[adData.i] = data;
	adData.i++;

	adData.i &= (ADWINDOW -1);

	set_bit(ADCSRA, ADIE);
	SREG = sreg;
}

