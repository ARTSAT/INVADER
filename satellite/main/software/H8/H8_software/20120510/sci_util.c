/*************************************************************************************
* �t�@�C�����@�F�@sci_util.c
* �^�C�g���@�@�F�@INVADER�V���A���ʐM�v���O����
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
#include "sci.h"

/*************************************************
		�������֐�
*************************************************/
unsigned char debug_sci_init(void)
{
	unsigned char flag = 1;
	
	sci1_init(19200,8,0,1);		//19200bps,8bit,non parity, 1 stop bit
	
	return flag;
}

/*************************************************
		1�������M
*************************************************/
void debug_putc(unsigned char data)
{
	sci1_putc(data);
}

/*************************************************
		�����񑗐M
*************************************************/
void debug_puts(unsigned char *str)
{
	sci1_puts((unsigned char*)str);
}

/************************************************
		1�����擾
*************************************************/
short debug_getc(void)
{
	return sci1_getc();
}

/*************************************************
		Hexadecimal
*************************************************/
void debug_puthex(unsigned long data)
{
	if(data <= 0xff)
	{
		sci1_puthex((unsigned char)data);
	}
	else if(data <= 0xffff)
	{
		sci1_putdoublehex((unsigned short)data);
	}
	else
	{
		sci1_putfourhex((unsigned long)data);
	}
}

/*************************************************
		Decimal
**************************************************/
void debug_putdec(unsigned long data)
{
	sci1_putdec(data);
}

/*************************************************
		Hex => Ascii
*************************************************/
unsigned char HexToAscii(unsigned char hex)
{
	if(hex <0x0a)
	{
		return (unsigned char)(hex+0x30);
	}
	else
	{
		return (unsigned char)('a'+hex-0x0a); // 10�`15��'a'�`'f'
	}
}

