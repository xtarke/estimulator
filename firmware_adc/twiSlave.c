/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			twiSlave.c
 * Module:			TWI Slave controller
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Modifed:			Renan Augusto Starke
 *
 * Build:			alpha
 * Last edition:	September 25, 2017
 * Purpose:			Interfaces a TWI Slave data bus
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "comm.h"
#include "twiSlave.h"
#include "ad_data.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bits.h"

#define TRUE 1
#define FALSE 0


extern volatile adData_t adData;


// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

static uint8_t * twiBufferData;
static volatile uint8_t twiBufferIndex = 0;
static volatile uint8_t twiBufferSize = 0;
static volatile uint8_t twiCommIndex = 0;
static volatile uint8_t twiBusy = 0;
static volatile uint8_t packRdy = 0;

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

void twiSlaveInit(uint8_t twiSlaveAddr, uint8_t *buffer, uint8_t bufferSize, uint8_t genCallAcceptance)
{
	//TWAR = (twiSlaveAddr << 1) | (genCallAcceptance & 1);
	TWAR = (twiSlaveAddr) | (genCallAcceptance & 1);
	TWCR =	(1 << TWEN) |
			(1 << TWIE) | (1 << TWINT) |
			(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
			(0 << TWWC);
	twiBusy = 0;

	twiBufferData = buffer;
	twiBufferSize = bufferSize;

	DDRB = 1 << PB0;

}

uint8_t TWIisPackReady(){
	return packRdy & twiBusy;
}

void TWIPackRead(){
	packRdy = 0;
}

ISR(TWI_vect)
{
	switch (TWSR) {
	case TWI_STX_ADR_ACK:

		//cpl_bit(PORTB,PB0);

		TWDR = (uint8_t)adData.filtered; 	/* Reply address */

		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 1;
		break;
	case TWI_STX_DATA_ACK:
		TWDR =  129;//adData.filtered; 	/* Reply ADC Data */

		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 1;
		break;

	case TWI_STX_DATA_NACK:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;
		break;

	case TWI_SRX_GEN_ACK:
	case TWI_SRX_ADR_ACK:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiCommIndex = TRUE;
		twiBusy = 1;
		break;

	case TWI_SRX_ADR_DATA_ACK:
	case TWI_SRX_GEN_DATA_ACK:

		/* Simple communication:
		 * If any data is received, ADC is reply 		 */
		twiBufferData[0] = TWDR;
		packRdy = 1;

		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 1;
		break;

	case TWI_SRX_STOP_RESTART:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;
		break;

	case TWI_SRX_ADR_DATA_NACK:
	case TWI_SRX_GEN_DATA_NACK:
	case TWI_STX_DATA_ACK_LAST_BYTE:
	case TWI_BUS_ERROR:
		TWCR =	(0 << TWEN) |
				(0 << TWIE) | (1 << TWINT) |
				(0 << TWEA) | (0 << TWSTA) | (1 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;

		break;

	default:
		TWCR =	(1 << TWEN) |
				(1 << TWIE) | (1 << TWINT) |
				(1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
				(0 << TWWC);
		twiBusy = 0;
		break;
	}
}
