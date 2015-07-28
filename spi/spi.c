/*
 * spi.c
 *
 * Created: 2015-07-28 21:08:22
 *  Author: Macikow
 */ 
#include <avr/io.h>
#include "spi.h"

#define MOSI	7
#define MISO	6
#define SCK		5
#define SS		4
#define CSN		3



void spi_Init(){
	
	PORTC.DIRSET = ( 1 << MOSI ) | ( 1 << SCK ) | ( 1 << SS ) | ( 1 << CSN) ;
	PORTC.DIRCLR = (1 << MISO) ;
	PORTC.OUTCLR = ( 1 << MOSI ) | ( 1 << SCK ) | ( 1 << SS ) | ( 1 << CSN) |  (1 << MISO) ;
	PORTC.REMAP = PORT_SPI_bm;
	
	SPIC.CTRL       =		SPI_ENABLE_bm|               // w³¹czenie SPI
	SPI_MASTER_bm|               // tryb master
	/*SPI_MODE_3_gc| */               // tryb 3
	SPI_PRESCALER_DIV64_gc;      // preskaler
	CSN_High();
}


unsigned char spi_Transmit(unsigned char spi_Data){
	SPIC.DATA       =    spi_Data;                        // wysy³anie danych
	while(SPIC.STATUS == 0);                          // czekanie na zakoñczenie transmisji
	return SPIC.DATA;
	
}

void CSN_Low(void){
	PORTC.OUTCLR = (1 << CSN);
}

void CSN_High(void){
	PORTC.OUTSET = (1 << CSN);
}