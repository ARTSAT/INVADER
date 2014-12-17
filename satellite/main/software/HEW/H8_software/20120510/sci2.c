/*************************************************************************************
* ファイル名　：　sci2.c
* タイトル　　：　INVADERシリアル通信(CH2)プログラム
* 製作者　　　：　03-110351 三好賢聖　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.5.14
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* PRISM(old_version_8.0~)のプログラムを参考にコーディング
* H8/3048FからH8/3069Fへの移行に当たり、sci0.c, sci1.cに加えて追加
*
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "sci.h"

#define SCI_BUFF_SIZE	256
#define	PCLOCK		(25000000)			// 外部クロック(Hz)

static volatile unsigned char	rx2_buff[SCI_BUFF_SIZE];
static volatile unsigned char	tx2_buff[SCI_BUFF_SIZE];
static volatile unsigned short	rx2_rp;
static volatile unsigned short	rx2_wp;
static volatile unsigned short	rx2_length;
static volatile unsigned short	tx2_rp;
static volatile unsigned short	tx2_wp;
static volatile unsigned short	tx2_length;

/*************************************************
		シリアルポート初期化
**************************************************/
unsigned char sci2_init(
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
	
	SCI2.SMR.BYTE = smr;
	SCI2.BRR = brr;
	for(i = 0; i < 3000; i++);	// 少なくとも1ビット期間待つ
	
	SCI2.SCR.BYTE = 0x70;		// 送受信、受信割り込みのみ許可
	
	dummy = SCI2.SSR.BYTE;		// dummy read
	SCI2.SSR.BYTE = 0x84;		// 送信割り込み関係の設定 TDRE = 1,TEND = 1
	
	return 1;
}

/*************************************************
                  1文字出力
*************************************************/
short sci2_putc(unsigned char data)
{
	short ret = -1;
	
	while(SCI2.SSR.BIT.TDRE == 0);				// TDRが空くまで待つ
	SCI2.SCR.BYTE &= (unsigned char)(~0x80);	// 送信割込み禁止
	
	if(SCI_BUFF_SIZE > tx2_length)
	{
		tx2_buff[tx2_wp] = data;				// TX_BUFFERに1文字格納
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
	SCI2.SCR.BYTE |= 0x80;						// 送信割込み許可
	
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
			break_flag = 1;			// 1の位まできたことを検出
		}
		div /= 10;
		
		if((flag)||(break_flag))	// すでに0でない位に出会ったか，あるいは最後まで0であったら
		{
			sci2_putc((unsigned char)(val[index] + 0x30));
		}
		else if(val[index] != 0)	// 最初に零でない位が現れるまで出力しない
		{
			flag = 1;
			sci2_putc((unsigned char)(val[index] + 0x30));
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
                  改行
*************************************************/
void sci2_putcrlf(void)
{
	sci2_putc('\r');
	sci2_putc('\n');
}

/*************************************************
               文字列格納
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
                1文字取得
************************************************/
short sci2_getc()
{
	short data;
	
	data = -1;
	
	SCI2.SCR.BYTE &= (unsigned char)(~0x40);		// 受信割込み禁止
	if (0 < rx2_length) {
		data = rx2_buff[rx2_rp];
		rx2_rp ++;
		if(rx2_rp >= SCI_BUFF_SIZE)
		{
			rx2_rp = 0;
		}
		rx2_length--;
	}
	SCI2.SCR.BYTE |= 0x40;							// 受信割込み許可
	
	return(data);
}

/************************************************
              受信エラー割り込み
*************************************************/
void INT_ERI2(void)
{
	SCI2.SSR.BYTE &= ~0x38;		// エラーフラグクリア
}

/*************************************************
               受信割り込み
*************************************************/
void INT_RXI2(void)
{
	unsigned char data;
	
	data = SCI2.RDR;			// 受信
	SCI2.SSR.BYTE &= ~0x40;		// RDRFクリア
	
	if(SCI_BUFF_SIZE > rx2_length){
		rx2_buff[rx2_wp] = data;		// RX_BUFFERに1文字格納
		rx2_wp ++;
		if(rx2_wp >= SCI_BUFF_SIZE)
		{
			rx2_wp = 0;
		}
		rx2_length++;
	}
}

/*************************************************
                  送信割り込み
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
		SCI2.TDR = data;							// 1バイト送信
		SCI2.SSR.BYTE &= (unsigned char)(~0x80);	// 送信割り込み禁止
	}
	else{
		SCI2.SCR.BYTE &= (unsigned char)(~0x80);	// 送信割込み禁止
	}
}

/*************************************************
              送信終了割込み
*************************************************/
void INT_TEI2(void)
{
	SCI2.SCR.BYTE &= ~0x04;							// 送信割込み禁止
}
