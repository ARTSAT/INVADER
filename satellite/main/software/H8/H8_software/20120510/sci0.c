/*************************************************************************************
* �t�@�C�����@�F�@sci0.c
* �^�C�g���@�@�F�@INVADER�V���A���ʐM(CH0)�v���O����
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

static volatile unsigned char	rx0_buff[SCI_BUFF_SIZE];
static volatile unsigned char	tx0_buff[SCI_BUFF_SIZE];
static volatile unsigned short	rx0_rp;
static volatile unsigned short	rx0_wp;
static volatile unsigned short	rx0_length;
static volatile unsigned short	tx0_rp;
static volatile unsigned short	tx0_wp;
static volatile unsigned short	tx0_length;

/*************************************************
		�V���A���|�[�g������
**************************************************/
unsigned char sci0_init(
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
	rx0_rp = 0;
	rx0_wp = 0;
	rx0_length = 0;
	
	// TX FIFO(First In First Out)
	tx0_rp = 0;
	tx0_wp = 0;
	tx0_length = 0;
	
	SCI0.SCR.BYTE = 0x00;
	SCI0.SMR.BYTE = 0x00;
	
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
	
	SCI0.SMR.BYTE = smr;
	SCI0.BRR = brr;
	for(i = 0; i < 3000; i++);	// ���Ȃ��Ƃ�1�r�b�g���ԑ҂�
	
	SCI0.SCR.BYTE = 0x70;		// ����M�A��M���荞�݂̂݋���
	
	dummy = SCI0.SSR.BYTE;		// dummy read
	SCI0.SSR.BYTE = 0x84;		// ���M���荞�݊֌W�̐ݒ� TDRE = 1,TEND = 1
	
	return 1;
}

/*************************************************
                  1�����o��
*************************************************/
short sci0_putc(unsigned char data)
{
	short ret = -1;
	
	while(SCI0.SSR.BIT.TDRE == 0);				// TDR���󂭂܂ő҂�
	SCI0.SCR.BYTE &= (unsigned char)(~0x80);	// ���M�����݋֎~
	
	if(SCI_BUFF_SIZE > tx0_length)
	{
		tx0_buff[tx0_wp] = data;				// TX_BUFFER��1�����i�[
		tx0_wp++;
		if(tx0_wp >= SCI_BUFF_SIZE)
		{
			tx0_wp = 0;
		}
		tx0_length++;
    	ret = data;
	}
	else{
	}
	SCI0.SCR.BYTE |= 0x80;						// ���M�����݋���
	
	return (ret);
}

/*************************************************
                Hexadecimal
*************************************************/
void sci0_puthex(unsigned char data)
{
	sci0_putc((unsigned char)HexToAscii((unsigned char)((data&0xf0)>>4)));
	sci0_putc((unsigned char)HexToAscii((unsigned char)(data&0x0f) ));
}

/*************************************************
                  Decimal
*************************************************/
void sci0_putdec(unsigned long data)
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
			break_flag = 1;			// 1�̈ʂ܂ł������Ƃ����o
		}
		div /= 10;
		
		if((flag)||(break_flag))	// ���ł�0�łȂ��ʂɏo��������C���邢�͍Ō�܂�0�ł�������
		{
			sci0_putc((unsigned char)(val[index] + 0x30));
		}
		else if(val[index] != 0)	// �ŏ��ɗ�łȂ��ʂ������܂ŏo�͂��Ȃ�
		{
			flag = 1;
			sci0_putc((unsigned char)(val[index] + 0x30));
		}
		
		index++;
		if(break_flag) 
		{
			break;					// 1�̈ʂ܂ł����̂ł�߂�
		}
	}
	return;
}

/*************************************************
                DoubleHex
*************************************************/
void sci0_putdoublehex(unsigned short data)
{
	sci0_puthex((unsigned char)(data>>8));
	sci0_puthex((unsigned char)(data&0x00ff));
}

/*************************************************
                 FourHex
*************************************************/
void sci0_putfourhex(unsigned long data)
{
	sci0_putdoublehex((unsigned short)(data >> 16));
	sci0_putdoublehex((unsigned short)(data&0xffff));
}

/*************************************************
                 EightHex
*************************************************/
/*
void sci0_puteighthex(unsigned long data)
{
	sci0_putfourhex((unsigned int)(data >> 32));
	sci0_putfourhex((unsigned int)(data & 0xffffffff));
}
*/

/*************************************************
                  ���s
*************************************************/
void sci0_putcrlf(void)
{
	sci0_putc('\r');
	sci0_putc('\n');
}

/*************************************************
               ������i�[
*************************************************/
short sci0_puts(unsigned char *str)
{
	int i;
	for(i=0;i<100;i++){
		if(str[i]=='\0') break;
		sci0_putc(str[i]);
	}
	return 1;
}

/*************************************************
                1�����擾
************************************************/
short sci0_getc()
{
	short data;
	
	data = -1;
	
	SCI0.SCR.BYTE &= (unsigned char)(~0x40);		// ��M�����݋֎~
	if (0 < rx0_length) {
		data = rx0_buff[rx0_rp];
		rx0_rp ++;
		if(rx0_rp >= SCI_BUFF_SIZE)
		{
			rx0_rp = 0;
		}
		rx0_length--;
	}
	SCI0.SCR.BYTE |= 0x40;							// ��M�����݋���
	
	return(data);
}

/************************************************
              ��M�G���[���荞��
*************************************************/
void INT_ERI0(void)
{
	SCI0.SSR.BYTE &= ~0x38;		// �G���[�t���O�N���A
}

/*************************************************
               ��M���荞��
*************************************************/
void INT_RXI0(void)
{
	unsigned char data;
	
	data = SCI0.RDR;			// ��M
	SCI0.SSR.BYTE &= ~0x40;		// RDRF�N���A
	
	if(SCI_BUFF_SIZE > rx0_length){
		rx0_buff[rx0_wp] = data;		// RX_BUFFER��1�����i�[
		rx0_wp ++;
		if(rx0_wp >= SCI_BUFF_SIZE)
		{
			rx0_wp = 0;
		}
		rx0_length++;
	}
}

/*************************************************
                  ���M���荞��
*************************************************/
void INT_TXI0(void)
{
	unsigned char data;
	
	if (0 < tx0_length){
		data = tx0_buff[tx0_rp];
		
		tx0_rp++;
		if(tx0_rp >= SCI_BUFF_SIZE)
		{
			tx0_rp = 0;
		}
		tx0_length--;
		
		while(SCI0.SSR.BIT.TDRE == 0);
		SCI0.TDR = data;							// 1�o�C�g���M
		SCI0.SSR.BYTE &= (unsigned char)(~0x80);	// ���M���荞�݋֎~
	}
	else{
		SCI0.SCR.BYTE &= (unsigned char)(~0x80);	// ���M�����݋֎~
	}
}

/*************************************************
              ���M�I��������
*************************************************/
void INT_TEI0(void)
{
	SCI0.SCR.BYTE &= ~0x04;							// ���M�����݋֎~
}
