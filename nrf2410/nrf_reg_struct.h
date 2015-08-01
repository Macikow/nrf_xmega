/*
 * nrf_reg_struct.h
 *
 * Created: 2015-08-01 21:43:05
 *  Author: Macikow
 */ 


#ifndef NRF_REG_STRUCT_H_
#define NRF_REG_STRUCT_H_

/*
	STRUKTURY S£U¯¥ G£ÓWNIE DO ODCZYTYWANIA STANÓW REJETRÓW FUNKCYJNYCH TRANSMITERA nrf w trybie debug b¹dz w innych funkcjonalnosciach rozwojowych
*/
unsigned char nrf_Registers_table[0x1d]; // tablica z danymi z refjerór nrf'a

#define NRF_R_CONFIG		((struct REG_CONFIG *)			(&nrf_Registers_table[0])) //0x00
#define NRF_R_EN_AA			((struct REG_EN_AA *)			(&nrf_Registers_table[1])) //0x01
#define NRF_R_SETUP_AW		((struct REG_SETUP_AW *)		(&nrf_Registers_table[2])) //0x02
#define NRF_R_SETUP_RETR	((struct REG_SETUP_RETR *)		(&nrf_Registers_table[3])) //0x03
#define NRF_R_RF_CH			((struct REG_RF_CH *)			(&nrf_Registers_table[4])) //0x04
#define NRF_R_RF_SETUP		((struct REG_RF_SETUP *)		(&nrf_Registers_table[5])) //0x05
#define NRF_R_STATUS		((struct REG_STATUS *)			(&nrf_Registers_table[6])) //0x06
#define NRF_R_OBSERVE_TX	((struct REG_OBSERVE_TX *)		(&nrf_Registers_table[7])) //0x07
#define NRF_R_RPD			((struct REG_RPD *)				(&nrf_Registers_table[8])) //0x08
#define NRF_R_EN_RXADDR		((struct REG_EN_RXADDR *)		(&nrf_Registers_table[9])) //0x09
// adresy RX, TX 0x0A - 0x10
#define NRF_R_RX_PW_P0 		((struct REG_RX_PW_P0 *)		(&nrf_Registers_table[10])) //0x11
#define NRF_R_RX_PW_P1 		((struct REG_RX_PW_P1 *)		(&nrf_Registers_table[11])) //0x12
#define NRF_R_RX_PW_P2 		((struct REG_RX_PW_P2 *)		(&nrf_Registers_table[12])) //0x13
#define NRF_R_RX_PW_P3 		((struct REG_RX_PW_P3 *)		(&nrf_Registers_table[13])) //0x14
#define NRF_R_RX_PW_P4 		((struct REG_RX_PW_P4 *)		(&nrf_Registers_table[14])) //0x15
#define NRF_R_RX_PW_P5 		((struct REG_RX_PW_P5 *)		(&nrf_Registers_table[15])) //0x16
#define NRF_R_FIFO_STATUS	((struct REG_FIFO_STATUS *)		(&nrf_Registers_table[16])) //0x17
// rejestry funkcyjne transmitera 0x18 - 0x1B
#define NRF_R_DYN_PD		((struct REG_DYN_PD *)			(&nrf_Registers_table[17])) //0x1C
#define NRF_R_FEATURE		((struct REG_FEATURE *)			(&nrf_Registers_table[18])) //0x1D


