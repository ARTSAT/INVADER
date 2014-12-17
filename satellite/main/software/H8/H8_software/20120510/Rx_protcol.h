/*Rx用*/

#ifndef _RX_PROTCOL_H_
#define _RX_PROTCOL_H_


/*comm_biffer.c用*/
#include "stdlib.h"
/*rx_pll_inv.c用*/
#include"rx_pin_as.h"
#include"timer.h"
/*rx_protocol_inv.c用*/
#include"rx_pin_as.h" //MX_M0など、モデムの端子をH8のポートに割り当て 江
#include"sci.h" //H8のシリアル通信用の関数の定義。debug_putsとか。sci0.cやsci1.cとちょっと違う　江
#include "Ax_to_info.h"



/*comm_biffer.c用*/
unsigned char *uplink_rx_buff;  /* アップリンク受用バッファ*/
/*rx_pll_inv.c用*/
/*rx_protocol_inv.c用*/
static int oldstate; //前のビット
unsigned int kbit; 
int shiftbuff;




/*comm_biffer.c用*/
int comm_buffer_init(void);
/*rx_pll_inv.c用*/
void out_bit(int);
void strobe(void);
void out_bits(int, long);
void set_reference_divider(long);
void set_option_register(int ,int ,int ,int ,int );
void set_programmable_divider(int ,long );
void pll_init_rx(long ,int ,long ,int ,int ,int ,int ,int );
/*rx_protocol_inv.c用*/
void init_itu(void);
int bitin(void);
unsigned char get_byte(int);
int get_buff(void);
//void precrc_bit(unsigned char);
//void Ax_to_info(unsigned char *);
int get_data(void);
void rx_init(void);



/*comm_biffer.c用*/
#define com_STARTFLAG_BUFFER_LENGTH 4
#define com_RX_BUFFER_LENGTH 128
#define com_HEADA_BUFFER_LENGTH 14
#define com_FOOTA_BUFFER_LENGTH 3
#define com_RSC_LENGTH 0
#define com_UPLINK_BUFFER_LENGTH 223/*163? アップリンク受信バッファ長 */
/*rx_pll_inv.c用*/
/*rx_protocol_inv.c用*/
#define ONE_BIT_CYCLE  360//360
#define TEST_1BIT 1
#define info_length 203

volatile unsigned char flag_rx[3];

#endif
