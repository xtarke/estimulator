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

#include "estimulador.h"
#include "comm.h"


int main(){

	  packate_t rcv_data;

	  initCommModules();
	  estInit();

	  sei();


	  estSetAmplitute(2048);

	  estStart();

	  // This loop runs forever.
	  for(;;)
	  {
		  if (packageRdy(&rcv_data)){
			  // printf("%x %x\r", rcv_data.cmd, rcv_data.data);

			  switch (rcv_data.cmd){
			  case START_CMD:
				  estStart();
				  break;

			  case STOP_CMD:
				  estStop();
				  break;

			  case AMPL:
				  estSetAmplitute(rcv_data.data);
				  break;

			  default:
				  break;
			  }
		  }
	  }


	return 0;
}