struct REG_CONFIG {
	unsigned PRIM_RX_R		:1;/*0*/
	unsigned PWR_UP_R		:1;/*1*/
	unsigned CRCO_R			:1;/*2*/
	unsigned EN_CRC_R		:1;/*3*/
	unsigned MASK_MAX_RT_R	:1;/*4*/
	unsigned MASK_TX_DS_R	:1;/*5*/
	unsigned MASK_RX_DS_R	:1;/*6*/
	unsigned RES_S			:1;/*7*/
};
struct REG_EN_AA {
	unsigned ENAA_P0_R		:1;/*0*/
	unsigned ENAA_P1_R		:1;/*1*/
	unsigned ENAA_P2_R		:1;/*2*/
	unsigned ENAA_P3_R		:1;/*3*/
	unsigned ENAA_P4_R		:1;/*4*/
	unsigned ENAA_P5_R		:1;/*5*/
	unsigned RES_S1			:1;/*6*/
	unsigned RES_S2			:1;/*7*/
};
struct REG_EN_RXADDR {
	unsigned ERX_P0_R		:1;/*0*/
	unsigned ERX_P1_R		:1;/*1*/
	unsigned ERX_P2_R		:1;/*2*/
	unsigned ERX_P3_R		:1;/*3*/
	unsigned ERX_P4_R		:1;/*4*/
	unsigned ERX_P5_R		:1;/*5*/
	unsigned RES_S1			:1;/*6*/
	unsigned RES_S2			:1;/*7*/
};
struct REG_SETUP_AW{
	unsigned AW_R			:2;/*01*/
};
struct REG_SETUP_RETR{
	unsigned ADR_R			:4;
	unsigned ARC_R			:4;
};
struct REG_RF_CH{
	unsigned RF_CH_R		:7;
};
struct REG_RF_SETUP {
	unsigned DONTCARE		:1;/*0*/
	unsigned RF_PWR_R		:1;/*1*/
	unsigned RF_DR_R		:1;/*2*/
	unsigned PLL_LOCK_R		:1;/*3*/
	unsigned RF_DR_L_R		:1;/*4*/
	unsigned ERX_P5_R		:1;/*5*/
	unsigned CONT_WAVE		:1;/*5*/
};
struct REG_STATUS	{
	unsigned TX_FULL_R		:1;/*0*/
	unsigned RX_P_NO_R		:3;/*123*/
	unsigned MAX_RX_R		:1;/*4*/
	unsigned TX_DS_R		:1;/*5*/
	unsigned RX_DR_R		:1;/*6*/
};
struct REG_OBSERVE_TX{
	unsigned PLOS_CNT_R		:4;/*0*/
	unsigned ARC_CNT_R		:4;/*1*/
};
struct REG_RPD{
	unsigned RPD_R			:1;/*0 Currier detect*/
};

struct REG_RX_ADDR{
	unsigned char RX_ADDR_P0_R[5];
	unsigned char RX_ADDR_P1_R[5];
	unsigned char RX_ADDR_P2_R;
	unsigned char RX_ADDR_P3_R;
	unsigned char RX_ADDR_P4_R;
	unsigned char RX_ADDR_P5_R;
};
struct REG_TX_ADDR{
	unsigned char TX_ADDR_R[5];
};
struct REG_RX_PW_P0{
	unsigned RX_PW_P0_R;
};
struct REG_RX_PW_P1{
	unsigned RX_PW_P1_R;
};
struct REG_RX_PW_P2{
	unsigned RX_PW_P2_R;
};
struct REG_RX_PW_P3{
	unsigned RX_PW_P3_R;
};
struct REG_RX_PW_P4{
	unsigned RX_PW_P4_R;
};
struct REG_RX_PW_P5{
	unsigned RX_PW_P5_R;
};
struct REG_FIFO_STATUS {
	unsigned RX_EMPTY_R		:1;/*0*/
	unsigned RX_FULL_R		:1;/*1*/
	unsigned RESERVED		:2;/*23*/
	unsigned TX_EMPTY_R		:1;/*4*/
	unsigned TX_FULL_R		:1;/*5*/
	unsigned TX_REUSE_R		:1;/*6*/
};

struct REG_DYN_PD {
	unsigned DYN_P0_R		:1;/*0*/
	unsigned DYN_P1_R		:1;/*1*/
	unsigned DYN_P2_R		:1;/*2*/
	unsigned DYN_P3_R		:1;/*3*/
	unsigned DYN_P4_R		:1;/*4*/
	unsigned DYN_P5_R		:1;/*5*/
};

struct REG_FEATURE{
	unsigned EN_DYN_ACK_R	:1;/*0*/
	unsigned EN_ACK_PAY		:1;/*1*/
	unsigned EN_DPL		    :2;/*2*/
};


#endif /* NRF_REG_STRUCT_H_ */