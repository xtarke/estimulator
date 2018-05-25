/*
 * main.c
 *
 *  Created on: Sep 19, 2017
 *      Author: xtarke
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include "avr_adc.h"
#include "bits.h"

#include "comm.h"

void init_ad();

volatile uint16_t adData = 0;

int main(){

	  packate_t rcv_data;

	  init_ad();
	  initCommModules();

	  DDRB = 0xff;


	  sei();


	  puts("Hello world!");

	  //estSetAmplitute(2048);

	  //estStart();

	  // This loop runs forever.
	  for(;;)
	  {
		  if (packageRdy(&rcv_data)){
			  printf("%x %x\n\r", rcv_data.cmd, rcv_data.data);




			  switch (rcv_data.cmd){
			  case START_CMD:
				  //estStart();
				  break;

			  case STOP_CMD:
				  //estStop();
				  break;

			  case AMPL:
				  //estSetAmplitute(rcv_data.data);
				  break;

			  default:
				  break;
			  }
		  }
	  }


	return 0;
}

void init_ad(){

	/* Ref interna de 1V1, Canal 0 */
	ADCS->AD_MUX = SET(REFS0) | SET(REFS1);
	/* Habilita AD:
	 * Conversão contínua
	 * IRQ
	 * Prescaler = 128  */
	ADCS->ADC_SRA = SET(ADEN) | SET(ADSC) | SET(ADATE) |
				SET(ADPS0) | SET(ADPS1) | SET(ADPS2) | SET(ADIE);

	/* Desabilita parte digital de PC0 */
	ADCS->DIDr0.BITS.ADC0 = 1;

}

ISR(ADC_vect)
{
	adData = ADC;
}

