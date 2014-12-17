/*************************************************************************************
* ファイル名　：　sci_util.c
* タイトル　　：　INVADERシリアル通信プログラム
* 製作者　　　：　03-110351 三好賢聖　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.29
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* PRISM(old_version_8.0~)のプログラムを参考にコーディング
*
*************************************************************************************/
#include "iodefine.h"
#include "sci.h"

/*************************************************
		初期化関数
*************************************************/
unsigned char debug_sci_init(void)
{
	unsigned char flag = 1;
	
	sci1_init(19200,8,0,1);		//19200bps,8bit,non parity, 1 stop bit
	
	return flag;
}

/*************************************************
		1文字送信
*************************************************/
void debug_putc(unsigned char data)
{
	sci1_putc(data);
}

/*************************************************
		文字列送信
*************************************************/
void debug_puts(unsigned char *str)
{
	sci1_puts((unsigned char*)str);
}

/************************************************
		1文字取得
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
		return (unsigned char)('a'+hex-0x0a); // 10～15→'a'～'f'
	}
}

