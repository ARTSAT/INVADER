/*************************************************************************************
* �t�@�C�����@�F�@sci1.c
* �^�C�g���@�@�F�@INVADER�V���A���ʐM(CH1)�v���O����
* ����ҁ@�@�@�F�@03-110351 �O�D�����@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.29
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* PRISM(old_version_8.0~)�̃v���O�������Q�l�ɃR�[�f�B���O
*
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "sci.h"

#define SCI_BUFF_SIZE	256
#define	PCLOCK		(25000000)			// �O���N���b�N(Hz)

static volatile unsigned char rx1_buff[SCI_BUFF_SIZE];
static volatile unsigned char tx1_buff[SCI_BUFF_SIZE];
static volatile unsigned short rx1_rp;
static volatile unsigned short rx1_wp;
static volatile unsigned short rx1_length;
static volatile unsigned short tx1_rp;
static volatile unsigned short tx1_wp;
static volatile unsigned short tx1_length;

/*************************************************
		�V���A���|�[�g������
**************************************************/
unsigned char sci1_init(
	unsigned short  bps,				// 4800~
	unsigned char   bit,				// 8 or 7
	unsigned char   parity,				// 0:NON 1:EVN 2:ODD
	unsigned char   stopBit)			// 1 or 2
{
	unsigned char smr;					// �V���A�����[�h���W�X�^
	unsigned char brr;					// �r�b�g���[�g���W�X�^
	unsigned char dummy;				// SSR�p�_�~�[�E���[�h
  	
	int i;
	
	// RX FIFO(First In First Out)
	rx1_rp = 0;
	rx1_wp = 0;
	rx1_length = 0;
	
	// TX FIFO(First In First Out)
	tx1_rp = 0;
	tx1_wp = 0;
	tx1_length = 0;
	
	SCI1.SCR.BYTE = 0x00;
	SCI1.SMR.BYTE = 0x00;
	
	// SMR
	smr = 0;
	
	if ( 7 == bit ){			// �r�b�g��=7
		smr |= 0x40;
	}
	if ( 1 == parity ){			// ��p���e�B
		smr |= 0x20;
	}
	else if ( 2 == parity ){	// �����p���e�B
		smr |= 0x30;
	}
	if ( 2 == stopBit ){		// �X�g�b�v�r�b�g=2
		smr |= 0x08;
	}
	
	// BRR
	brr = (unsigned char)(((PCLOCK / 32) / bps) - 1);
	
	SCI1.SMR.BYTE = smr;
	SCI1.BRR = brr;
	for(i = 0; i < 3000; i++);	// ���Ȃ��Ƃ�1�r�b�g���ԑ҂�
	
	SCI1.SCR.BYTE = 0x70;		// ����M�A��M���荞�݂̂݋���
	
	dummy = SCI1.SSR.BYTE;		// dummy read
	SCI1.SSR.BYTE = 0x84;		// ���M���荞�݊֌W�̐ݒ� TDRE = 1,TEND = 1
	
	return 1;
}

/*************************************************
		1�����o��
*************************************************/
short sci1_putc(unsigned char data)
{
	short ret = -1;
	
	while(SCI1.SSR.BIT.TDRE == 0);				// TDR���󂭂܂ő҂�
	SCI1.SCR.BYTE &= (unsigned char)(~0x80);	// ���M�����݋֎~
	
	if(SCI_BUFF_SIZE > tx1_length)
	{
		tx1_buff[tx1_wp] = data;				// TX_BUFFER��1�����i�[
		tx1_wp++;
		if(tx1_wp >= SCI_BUFF_SIZE)
		{
			tx1_wp = 0;
		}
		tx1_length++;
		
		ret = data;
	}
	else{
	}
	SCI1.SCR.BYTE |= 0x80;						// ���M�����݋���
	
	return (ret);
}

/*************************************************
		Hexadecimal
*************************************************/
void sci1_puthex(unsigned char data)
{
	sci1_putc((unsigned char)HexToAscii((unsigned char)((data&0xf0)>>4)));
	sci1_putc((unsigned char)HexToAscii((unsigned char)(data&0x0f) ));
}

/*************************************************
		Decimal
*************************************************/
void sci1_putdec(unsigned long data)
{
	unsigned long div = 1000000000;
	unsigned long copy = data;
	unsigned char val[10];
	unsigned char index=0;
	unsigned char flag = 0;
	unsigned char break_flag = 0;
	while(1)
	{
		val[index] = (unsigned char)(copy / div);
		copy = copy - val[index] * div;
		if(div <= 1)
		{
			break_flag = 1;			//1�̈ʂ܂ł������Ƃ����o
		}
		div /= 10;
    
		if((flag)||(break_flag))	//���ł�0�łȂ��ʂɏo��������C���邢�͍Ō�܂�0�ł�������
		{
			sci1_putc((unsigned char)(val[index] + 0x30));
		}
		else if(val[index] != 0)	//�ŏ��ɗ�łȂ��ʂ������܂ŏo�͂��Ȃ�
		{
			flag = 1;
			sci1_putc((unsigned char)(val[index] + 0x30));
		}
		
		index++;
		if(break_flag) 
		{
			break;					//1�̈ʂ܂ł����̂ł�߂�
		}
	}
	return;
}

