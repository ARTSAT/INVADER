/*************************************************************************************
* �t�@�C�����@�F�@sci2.c
* �^�C�g���@�@�F�@INVADER�V���A���ʐM(CH2)�v���O����
* ����ҁ@�@�@�F�@03-110351 �O�D�����@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.5.14
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* PRISM(old_version_8.0~)�̃v���O�������Q�l�ɃR�[�f�B���O
* H8/3048F����H8/3069F�ւ̈ڍs�ɓ�����Asci0.c, sci1.c�ɉ����Ēǉ�
*
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "sci.h"

#define SCI_BUFF_SIZE	256
#define	PCLOCK		(25000000)			// �O���N���b�N(Hz)

static volatile unsigned char	rx2_buff[SCI_BUFF_SIZE];
static volatile unsigned char	tx2_buff[SCI_BUFF_SIZE];
static volatile unsigned short	rx2_rp;
static volatile unsigned short	rx2_wp;
static volatile unsigned short	rx2_length;
static volatile unsigned short	tx2_rp;
static volatile unsigned short	tx2_wp;
static volatile unsigned short	tx2_length;

/*************************************************
		�V���A���|�[�g������
**************************************************/
unsigned char sci2_init(
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
	rx2_rp = 0;
	rx2_wp = 0;
	rx2_length = 0;
	
	// TX FIFO(First In First Out)
	tx2_rp = 0;
	tx2_wp = 0;
	tx2_length = 0;
	
	SCI2.SCR.BYTE = 0x00;
	SCI2.SMR.BYTE = 0x00;
	
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
	
	SCI2.SMR.BYTE = smr;
	SCI2.BRR = brr;
	for(i = 0; i < 3000; i++);	// ���Ȃ��Ƃ�1�r�b�g���ԑ҂�
	
	SCI2.SCR.BYTE = 0x70;		// ����M�A��M���荞�݂̂݋���
	
	dummy = SCI2.SSR.BYTE;		// dummy read
	SCI2.SSR.BYTE = 0x84;		// ���M���荞�݊֌W�̐ݒ� TDRE = 1,TEND = 1
	
	return 1;
}

/*************************************************
                  1�����o��
*************************************************/
short sci2_putc(unsigned char data)
{
	short ret = -1;
	
	while(SCI2.SSR.BIT.TDRE == 0);				// TDR���󂭂܂ő҂�
	SCI2.SCR.BYTE &= (unsigned char)(~0x80);	// ���M�����݋֎~
	
	if(SCI_BUFF_SIZE > tx2_length)
	{
		tx2_buff[tx2_wp] = data;				// TX_BUFFER��1�����i�[
		tx2_wp++;
		if(tx2_wp >= SCI_BUFF_SIZE)
		{
			tx2_wp = 0;
		}
		tx2_length++;
    	ret = data;
	}
	else{
	}
	SCI2.SCR.BYTE |= 0x80;						// ���M�����݋���
	
	return (ret);
}

/*************************************************
                Hexadecimal
*************************************************/
void sci2_puthex(unsigned char data)
{
	sci2_putc((unsigned char)HexToAscii((unsigned char)((data&0xf0)>>4)));
	sci2_putc((unsigned char)HexToAscii((unsigned char)(data&0x0f) ));
}

/*************************************************
                  Decimal
*************************************************/
void sci2_putdec(unsigned long data)
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
			sci2_putc((unsigned char)(val[index] + 0x30));
		}
		else if(val[index] != 0)	// �ŏ��ɗ�łȂ��ʂ������܂ŏo�͂��Ȃ�
		{
			flag = 1;
			sci2_putc((unsigned char)(val[index] + 0x30));
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
void sci2_putdoublehex(unsigned short data)
{
	sci2_puthex((unsigned char)(data>>8));
	sci2_puthex((unsigned char)(data&0x00ff));
}

/*************************************************
                 FourHex
*************************************************/
void sci2_putfourhex(unsigned long data)
{
	sci2_putdoublehex((unsigned short)(data >> 16));
	sci2_putdoublehex((unsigned short)(data&0xffff));
}

/*************************************************
                 EightHex
*************************************************/
/*
void sci2_puteighthex(unsigned long data)
{
	sci2_putfourhex((unsigned int)(data >> 32));
	sci2_putfourhex((unsigned int)(data & 0xffffffff));
}
*/

/*************************************************
                  ���s
*************************************************/
void sci2_putcrlf(void)
{
	sci2_putc('\r');
	sci2_putc('\n');
}

/*************************************************
               ������i�[
*************************************************/
short sci2_puts(unsigned char *str)
{
	int i;
	for(i=0;i<100;i++){
		if(str[i]=='\0') break;
		sci2_putc(str[i]);
	}
	return 1;
}

/*************************************************
                1�����擾
************************************************/
short sci2_getc()
{
	short data;
	
	data = -1;
	
	SCI2.SCR.BYTE &= (unsigned char)(~0x40);		// ��M�����݋֎~
	if (0 < rx2_length) {
		data = rx2_buff[rx2_rp];
		rx2_rp ++;
		if(rx2_rp >= SCI_BUFF_SIZE)
		{
			rx2_rp = 0;
		}
		rx2_length--;
	}
	SCI2.SCR.BYTE |= 0x40;							// ��M�����݋���
	
	return(data);
}

/************************************************
              ��M�G���[���荞��
*************************************************/
void INT_ERI2(void)
{
	SCI2.SSR.BYTE &= ~0x38;		// �G���[�t���O�N���A
}

/*************************************************
               ��M���荞��
*************************************************/
void INT_RXI2(void)
{
	unsigned char data;
	
	data = SCI2.RDR;			// ��M
	SCI2.SSR.BYTE &= ~0x40;		// RDRF�N���A
	
	if(SCI_BUFF_SIZE > rx2_length){
		rx2_buff[rx2_wp] = data;		// RX_BUFFER��1�����i�[
		rx2_wp ++;
		if(rx2_wp >= SCI_BUFF_SIZE)
		{
			rx2_wp = 0;
		}
		rx2_length++;
	}
}

/*************************************************
                  ���M���荞��
*************************************************/
void INT_TXI2(void)
{
	unsigned char data;
	
	if (0 < tx2_length){
		data = tx2_buff[tx2_rp];
		
		tx2_rp++;
		if(tx2_rp >= SCI_BUFF_SIZE)
		{
			tx2_rp = 0;
		}
		tx2_length--;
		
		while(SCI2.SSR.BIT.TDRE == 0);
		SCI2.TDR = data;							// 1�o�C�g���M
		SCI2.SSR.BYTE &= (unsigned char)(~0x80);	// ���M���荞�݋֎~
	}
	else{
		SCI2.SCR.BYTE &= (unsigned char)(~0x80);	// ���M�����݋֎~
	}
}

/*************************************************
              ���M�I��������
*************************************************/
void INT_TEI2(void)
{
	SCI2.SCR.BYTE &= ~0x04;							// ���M�����݋֎~
}
