/*Rx�p*/

#ifndef _RX_PROTCOL_H_
#define _RX_PROTCOL_H_


/*comm_biffer.c�p*/
#include "stdlib.h"
/*rx_pll_inv.c�p*/
#include"rx_pin_as.h"
#include"timer.h"
/*rx_protocol_inv.c�p*/
#include"rx_pin_as.h" //MX_M0�ȂǁA���f���̒[�q��H8�̃|�[�g�Ɋ��蓖�� �]
#include"sci.h" //H8�̃V���A���ʐM�p�̊֐��̒�`�Bdebug_puts�Ƃ��Bsci0.c��sci1.c�Ƃ�����ƈႤ�@�]
#include "Ax_to_info.h"



/*comm_biffer.c�p*/
unsigned char *uplink_rx_buff;  /* �A�b�v�����N��p�o�b�t�@*/
/*rx_pll_inv.c�p*/
/*rx_protocol_inv.c�p*/
static int oldstate; //�O�̃r�b�g
unsigned int kbit; 
int shiftbuff;




/*comm_biffer.c�p*/
int comm_buffer_init(void);
/*rx_pll_inv.c�p*/
void out_bit(int);
void strobe(void);
void out_bits(int, long);
void set_reference_divider(long);
void set_option_register(int ,int ,int ,int ,int );
void set_programmable_divider(int ,long );
void pll_init_rx(long ,int ,long ,int ,int ,int ,int ,int );
/*rx_protocol_inv.c�p*/
void init_itu(void);
int bitin(void);
unsigned char get_byte(int);
int get_buff(void);
//void precrc_bit(unsigned char);
//void Ax_to_info(unsigned char *);
int get_data(void);
void rx_init(void);



/*comm_biffer.c�p*/
#define com_STARTFLAG_BUFFER_LENGTH 4
#define com_RX_BUFFER_LENGTH 128
#define com_HEADA_BUFFER_LENGTH 14
#define com_FOOTA_BUFFER_LENGTH 3
#define com_RSC_LENGTH 0
#define com_UPLINK_BUFFER_LENGTH 223/*163? �A�b�v�����N��M�o�b�t�@�� */
/*rx_pll_inv.c�p*/
/*rx_protocol_inv.c�p*/
#define ONE_BIT_CYCLE  360//360
#define TEST_1BIT 1
#define info_length 203

volatile unsigned char flag_rx[3];

#endif
