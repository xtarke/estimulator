/*
 * mcp4921.c
 *
 *  Created on: Oct 2, 2017
 *      Author: xtarke
 */

#include "spi.h"
#include "bits.h"


/* 16-bit spi command:
 *
 * Upper Half:
 * | A/B | BUF | ~GA | ~ SHDN | D11 | D10 | D9 | D8 |
 * Lower Half:
 * | D7  | D6  | D5  | D4 | D3 | D2 | D1 | D0 |
 *
 * A/B: 1:Write to DAC_B / 0: Write do DAC_A
 * BUF: 1 Buffered / Unbuffered VREF
 * ~GA: 1: 1x (Vout = VREF * D /4096) 0: 2x(Vout = 2*VREF * D / 4096)
 * ~SHDN: 1: Ouput power down control bit / 0: Oubut buffer disable, high impedance
 * D11 to D0: DAC data bits
 */
void DAC_set_voltage(uint16_t voltage){

	 uint8_t dac_MSB = 0b01010000;
	 uint8_t dac_LSB = voltage;

	 dac_MSB = dac_MSB | (voltage >> 8);

	 clr_bit(SPI_PORT, SS);
	 SPI_Transmit(dac_MSB);
	 SPI_Transmit(dac_LSB);
	 set_bit(SPI_PORT, SS);
}
