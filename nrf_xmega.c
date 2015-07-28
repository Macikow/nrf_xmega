/*
 * nrf_xmega.c
 *
 * Created: 2015-07-28 18:50:28
 *  Author: Macikow
 */ 

#define F_CPU 2000000UL
#include <avr/io.h>
#include "spi/spi.h"
#include "nrf2410/nrf24.h"
#include <stdio.h>
#include "uart/uart.h"
#include <avr/interrupt.h>
#include "avr/delay.h"

/************************************************************************/
/* UART CONSOLE APP                                                                     */
/************************************************************************/
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

static int uart_putchar (char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	
	// Wait for the transmit buffer to be empty
	while ( !( USARTD0.STATUS & USART_DREIF_bm) );
	
	// Put our character into the transmit buffer
	USARTD0.DATA = c;
	
	return 0;
}
///////////////////////////////////////////////////////////////////

void peripherial_Init(void){
	PORTE.DIRSET = (1 << 6);
	PORTE.OUTSET = (1 << 6);	
}

int main(void)
{
	peripherial_Init();
	uart_Init();
	stdout = &mystdout;
	sei();
	printf(ERASE_SCREEN);
	nrf24_init();
	
	nrf24_config(2, 3);
	CSN_Low();
	spi_Transmit(0x00);
	CSN_High();
	CSN_Low();
	spi_Transmit(0x32);
	CSN_High();
	CSN_Low();
	spi_Transmit(0x17);
	spi_Transmit(0x64);
	CSN_High();
    while(1)
    {
		//printf("sdfghjkl");
        //TODO:: Please write your application code 
    }
}

ISR(USARTD0_RXC_vect){
	
}