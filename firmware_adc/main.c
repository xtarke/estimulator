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

void init_ad();

volatile adData_t adData;

int main(){

	  memset((void *)&adData, 0, sizeof(adData));

	  init_ad();
	  initCommModules();

	  sei();

	  // This loop runs forever.
	  for(;;)
	  {

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
				//SET(ADPS0) | SET(ADPS1) | SET(ADIE);
				SET(ADPS0) | SET(ADPS1) | SET(ADPS2) | SET(ADIE);

	/* Desabilita parte digital de PC0 */
	ADCS->DIDr0.BITS.ADC0 = 1;

}

ISR(ADC_vect)
{
	uint8_t data = ADCH;

	adData.filtered = adData.sum >> ADWINDOW_SHIFT;
	adData.sum = adData.sum - adData.data[adData.i] + data;
	adData.data[adData.i] = data;
	adData.i++;

	adData.i &= (ADWINDOW -1);
}

