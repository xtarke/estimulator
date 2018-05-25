/*
 * main.c
 *
 *  Created on: Oct 25, 2016
 *      Author: xtarke
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "bits.h"
#include "i2c.h"

#include "usart.h"

#define SS PB2

#define TWI_slaveAddress 0x08

FILE usart_str = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(){

	uint8_t byte;
	DDRD = 0xff;

	DDRC = (1 << PC4) | (1<<PC5);

	//UART module init

	stdout = &usart_str;

	USART_init(_9600, NULL, 0);


	i2c_init();
	sei();


	//0xd0 eh o endereco do RTC

	//i2c_write(0x07, 0b00010000, RTC_ADDR);

	_delay_ms(100);

	while (1){

		byte = i2c_read(0x05, TWI_slaveAddress);


		printf("recebido: %x\n\r", byte);

		//i2c_write(0x00, 0x00, TWI_slaveAddress << 1);

		cpl_bit(PORTD, 4);

		_delay_ms(1000);
	}
	return 0;
}
