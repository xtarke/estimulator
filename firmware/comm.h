/*
 * comm.h
 *
 *  Created on: Sep 22, 2017
 *      Author: Renan Augusto Starke
 */

#ifndef COMM_H_
#define COMM_H_

#include <stdint.h>

#define TWI_slaveAddress 0x09

#define PKG_INIT 0x7E
#define PKG_PAYLOAD_SIZE 3

enum CMD_TYPE {START_CMD, STOP_CMD, PULSE_FREQ, PULSE_HTIME, PULSE_LTIME,
				BURST_FREQ, BURST_HTIME, BURST_LTIME, RISE_TIME, STABLE_TIME,
				FALL_TIME, IDLE_TIME, AMPL, PULSE_PATT};

typedef struct packages {
	uint8_t cmd;
	uint16_t data;
} packate_t;


void initCommModules();
uint8_t packageRdy(packate_t *my_pkg);

#endif /* COMM_H_ */
