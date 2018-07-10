/*
 * comm.c
 *
 *  Created on: Sep 22, 2017
 *      Author: Renan Augusto Starke
 */
//#include "twiSlave.h"

#include "TWI_slave.h"
#include "comm.h"

#include <string.h>
#include <stdio.h>
#include "usart.h"

FILE usart_str = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);
uint8_t messageBuf[8];

void initCommModules(){
	// Initialise TWI module for slave operation. Include address and/or enable General Call.
	twiSlaveInit(TWI_slaveAddress, messageBuf,8 , 1);
	//TWI_Slave_Initialise(TWI_slaveAddress);

	//stdout mapped to UART module
	stdout = &usart_str;

	//UART module init
	USART_init(_9600, NULL, 0);
}


uint8_t packageRdy(packate_t *my_pkg){

	uint8_t ret = 0;

	// Check if the TWI Transceiver has completed an operation.
//	if (TWIisPackReady()){
//		my_pkg->cmd = messageBuf[0];
//		my_pkg->data = (messageBuf[1] << 8) | messageBuf[2];
//		TWIPackRead();
//		ret = 1;
//	}

	return ret;
}
