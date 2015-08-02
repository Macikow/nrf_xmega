/*
 * nrf24.c
 *
 * Created: 2015-07-28 21:08:55
 *  Author: Macikow
 */ 
#include <avr/io.h>
#include "nrf24.h"
#include "../spi/spi.h"
#include <stdio.h>
#include "../main.h"
#include "nrf_reg_struct.h"
#include "util/delay.h"
#include "../uart/vt100.h"


/************************************************************************/
/* architecture require                                                                     */
/************************************************************************/
#define IRQ_PORT PORTC  //PORTG
#define CE_PORT	PORTC
#define IRQ PIN0_bp
#define CE	PIN1_bp




unsigned char payload_len;




void readCONFIG(void){
	refreshRegsterData();
	printf(SET_FORE_WHITE);		
	for(unsigned char i=0;i<18;i++){
		
		if(i<10)				printf("tab[0x%x] = 0x%x\n",i,nrf_Registers_table[i]);
		else if(i>=10 && i<17)	printf("tab[0x%x] = 0x%x\n",i+7,nrf_Registers_table[i]);
		else					printf("tab[0x%x] = 0x%x\n",i+11,nrf_Registers_table[i]);
	}
	printf(SET_BACK_YELLOW);		
	printf("%sCONFIG - onfiguration register%s\n",SET_FORE_BLACK,SET_BACK_BLACK);
	if(NRF_R_CONFIG->PRIM_RX_R)		{printf(SET_FORE_BLUE);		printf("\tPRIM_RX is set\n");}	else {printf(SET_FORE_MAGNETA); printf("\tPRIM_RX clear \n");}													/*0*/
	if(NRF_R_CONFIG->PWR_UP_R)		{printf(SET_FORE_GREEN);	printf("\tPWR_UP is set\n");}														/*1*/
	if(NRF_R_CONFIG->CRCO_R == 0)	{printf(SET_FORE_WHITE);	printf("\tCRC 1 byte\n");}		else {printf(SET_FORE_WHITE);  printf("CRC 2 bytes");}						/*2*/			
	if(NRF_R_CONFIG->EN_CRC_R)		{printf(SET_FORE_GREEN);	printf("\tCRC enabled\n");}														/*3*/
	if(NRF_R_CONFIG->MASK_MAX_RT_R) {printf(SET_FORE_GREEN);	printf("\titerrupt when send max number of retransmit (enabled)\n");}		/*4*/
	if(NRF_R_CONFIG->MASK_TX_DS_R)	{printf(SET_FORE_GREEN);	printf("\tinterrupt when TX data send (enabled)\n");}							/*5*/
	if(NRF_R_CONFIG->MASK_RX_DS_R)	{printf(SET_FORE_GREEN);	printf("\tinterrupt when RX data received (enabled)\n");}
	printf(SET_FORE_WHITE);						/*6*/
}
void readEN_AA(void){
	
	refreshRegsterData();
	printf(SET_BACK_YELLOW);		
	printf("%sEN_AA - Enable Ack%s\n",SET_FORE_BLACK,SET_BACK_BLACK);	
	if(NRF_R_EN_AA->ENAA_P0_R)  {printf(SET_FORE_GREEN); printf("\tEnable Ack pipe 0\n");}													/*0*/
	if(NRF_R_EN_AA->ENAA_P1_R)	{printf(SET_FORE_GREEN); printf("\tEnable Ack pipe 1\n");}													/*1*/
	if(NRF_R_EN_AA->ENAA_P2_R)	{printf(SET_FORE_GREEN); printf("\tEnable Ack pipe 2\n");}						/*2*/
	if(NRF_R_EN_AA->ENAA_P3_R)	{printf(SET_FORE_GREEN); printf("\tEnable Ack pipe 3\n");}														/*3*/
	if(NRF_R_EN_AA->ENAA_P4_R)	{printf(SET_FORE_GREEN); printf("\tEnable Ack pipe 4\n");}		/*4*/
	if(NRF_R_EN_AA->ENAA_P5_R)	{printf(SET_FORE_GREEN); printf("\tEnable Ack pipe 5\n");}
	printf(SET_FORE_WHITE);								/*5*/
}
void readEN_RX_ADDR(void){
	refreshRegsterData();
	printf(SET_BACK_YELLOW);		
	printf("%sEN_RX_ADDR - Enable RX Address%s\n",SET_FORE_BLACK,SET_BACK_BLACK);	
	if(NRF_R_EN_RXADDR->ERX_P0_R)	{printf(SET_FORE_GREEN); printf("\tEnable data pipe 0\n");}													/*0*/
	if(NRF_R_EN_RXADDR->ERX_P1_R)	{printf(SET_FORE_GREEN); printf("\tEnable data pipe 1\n");}													/*1*/
	if(NRF_R_EN_RXADDR->ERX_P2_R)	{printf(SET_FORE_GREEN); printf("\tEnable data pipe 2\n");	}						/*2*/
	if(NRF_R_EN_RXADDR->ERX_P3_R)	{printf(SET_FORE_GREEN); printf("\tEnable data pipe 3\n");	}													/*3*/
	if(NRF_R_EN_RXADDR->ERX_P4_R)	{printf(SET_FORE_GREEN); printf("\Enable data pipe 4\n");}		/*4*/
	if(NRF_R_EN_RXADDR->ERX_P5_R)	{printf(SET_FORE_GREEN); printf("\tEnable data pipe 5\n");}	
	printf(SET_FORE_WHITE);		/*5*/
}
void readSETUP_AW(void){
	refreshRegsterData();
	printf(SET_BACK_YELLOW);		
	printf("%sSETUP_AW - Setup of Addres Widths%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	if(NRF_R_SETUP_AW->AW_R == 0x03) printf("\tRX/TX address field widths - 5B \n");
	else if(NRF_R_SETUP_AW->AW_R == 0x02) printf("\tRX/TX address field widths - 4B \n");
	else if(NRF_R_SETUP_AW->AW_R == 0x01) printf("\tRX/TX address field widths - 3B \n");
	else printf("\tRX/TX address field widths - illegal \n");							
	printf(SET_FORE_WHITE);	


}
void readSETUP_RETR(void){
	refreshRegsterData();
	printf(SET_FORE_WHITE);	printf(SET_BACK_YELLOW);		
	printf("%sSETUP_RETR - Setup of automatic retransmission%s \n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);			
	printf("\tRetransmission delay : %d * 250 us\n", NRF_R_SETUP_RETR->ADR_R );										/*0*/
	printf("\tRetransmission Count : %d \n", NRF_R_SETUP_RETR->ARC_R );
	printf(SET_FORE_WHITE);															/*5*/
}

void readRF_CH(void){
	refreshRegsterData();
	printf(SET_FORE_WHITE);printf(SET_BACK_YELLOW);		
	printf("%sRF_CH - RF Channel%s \n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	printf("\tRF Chanell :  %d \n", NRF_R_RF_CH->RF_CH_R);
	printf(SET_FORE_WHITE);															/*5*/
}
void readRF_SETUP(void){
	refreshRegsterData();
	printf(SET_BACK_YELLOW);	
	printf("%sRF_SETUP- RF Setup Register%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	if(NRF_R_RF_SETUP->RF_PWR_R == 0x00 ) printf("\tRF Poer = -18dBm\n");	
	else if(NRF_R_RF_SETUP->RF_PWR_R == 0x01 ) printf("\tRF Power = -12dBm\n");		
	else if(NRF_R_RF_SETUP->RF_PWR_R == 0x02 ) printf("\tRF Power = -6dBm\n");		
	else if(NRF_R_RF_SETUP->RF_PWR_R == 0x03 ) printf("\tRF Power = -0dBm\n");														/*0*/
	if(NRF_R_RF_SETUP->RF_DR_R == 0 ) printf("\tdata rates speed : 1Mbps\n");
	else if(NRF_R_RF_SETUP->RF_DR_R == 0x01 ) printf("\tdata rates speed : 2Mbps\n");	
	else if(NRF_R_RF_SETUP->RF_DR_R == 0x02 ) printf("\tdata rates speed : 250kbps\n");	
	else if(NRF_R_RF_SETUP->RF_DR_R == 0x03 ) printf("\tdata rates speed : reserved\n");
	if(NRF_R_RF_SETUP->RF_DR_L_R ) printf("\tdata rates speed : 250kbps\n");	
	if(NRF_R_RF_SETUP->CONT_WAVE) printf("\tContinous Carrier transmit \n");									/*1									/*5*/
}
void readSTATUS(void){
	refreshRegsterData();
	printf(SET_BACK_YELLOW);	
	printf("%sSTATUS- Status Regoster%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	if(NRF_R_STATUS->TX_FULL_R) printf("\tTX Fifo full flag\n");													/*0*/
	if(NRF_R_STATUS->RX_P_NO_R == 7) printf("\tRX Fifo empty\n");
	else if(NRF_R_STATUS->RX_P_NO_R == 6) printf("\tNot used\n");
	else printf("\tRX Fifo number  %d payload available for reading\n",NRF_R_STATUS->RX_P_NO_R);								/*1*/
	if(NRF_R_STATUS->MAX_RX_R) printf("\tMaximum number of TX retransmits interrupt\n");						/*2*/
	if(NRF_R_STATUS->TX_DS_R) printf("\tData send TX FIFO iterrupt\n");														/*3*/
	if(NRF_R_STATUS->RX_DR_R) printf("\tData ready RX FIFO interrupt\n");		/*4*/					/*5*/
}
void readOBSERVE_TX(void){
	refreshRegsterData();
	printf(SET_BACK_YELLOW);	
	printf("%sOBSERVE_TX - Transmit observe register%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	printf("\tCount lost packets :%d\n",NRF_R_OBSERVE_TX->PLOS_CNT_R);
	printf("\tCount retransmitted packets :%d\n",NRF_R_OBSERVE_TX->ARC_CNT_R);														/*0*/	/*4*/					/*5*/
}
void readRPD(void){
	refreshRegsterData();
	printf(SET_BACK_YELLOW);	
	printf("%sRPD - Recieved power detector%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	if(NRF_R_RPD->RPD_R)	printf("\tCurrier Detect\n");	
	else printf("\tCurrier not Detect\n");														/*0*/	/*4*/					/*5*/
}


void readRX_ADDR_P0(void){
	RX_ADDR_P0_tableWrite();
	printf(SET_BACK_YELLOW);	
	printf("%sRX_ADDR_P0 - Recive addres data pipe 0%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	printf("\tAddress : 0x");
	for(unsigned char i=0;i<5;i++){
		printf("%x%x",NRF_R_RX_ADDR_P0->TX_ADDR_P0_R[i]/16,NRF_R_RX_ADDR_P0->TX_ADDR_P0_R[i]%16);
	}	
	printf("\n");											/*0*/	/*4*/					/*5*/
}

void readRX_ADDR_P1(void){
	RX_ADDR_P1_tableWrite();
	printf(SET_BACK_YELLOW);	
	printf("%sRX_ADDR_P1 - Receive address data pipe 1%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	printf("\tAddress : 0x");
	for(unsigned char i=0;i<5;i++){
		printf("%x%x",NRF_R_RX_ADDR_P1->TX_ADDR_P1_R[i]/16,NRF_R_RX_ADDR_P1->TX_ADDR_P1_R[i]%16);
	}	
	printf("\n");															/*0*/	/*4*/					/*5*/
}

void readRX_ADDR_P2_P5(void){
	RX_ADDR_P2_P5_tableWrite();
	printf(SET_BACK_YELLOW);	
	printf("%sRX_ADDR_P2_P5 - Receive address data pipe 2 - 5 last byte of 5B%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);	
	printf("\tAddress pipe 2: 0x%x%x\n",NRF_R_RX_ADDR_P2->RX_ADDR_P2_R/16,NRF_R_RX_ADDR_P2->RX_ADDR_P2_R%16);	
	printf("\tAddress pipe 3: 0x%x%x\n",NRF_R_RX_ADDR_P3->RX_ADDR_P3_R/16,NRF_R_RX_ADDR_P3->RX_ADDR_P3_R%16);	
	printf("\tAddress pipe 4: 0x%x%x\n",NRF_R_RX_ADDR_P4->RX_ADDR_P4_R/16,NRF_R_RX_ADDR_P4->RX_ADDR_P4_R%16);	
	printf("\tAddress pipe 5: 0x%x%x\n",NRF_R_RX_ADDR_P5->RX_ADDR_P5_R/16,NRF_R_RX_ADDR_P5->RX_ADDR_P5_R%16);											/*0*/	/*4*/					/*5*/
}

void read_ALL_RX_PW(void){
	printf(SET_BACK_YELLOW);
	printf("%sRX_PW_P0_P5 - Number of bytes in RX payload in all data pipes%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);
	printf("\tRX_PW_P0",NRF_R_RX_PW_P0->RX_PW_P0_R);
	printf("\tRX_PW_P1",NRF_R_RX_PW_P1->RX_PW_P1_R);
	printf("\tRX_PW_P2",NRF_R_RX_PW_P2->RX_PW_P2_R);
	printf("\tRX_PW_P3",NRF_R_RX_PW_P3->RX_PW_P3_R);
	printf("\tRX_PW_P4",NRF_R_RX_PW_P4->RX_PW_P4_R);
	printf("\tRX_PW_P5",NRF_R_RX_PW_P5->RX_PW_P5_R);										/*0*/	/*4*/					/*5*/
}										/*0*/	/*4*/					/*5*/
void readFIFO_STATUS(void){
	printf(SET_BACK_YELLOW);	
	printf("%sFIFO_STATUS - FIFO status register%s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);		
	if(NRF_R_FIFO_STATUS->RX_EMPTY_R)		{printf(SET_FORE_RED);		printf("\tRX_FIFO empty\n");}		else {printf(SET_FORE_GREEN); printf("\tRX_FIFO not empty\n");}													/*0*/
	if(NRF_R_FIFO_STATUS->RX_FULL_R)		{printf(SET_FORE_RED);		printf("\tRX_FIFO full \n");}		else {printf(SET_FORE_GREEN); printf("\tRX_FIFO not full\n");}											/*1*/
	if(NRF_R_FIFO_STATUS->TX_EMPTY_R)		{printf(SET_FORE_RED);		printf("\tTX_FIFO empty\n");}		else {printf(SET_FORE_GREEN);  printf("\tTX_FIFO not empty");}						/*2*/
	if(NRF_R_FIFO_STATUS->TX_FULL_R)		{printf(SET_FORE_RED);		printf("\tTX_FIFO full \n");}		else {printf(SET_FORE_GREEN);  printf("\tTX_FIFO not full");}												/*3*/
	if(NRF_R_FIFO_STATUS->TX_REUSE_R)		printf("do sprawdzenia \n");	/*4*/
	printf(SET_FORE_WHITE);											/*0*/	/*4*/					/*5*/
}
void readDYNPD(void){
	printf(SET_BACK_YELLOW);
	printf("%sDYNPD - Dynamic payload data length %s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);
	if(NRF_R_DYN_PD->DYN_P0_R)		{printf(SET_FORE_GREEN);		printf("\tEnable dynamic data pipe 0 length\n");}		else {printf(SET_FORE_RED); printf("\tDisable dynamic data pipe 0 length\n");}													/*0*/
	if(NRF_R_DYN_PD->DYN_P1_R)		{printf(SET_FORE_GREEN);		printf("\tEnable dynamic data pipe 1 length\n");}		else {printf(SET_FORE_RED); printf("\tDisable dynamic data pipe 1 length\n");}											/*1*/
	if(NRF_R_DYN_PD->DYN_P2_R)		{printf(SET_FORE_GREEN);		printf("\tEnable dynamic data pipe 2 length\n");}		else {printf(SET_FORE_RED); printf("\tDisable dynamic data pipe 2 length\n");}	
	if(NRF_R_DYN_PD->DYN_P3_R)		{printf(SET_FORE_GREEN);		printf("\tEnable dynamic data pipe 3 length\n");}		else {printf(SET_FORE_RED); printf("\tDisable dynamic data pipe 3 length\n");}											/*3*/
	if(NRF_R_DYN_PD->DYN_P4_R)		{printf(SET_FORE_GREEN);		printf("\tEnable dynamic data pipe 4 length\n");}		else {printf(SET_FORE_RED); printf("\tDisable dynamic data pipe 4 length\n");}	
	if(NRF_R_DYN_PD->DYN_P5_R)		{printf(SET_FORE_GREEN);		printf("\tEnable dynamic data pipe 5 length\n");}		else {printf(SET_FORE_RED); printf("\tDisable dynamic data pipe 5 length\n");}													/*0*/
	printf(SET_FORE_WHITE);											/*0*/	/*4*/					/*5*/
}
void readFEATURE(void){
	printf(SET_BACK_YELLOW);
	printf("%sFEATURE - Feature registers %s\n",SET_FORE_BLACK,SET_BACK_BLACK);printf(SET_FORE_WHITE);
	if(NRF_R_FEATURE->EN_DYN_ACK_R)		{printf(SET_FORE_GREEN);		printf("\tEnables the W_TX_PAYLOAD_NOACK\n");}		else {printf(SET_FORE_RED); printf("\tDisable the W_TX_PAYLOAD_NOACK\n");}													/*0*/
	if(NRF_R_FEATURE->EN_ACK_PAY)		{printf(SET_FORE_GREEN);		printf("\tEnable payload with ack\n");}				else {printf(SET_FORE_RED); printf("\tDisable payload with ack\n");}											/*1*/
	if(NRF_R_FEATURE->EN_DPL)			{printf(SET_FORE_GREEN);		printf("\tEnable dynamic payload length\n");}		else {printf(SET_FORE_RED); printf("\tDisable  dynamic payload length\n");}												/*0*/
	printf(SET_FORE_WHITE);											/*0*/	/*4*/					/*5*/
}

void refreshRegsterData(void){
	nrf24_readAllReg();
}

void ce_High(void){
	CE_PORT.OUTSET =  (1 << CE);
}

void ce_Low(void){
	CE_PORT.OUTCLR =  (1 << CE);
}


/* init the hardware pins */

void nrf24_ce_digitalWrite(unsigned char status){
	switch(status){
		
		case HIGH:	ce_High();
		break;
		
		case LOW:	ce_Low();
		break;
	}
}
void nrf24_csn_digitalWrite(unsigned char status){
	switch(status){
		
		case HIGH:	CSN_High();
		break;
		
		case LOW:	CSN_Low();
		break;
	}
	
}
void nrf24_setupPins(){
	spi_Init();
	CE_PORT.DIR |= (1<<CE); // CE -output IRQ inputs
}

void nrf24_init()
{
#if DEBUG
	printf("inicjalizacja nrf24l01..\n");
#endif
	nrf24_setupPins();
	nrf24_ce_digitalWrite(LOW);
	nrf24_csn_digitalWrite(HIGH);
}

/* configure the module */
void nrf24_config(unsigned char channel, unsigned char pay_length)
{
	

#if DEBUG
	printf("konfiguracja nrf24l10 : kana³: %d, d³ugoœæ strumiena danych = %d bjtów\n",channel,pay_length);
#endif
	/* Use static payload length ... */
	payload_len = pay_length;

	// Set RF channel
	nrf24_configRegister(RF_CH,channel);

	// Set length of incoming payload
	nrf24_configRegister(RX_PW_P0, 0x00); // Auto-ACK pipe ...
	nrf24_configRegister(RX_PW_P1, payload_len); // Data payload pipe
	nrf24_configRegister(RX_PW_P2, 0x00); // Pipe not used
	nrf24_configRegister(RX_PW_P3, 0x00); // Pipe not used
	nrf24_configRegister(RX_PW_P4, 0x00); // Pipe not used
	nrf24_configRegister(RX_PW_P5, 0x00); // Pipe not used

	// 1 Mbps, TX gain: 0dbm
	nrf24_configRegister(RF_SETUP, (0<<RF_DR)|((0x03)<<RF_PWR));

	// CRC enable, 1 byte CRC length
	nrf24_configRegister(CONFIG,nrf24_CONFIG);

	// Auto Acknowledgment
	nrf24_configRegister(EN_AA,(1<<ENAA_P0)|(1<<ENAA_P1)|(0<<ENAA_P2)|(0<<ENAA_P3)|(0<<ENAA_P4)|(0<<ENAA_P5));

	// Enable RX addresses
	nrf24_configRegister(EN_RXADDR,(1<<ERX_P0)|(1<<ERX_P1)|(0<<ERX_P2)|(0<<ERX_P3)|(0<<ERX_P4)|(0<<ERX_P5));

	// Auto retransmit delay: 1000 us and Up to 15 retransmit trials
	nrf24_configRegister(SETUP_RETR,(0x04<<ARD)|(0x0F<<ARC));

	// Dynamic length configurations: No dynamic length
	nrf24_configRegister(DYNPD,(0<<DPL_P0)|(0<<DPL_P1)|(0<<DPL_P2)|(0<<DPL_P3)|(0<<DPL_P4)|(0<<DPL_P5));
	
	// Start listening
	nrf24_powerUpRx();
	

//#if DEBUG
	printf("ustawienia rejestrów :\n");
	//nrf24_readRegisters(0x00,&register_table ,29);
	//for(unsigned char i=0;i<0x1d;i++){
	//	nrf_Registers_table[i] = nrf24_readSingleRegister(i);
	//	printf("register[%d] = 0x%x \n",i,nrf_Registers_table[i]);
	//}
	
//#endif

	
}

/* Set the RX address */
void nrf24_rx_address(unsigned char * adr)
{
	nrf24_ce_digitalWrite(LOW);
	nrf24_writeRegister(RX_ADDR_P1,adr,nrf24_ADDR_LEN);
	nrf24_ce_digitalWrite(HIGH);
}

/* Returns the payload length */
unsigned char nrf24_payload_length()
{
	return payload_len;
}

/* Set the TX address */
void nrf24_tx_address(unsigned char* adr)
{
	/* RX_ADDR_P0 must be set to the sending addr for auto ack to work. */
	nrf24_writeRegister(RX_ADDR_P0,adr,nrf24_ADDR_LEN);
	nrf24_writeRegister(TX_ADDR,adr,nrf24_ADDR_LEN);
}

/* Checks if data is available for reading */
/* Returns 1 if data is ready ... */
unsigned char nrf24_dataReady()
{
	// See note in getData() function - just checking RX_DR isn't good enough
	unsigned char status = nrf24_getStatus();

	// We can short circuit on RX_DR, but if it's not set, we still need
	// to check the FIFO for any pending packets
	if ( status & (1 << RX_DR) )
	{
		return 1;
	}

	return !nrf24_rxFifoEmpty();
}

/* Checks if receive FIFO is empty or not */
unsigned char nrf24_rxFifoEmpty()
{
	unsigned char fifoStatus;

	nrf24_readRegister(FIFO_STATUS,&fifoStatus,1);
	
	return (fifoStatus & (1 << RX_EMPTY));
}

/* Returns the length of data waiting in the RX fifo */
unsigned char nrf24_payloadLength()
{
	unsigned char status;
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(R_RX_PL_WID);
	status = spi_Transmit(0x00);
	nrf24_csn_digitalWrite(HIGH);
	return status;
}

/* Reads payload bytes into data array */
void nrf24_getData(unsigned char* data)
{
	/* Pull down chip select */
	nrf24_csn_digitalWrite(LOW);

	/* Send cmd to read rx payload */
	spi_Transmit( R_RX_PAYLOAD );
	
	/* Read payload */
	nrf24_transferSync(data,data,payload_len);
	
	/* Pull up chip select */
	nrf24_csn_digitalWrite(HIGH);

	/* Reset status register */
	nrf24_configRegister( NRF_STATUS, ( 1 << RX_DR ) );
}

/* Returns the number of retransmissions occured for the last message */
unsigned char nrf24_retransmissionCount()
{
	unsigned char rv;
	nrf24_readRegister(OBSERVE_TX,&rv,1);
	rv = rv & 0x0F;
	return rv;
}

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
void nrf24_send(unsigned char* val)
{
	/* Go to Standby-I first */
	nrf24_ce_digitalWrite(LOW);
	
	/* Set to transmitter mode , Power up if needed */
	nrf24_powerUpTx();

	/* Do we really need to flush TX fifo each time ? */
	#if 1
	/* Pull down chip select */
	nrf24_csn_digitalWrite(LOW);

	/* Write cmd to flush transmit FIFO */
	spi_Transmit(FLUSH_TX);

	/* Pull up chip select */
	nrf24_csn_digitalWrite(HIGH);
	#endif

	/* Pull down chip select */
	nrf24_csn_digitalWrite(LOW);

	/* Write cmd to write payload */
	spi_Transmit(W_TX_PAYLOAD);

	/* Write payload */
	nrf24_transmitSync(val,payload_len);

	/* Pull up chip select */
	nrf24_csn_digitalWrite(HIGH);

	/* Start the transmission */
	nrf24_ce_digitalWrite(HIGH);
}

unsigned char nrf24_isSending()
{
	unsigned char status;

	/* read the current status */
	status = nrf24_getStatus();
	
	/* if sending successful (TX_DS) or max retries exceded (MAX_RT). */
	if((status & ((1 << TX_DS)  | (1 << MAX_RT))))
	{
		return 0; /* false */
	}

	return 1; /* true */

}

unsigned char nrf24_getStatus()
{
	unsigned char rv;
	nrf24_csn_digitalWrite(LOW);
	rv = spi_Transmit(NOP);
	nrf24_csn_digitalWrite(HIGH);
	return rv;
}

unsigned char nrf24_lastMessageStatus()
{
	unsigned char rv;

	rv = nrf24_getStatus();

	/* Transmission went OK */
	if((rv & ((1 << TX_DS))))
	{
		return NRF24_TRANSMISSON_OK;
	}
	/* Maximum retransmission count is reached */
	/* Last message probably went missing ... */
	else if((rv & ((1 << MAX_RT))))
	{
		return NRF24_MESSAGE_LOST;
	}
	/* Probably still sending ... */
	else
	{
		return 0xFF;
	}
}

void nrf24_powerUpRx()
{
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(FLUSH_RX);
	nrf24_csn_digitalWrite(HIGH);

	nrf24_ce_digitalWrite(LOW);
	nrf24_configRegister(NRF_STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT));
	nrf24_configRegister(CONFIG,nrf24_CONFIG|((1<<PWR_UP)|(1<<PRIM_RX)));
	nrf24_ce_digitalWrite(HIGH);
}

void nrf24_powerUpTx()
{
	nrf24_configRegister(NRF_STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT));

	nrf24_configRegister(CONFIG,nrf24_CONFIG|((1<<PWR_UP)|(0<<PRIM_RX)));
}

void nrf24_powerDown()
{
	nrf24_ce_digitalWrite(LOW);
	nrf24_configRegister(CONFIG,nrf24_CONFIG);
}

/* software spi routine */
// unsigned char spi_Transmit(unsigned char tx)
// {
// 	unsigned char i = 0;
// 	unsigned char rx = 0;
// 
// 	nrf24_sck_digitalWrite(LOW);
// 
// 	for(i=0;i<8;i++)
// 	{
// 
// 		if(tx & (1<<(7-i)))
// 		{
// 			nrf24_mosi_digitalWrite(HIGH);
// 		}
// 		else
// 		{
// 			nrf24_mosi_digitalWrite(LOW);
// 		}
// 
// 		nrf24_sck_digitalWrite(HIGH);
// 
// 		rx = rx << 1;
// 		if(nrf24_miso_digitalRead())
// 		{
// 			rx |= 0x01;
// 		}
// 
// 		nrf24_sck_digitalWrite(LOW);
// 
// 	}
// 
// 	return rx;
// }

/* send and receive multiple bytes over SPI */
void nrf24_transferSync(unsigned char* dataout,unsigned char* datain,unsigned char len)
{
	unsigned char i;

for(i=0;i<len;i++)
	{
		printf("0x%x",dataout[i]);
		datain[i] = spi_Transmit(dataout[i]);
	}

}

/* send multiple bytes over SPI */
void nrf24_transmitSync(unsigned char* dataout,unsigned char len)
{
	unsigned char i;
	
	for(i=0;i<len;i++)
	{
		spi_Transmit(dataout[i]);
	}

}

/* Clocks only one byte into the given nrf24 register */
void nrf24_configRegister(unsigned char reg, unsigned char val)
{
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(W_REGISTER | (REGISTER_MASK & reg));
	spi_Transmit(val);
	nrf24_csn_digitalWrite(HIGH);
}

/* Read single register from nrf24 */
/************************************************************************/
/*  reg - first register to read
	val - data to send 
	len - size of val                                                                      */
/************************************************************************/
void nrf24_readRegister(unsigned char reg, unsigned char* val, unsigned char len)
{
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(R_REGISTER | (REGISTER_MASK & reg));
	nrf24_transferSync(0xff,val,len); // val
	nrf24_csn_digitalWrite(HIGH);
}

/* Write to a single register of nrf24 */
void nrf24_writeRegister(unsigned char reg, unsigned char* val, unsigned char len)
{
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(W_REGISTER | (REGISTER_MASK & reg));
	nrf24_transmitSync(val,len);
	nrf24_csn_digitalWrite(HIGH);
}

void nrf24_writeSingleRegister(unsigned char reg, unsigned char val)
{
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(W_REGISTER | (REGISTER_MASK & reg));
	spi_Transmit(val);
	nrf24_csn_digitalWrite(HIGH);
}

unsigned char nrf24_readSingleRegister(unsigned char reg)
{
	unsigned char readData;
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(R_REGISTER | (REGISTER_MASK & reg));
	readData = spi_Transmit(NOP);
	nrf24_csn_digitalWrite(HIGH);
	return readData;
}
void nrf24_readAllReg(unsigned char* table){
	unsigned char address = 0x00;
	
	for(unsigned char i=0;i<0x09;i++){
		nrf24_csn_digitalWrite(LOW);
		spi_Transmit(R_REGISTER | (REGISTER_MASK & address++));
		*(nrf_Registers_table+i) = spi_Transmit(NOP);
		nrf24_csn_digitalWrite(HIGH);
	}
	
	address = 0x11;
	for(unsigned char i=10;i<0x16;i++){
		nrf24_csn_digitalWrite(LOW);
		spi_Transmit(R_REGISTER | (REGISTER_MASK & address++));
		*(nrf_Registers_table+i) = spi_Transmit(NOP);
		nrf24_csn_digitalWrite(HIGH);
	}
	
	address = 0x1C;
	for(unsigned char i=17;i<0x18;i++){
		nrf24_csn_digitalWrite(LOW);
		spi_Transmit(R_REGISTER | (REGISTER_MASK & address++));
		*(nrf_Registers_table+i) = spi_Transmit(NOP);
		nrf24_csn_digitalWrite(HIGH);
	}
	
}

void nrf24_transferRead(unsigned char* datain,unsigned char len)
{
	unsigned char i;

	for(i=0;i<len;i++)
	{
		printf("%d : %d  --->",i,datain[i]);	
		datain[i] = spi_Transmit(0xff);
		printf("%d : %d\n",i,datain[i]);	
	}
}

void nrf24_readRegisters(unsigned char reg, unsigned char* val, unsigned char len)
{
	nrf24_csn_digitalWrite(LOW);
	printf("readRegister : %d %d %d",reg,*val,len);
	spi_Transmit(R_REGISTER | (REGISTER_MASK & reg));
	nrf24_transferRead(val,len); // val
	nrf24_csn_digitalWrite(HIGH);
}

void RX_ADDR_P1_tableWrite(void)
{
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(R_REGISTER | (REGISTER_MASK & RX_ADDR_P1));
	for(unsigned char i=0;i<5;i++){
		
		*(RX_Address_P1+i) = spi_Transmit(NOP);
	}
	nrf24_csn_digitalWrite(HIGH);
}

void RX_ADDR_P0_tableWrite(void){
	nrf24_csn_digitalWrite(LOW);
	spi_Transmit(R_REGISTER | (REGISTER_MASK & RX_ADDR_P0));
	for(unsigned char i=0;i<5;i++){
		
		*(RX_Address_P0+i) = spi_Transmit(NOP);
	}
	nrf24_csn_digitalWrite(HIGH);
}
void RX_ADDR_P2_P5_tableWrite(void){
	unsigned char address = RX_ADDR_P2;
	for(unsigned char i=0;i<5;i++){
		nrf24_csn_digitalWrite(LOW);
		spi_Transmit(R_REGISTER | (REGISTER_MASK & address++));
		*(RX_Address_P2_P5+i) = spi_Transmit(NOP);
		nrf24_csn_digitalWrite(HIGH);
	}
	
}