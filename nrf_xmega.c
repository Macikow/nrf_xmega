/*
 * nrf_xmega.c
 *
 * Created: 2015-07-28 18:50:28
 *  Author: Macikow
 */ 


#include <avr/io.h>
#include "spi/spi.h"

void peripherial_Init(void){
	PORTE.DIRSET = (1 << 6);
	PORTE.OUTSET = (1 << 6);	
}

int main(void)
{
	//peripherial_Init();
	spi_Init();
	/*CSN_Low();
	spi_Transmit(0x00);
	CSN_High();
	CSN_Low();
	spi_Transmit(0x32);
	CSN_High();
	CSN_Low();
	spi_Transmit(0x17);
	spi_Transmit(0x64);
	CSN_High();*/
    while(1)
    {
        //TODO:: Please write your application code 
    }
}