/*
 * spi.h
 *
 *  Created on: Oct 25, 2016
 *      Author: xtarke
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <avr/io.h>

#define SS PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5
#define SPI_PORT PORTB

void SPI_MasterInit(void);
uint8_t SPI_Transmit(uint8_t byte);
uint8_t SPI_SlaveReceive(uint8_t byte);

#endif /* SPI_H_ */
