/*
 * spi.h
 *
 * Created: 2015-07-28 21:08:33
 *  Author: Macikow
 */ 


#ifndef SPI_H_
#define SPI_H_

#define MOSI	7
#define MISO	6
#define SCK		5
#define SS		4
#define CSN		3

void spi_Init();
unsigned char spi_Transmit(unsigned char spi_Data);
void CSN_Low(void);
void CSN_High(void);



#endif /* SPI_H_ */