/*************************************************
		DoubleHex
*************************************************/
void sci1_putdoublehex(unsigned short data)
{
	sci1_puthex((unsigned char)(data>>8));
	sci1_puthex((unsigned char)(data&0x00ff));
}

/*************************************************
		FourHex
*************************************************/
void sci1_putfourhex(unsigned long data)
{
	sci1_putdoublehex((unsigned short)(data >> 16));
	sci1_putdoublehex((unsigned short)(data&0xffff));
}

/*************************************************
		EightHex
*************************************************/
/*
void sci1_puteighthex(unsigned long data)
{
	sci1_putfourhex((unsigned int)(data >> 32));
	sci1_putfourhex((unsigned int)(data & 0xffffffff));
}
*/

/*************************************************
		���s
*************************************************/
void sci1_putcrlf(void)
{
	sci1_putc('\r');
	sci1_putc('\n');
}

/*************************************************
		������i�[
*************************************************/
short sci1_puts(unsigned char *str)
{	
	int i;
	
	for(i=0;i<100;i++){
		if(str[i]=='\0') break;
		sci1_putc(str[i]);
	}
	return 1;
}

/*************************************************
		1�����擾
************************************************/
short sci1_getc()
{
	short data;
	
	data = -1;
	
	SCI1.SCR.BYTE &= (unsigned char)(~0x40);	// ��M�����݋֎~
	if (0 < rx1_length){
		data = rx1_buff[rx1_rp];
		rx1_rp ++;
		if(rx1_rp >= SCI_BUFF_SIZE)
		{
			rx1_rp = 0;
		}
		rx1_length--;
	}
	SCI1.SCR.BYTE |= 0x40;						// ��M�����݋���
	
	return(data);
}

/************************************************
		��M�G���[���荞��
*************************************************/
void INT_ERI1(void)
{
	SCI1.SSR.BYTE &= ~0x38;		// �G���[�t���O�N���A
}

/*************************************************
		������i�[ RX�̂ق�
*************************************************/
void sci1_puts_rx(unsigned char *buff,int num){
	unsigned char data;
	int i;
//	debug_puts("\r\n");
	SCI1.SCR.BYTE &= (unsigned char)(~0x40);// ��M�����݋֎~
	for(i=0;i<num+1;i++){
		if(buff[i]=='\0') break;
		data = buff[i];
		SCI1.SSR.BYTE &= ~0x40;	
		if(SCI_BUFF_SIZE > rx1_length){
			rx1_buff[rx1_wp] = data;		// RX_BUFFER��1�����i�[
			rx1_wp ++;
//			debug_puthex(data);
//			debug_puts(" ");
			if(rx1_wp >= SCI_BUFF_SIZE){
				rx1_wp = 0;
			}
			rx1_length++;	
		}
	}
	SCI1.SCR.BYTE |= 0x40;					// ��M�����݋���
//	debug_puts("\r\n");
	
/*	debug_puts("rx1_buff check...\r\n");
	for(i=0;i<SCI_BUFF_SIZE;i++){
		debug_puthex(rx1_buff[i]);
		debug_puts(" ");
	}
	debug_puts("\r\nrx1_buff check done\r\n");
	*/
}	

/*************************************************
		��M���荞��
*************************************************/
void INT_RXI1(void)
{
	unsigned char data;
	
	data = SCI1.RDR;			// ��M
	SCI1.SSR.BYTE &= ~0x40;		// RDRF�N���A
	
	if(SCI_BUFF_SIZE > rx1_length){
		rx1_buff[rx1_wp] = data;		// RX_BUFFER��1�����i�[
		rx1_wp ++;
		if(rx1_wp >= SCI_BUFF_SIZE)
		{
			rx1_wp = 0;
		}
		rx1_length++;
	}
}

/*************************************************
		���M���荞��
*************************************************/
void INT_TXI1(void)
{
	unsigned char data;
	
	if (0 < tx1_length){
		data = tx1_buff[tx1_rp];
		
		tx1_rp++;
		if(tx1_rp >= SCI_BUFF_SIZE)
		{
			tx1_rp = 0;
		}
		tx1_length--;
		
		while(SCI1.SSR.BIT.TDRE == 0);
		SCI1.TDR = data;							// 1�o�C�g���M
		SCI1.SSR.BYTE &= (unsigned char)(~0x80);	// ���M���荞�݋֎~
	}
	else
	{
		SCI1.SCR.BYTE &= (unsigned char)(~0x80);	// ���M�����݋֎~
	}
}

/*************************************************
		���M�I��������
*************************************************/
void INT_TEI1(void)
{
	SCI1.SCR.BYTE &= ~0x04;							// ���M�����݋֎~
}
