/*************************************************************************************
* ファイル名　：　sci0.c
* タイトル　　：　INVADERシリアル通信(CH0)プログラム
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
#include "inthandler.h"
#include "sci.h"

#define SCI_BUFF_SIZE	256
#define	PCLOCK		(25000000)			// 外部クロック(Hz)

static volatile unsigned char	rx0_buff[SCI_BUFF_SIZE];
static volatile unsigned char	tx0_buff[SCI_BUFF_SIZE];
static volatile unsigned short	rx0_rp;
static volatile unsigned short	rx0_wp;
static volatile unsigned short	rx0_length;
static volatile unsigned short	tx0_rp;
static volatile unsigned short	tx0_wp;
static volatile unsigned short	tx0_length;

/*************************************************
		シリアルポート初期化
**************************************************/
unsigned char sci0_init(
	unsigned short  bps,				// 4800~
	unsigned char   bit,				// 8 or 7
	unsigned char   parity,				// 0:NON 1:EVN 2:ODD
	unsigned char   stopBit)			// 1 or 2
{
	unsigned char smr;					// シリアルモードレジスタ
	unsigned char brr;					// ビットレートレジスタ
	unsigned char dummy;				// SSR用ダミー・リード
	
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
 	
	if ( 7 == bit ){			// ビット数=7
		smr |= 0x40;
	}
	if ( 1 == parity ){			// 奇数パリティ
		smr |= 0x20;
	}
	else if ( 2 == parity ){	// 偶数パリティ
		smr |= 0x30;
	}
	if ( 2 == stopBit ){		// ストップビット=2
		smr |= 0x08;
	}
	
	// BRR
	brr = (unsigned char)(((PCLOCK / 32) / bps) - 1);
	
	SCI0.SMR.BYTE = smr;
	SCI0.BRR = brr;
	for(i = 0; i < 3000; i++);	// 少なくとも1ビット期間待つ
	
	SCI0.SCR.BYTE = 0x70;		// 送受信、受信割り込みのみ許可
	
	dummy = SCI0.SSR.BYTE;		// dummy read
	SCI0.SSR.BYTE = 0x84;		// 送信割り込み関係の設定 TDRE = 1,TEND = 1
	
	return 1;
}

/*************************************************
                  1文字出力
*************************************************/
short sci0_putc(unsigned char data)
{
	short ret = -1;
	
	while(SCI0.SSR.BIT.TDRE == 0);				// TDRが空くまで待つ
	SCI0.SCR.BYTE &= (unsigned char)(~0x80);	// 送信割込み禁止
	
	if(SCI_BUFF_SIZE > tx0_length)
	{
		tx0_buff[tx0_wp] = data;				// TX_BUFFERに1文字格納
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
	SCI0.SCR.BYTE |= 0x80;						// 送信割込み許可
	
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
			break_flag = 1;			// 1の位まできたことを検出
		}
		div /= 10;
		
		if((flag)||(break_flag))	// すでに0でない位に出会ったか，あるいは最後まで0であったら
		{
			sci0_putc((unsigned char)(val[index] + 0x30));
		}
		else if(val[index] != 0)	// 最初に零でない位が現れるまで出力しない
		{
			flag = 1;
			sci0_putc((unsigned char)(val[index] + 0x30));
		}
		
		index++;
		if(break_flag) 
		{
			break;					// 1の位まできたのでやめる
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
                  改行
*************************************************/
void sci0_putcrlf(void)
{
	sci0_putc('\r');
	sci0_putc('\n');
}

/*************************************************
               文字列格納
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
                1文字取得
************************************************/
short sci0_getc()
{
	short data;
	
	data = -1;
	
	SCI0.SCR.BYTE &= (unsigned char)(~0x40);		// 受信割込み禁止
	if (0 < rx0_length) {
		data = rx0_buff[rx0_rp];
		rx0_rp ++;
		if(rx0_rp >= SCI_BUFF_SIZE)
		{
			rx0_rp = 0;
		}
		rx0_length--;
	}
	SCI0.SCR.BYTE |= 0x40;							// 受信割込み許可
	
	return(data);
}

/************************************************
              受信エラー割り込み
*************************************************/
void INT_ERI0(void)
{
	SCI0.SSR.BYTE &= ~0x38;		// エラーフラグクリア
}

/*************************************************
               受信割り込み
*************************************************/
void INT_RXI0(void)
{
	unsigned char data;
	
	data = SCI0.RDR;			// 受信
	SCI0.SSR.BYTE &= ~0x40;		// RDRFクリア
	
	if(SCI_BUFF_SIZE > rx0_length){
		rx0_buff[rx0_wp] = data;		// RX_BUFFERに1文字格納
		rx0_wp ++;
		if(rx0_wp >= SCI_BUFF_SIZE)
		{
			rx0_wp = 0;
		}
		rx0_length++;
	}
}

/*************************************************
                  送信割り込み
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
		SCI0.TDR = data;							// 1バイト送信
		SCI0.SSR.BYTE &= (unsigned char)(~0x80);	// 送信割り込み禁止
	}
	else{
		SCI0.SCR.BYTE &= (unsigned char)(~0x80);	// 送信割込み禁止
	}
}

/*************************************************
              送信終了割込み
*************************************************/
void INT_TEI0(void)
{
	SCI0.SCR.BYTE &= ~0x04;							// 送信割込み禁止
}
