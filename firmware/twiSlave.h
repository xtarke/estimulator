/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twiSlave.h
 * Module:			TWI Slave controller
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 *
 * Modifed: 		Renan Augusto Starke
 *
 * Build:			1
 * Last edition:	September 25, 2017
 * Purpose:			Interfaces a TWI Slave data bus
 * -------------------------------------------------------------------------- */

#ifndef __TWI_SLAVE_H
#define __TWI_SLAVE_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile uint8_t twiBuffer_t;

typedef enum twiSlaveState_t {
	TWI_STX_ADR_ACK				= 0xA8,
	TWI_STX_DATA_ACK			= 0xB8,
	TWI_STX_DATA_NACK			= 0xC0,
	TWI_SRX_GEN_ACK				= 0x70,
	TWI_SRX_ADR_ACK				= 0x60,
	TWI_SRX_ADR_DATA_ACK		= 0x80,
	TWI_SRX_GEN_DATA_ACK		= 0x90,
	TWI_SRX_STOP_RESTART		= 0xA0,
	TWI_SRX_ADR_DATA_NACK		= 0x88,
	TWI_SRX_GEN_DATA_NACK		= 0x98,
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,
	TWI_BUS_ERROR				= 0x00
} twiSlaveState_t;

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

void twiSlaveInit(uint8_t twiSlaveAddr, uint8_t *buffer, uint8_t bufferSize, uint8_t genCallAcceptance);
uint8_t TWIisPackReady();
void TWIPackRead();

#endif
