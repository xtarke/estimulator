/*
 * spi.c
 *
 *  Created on: Oct 25, 2016
 *      Author: xtarke
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bits.h"
#include "spi.h"

// Inicialização do SPÌ
void SPI_MasterInit(void)
{
	/* Configura direção das portas */
	DDRB = (1 << SS) | (1 << MOSI) | (1 << SCK);

	/* Ativa SPI em modo mestre
	 * clock = F_CPU / 16  */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t SPI_Transmit(uint8_t byte){

	//Coloca dado no registrador de hardware
	SPDR = byte;

	//Aguarada transmissão
	while(!(SPSR & (1<<SPIF)));

	// retorna se algum dado for lido
	return SPDR;
}



uint8_t SPI_SlaveReceive(uint8_t byte)
{
	// Uma recepção de SPI deve ativar o clock
	// logo, enviamos outro dado ou qualquer coisa
	return SPI_Transmit(byte);
}

