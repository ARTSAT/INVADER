/******************************************************************************
* ファイル名　：　iic2.c
* タイトル　　：　EEPROMとi2c通信を行うプログラム③
* 製作者　　　：　03-110317 木村元紀, 03-103002 尾崎直哉
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.28
* 言語　　　　：　C
*******************************************************************************
* ※備考※
* EEPROMとI2C通信をするためのプログラム
* P5B6-SCL,P5B7-SDAとして使用
*
******************************************************************************/
#include "iodefine.h"
#include "iic.h"

#define eepstop		IIC2_EEPSTOP
#define	eepread		IIC2_EEPREAD
#define eepwrite	IIC2_EEPWRITE
#define	eepreset	IIC2_EEPRESET
#define eepinit		IIC2_EEPINIT
#define	eepwait		IIC2_EEPWAIT
#define	writeb		IIC2_WRITEB
#define readb		IIC2_READB
#define eepstart	IIC2_EEPSTART
#define eepchange   IIC2_EEPCHANGE

#define		SCL_H	(P2DDR = P2_DDR = P2_DDR &= ~64)	// P2DDR.BIT.B6のみを0にする(入力設定) ~64=1011 1111
#define		SCL_L	(P2DDR = P2_DDR |=  64)				// P2DDR.BIT.B6のみを1にする(出力設定)  64=0100 0000
#define		SDA_H	(P2DDR = P2_DDR &= ~128)			// P2DDR.BIT.B7のみを0にする(入力設定) ~128=0111 1111
#define		SDA_L	(P2DDR = P2_DDR |=  128)			// P2DDR.BIT.B7のみを1にする(出力設定)  128=1000 0000
#define		WR_CYC	1250//80000						// ページ書き込み後の待ち時間5ms以上 25MHz*5msec = 125000
#define		CYCLE	0//160								// [SCL]パルス幅(約10us) 25MHz*10us = 250

unsigned char P2_DDR;

/******************************************************************************
		wait()関数
******************************************************************************/
void eepwait(volatile unsigned long count)
{
	while (count--);
}

/******************************************************************************
		1バイト書き込み 
******************************************************************************/
static void writeb(unsigned char dat)
{
	unsigned char i;
	
	eepwait(CYCLE);
	for (i=0; i<=7; i++){
		if (dat & 0x80) SDA_H; else SDA_L;	// datのBIT.B7が1ならSDA_H，0ならSDA_L
		dat <<= 1;							// datのBIT.B6がB7に入る．
 		eepwait(CYCLE);
		SCL_H;								// SCLがHigh
		eepwait(CYCLE);
		while ((P2DR.BYTE & 64) == 0);		// SCLがHighで無くなるまでループ
		SCL_L;								// SCL_Low
	}
	
	SDA_H;									// SDAがHigh
	eepwait(CYCLE);							// SCLがHigh，アクノリッジ信号
	SCL_H;
	eepwait(CYCLE);
	SDA_L;									// ストップコンディションとなるのを防止(開始条件)
	SCL_L;									// SCLがLow
}

/******************************************************************************
	1バイト読み出し
	f=1: ACKをEEPROMに返す，f=0: ACKをEEPROMに返さない(NO ACK:読み出し終了)
******************************************************************************/
static void readb(unsigned char *dat, unsigned char f)
{
	unsigned char i;
	
	SDA_H;									// High-Imp
	eepwait(CYCLE);
	for (i=0; i<=7; i++){
		SCL_H;
		*dat <<= 1;
		if (P2DR.BYTE & 128) *dat |= 0x01;	// SDAにHighが入力されているならdatに1を返す
		eepwait(CYCLE);
		while ((P2DR.BYTE & 64) == 0);		// SCLがLowになるまで待つ
		SCL_L;
	}
	if(f) SDA_H; else SDA_L;				// ACK 最終は"H"で終了
	eepwait(CYCLE);
	SCL_H;
	eepwait(CYCLE);
	SCL_L;
}

/******************************************************************************
		スタートコンディション ～ デバイスアドレス設定 (RW: 0=write, 1=read)
******************************************************************************/
static void eepstart(unsigned char rw, unsigned char s_ad, unsigned char block)
{
	SCL_H;
	SDA_H;
	eepwait(CYCLE);
	SDA_L;                                // Start
	eepwait(CYCLE);
	SCL_L;
	//writeb((rw & 1) | 0xA0);
	//writeb(0xA0 | ( ((s_ad<<1)&(0x0e)) | rw&1 ));
	writeb(0xA0 | (block<<3)&(0x08) | ( ((s_ad<<1)&(0x06)) | rw&1 ));        // 1010-(block adress:1bit)-(スレーブadress:2bit)-(R/W:1bit)
	/*
	debug_puthex(0xA0 | (block<<3)&(0x08) | ( ((s_ad<<1)&(0x06)) | rw&1));   // for debug
	debug_puts("\r\n");
	*/
}

/******************************************************************************
		ストップコンディション
******************************************************************************/
void eepstop(void)
{
	SCL_L;
	SDA_L;
	eepwait(CYCLE);
	SCL_H;
	eepwait(CYCLE);
	SDA_H;
	eepwait(CYCLE);
}

/******************************************************************************
		EEP-ROM 読み出し
		Entry	uchar page : EEP-ROM ページ番号(0～3FFFh)
				uchar *buf : 読み込みデータバッファ(8byte)へのポインタ
******************************************************************************/
void eepread(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte)
{
	unsigned char i;
	unsigned char block=0;
	
	P2DR.BYTE &= ~0xc0;                  // ~0xc0 = 00111111 : データレジスタは出力時に絶対条件として"L"
	
	if(page > 0x1FF)	block = 1;
	eepstart(0,s_ad,block);
	
	unsigned char bit = eepchange(byte);
	
	/* アドレスの指定 */
	writeb((unsigned char)(page >> (8-bit)));   // 下位へ移動とA8までに規制する
	writeb((unsigned char)(page << bit));   // 1ページ128バイトの為128倍する
	
	eepwait(WR_CYC);
	
	/* データの送信 */
	eepstart(1,s_ad,block);
	for (i=0; i<(byte-1); i++){
		readb(buf++,0);	
	}
	readb(buf,1);                         // 最後はNO ACKで終了
	eepstop();
}

/******************************************************************************
	EEPROM 書き込み
	Entry	unsigned char page : EEP-ROM ページ番号(0～3FFFh)
			unsigned char *buf : 書き込みデータバッファ(8bytes)へのポインタ
******************************************************************************/
void eepwrite(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte)
{
	unsigned char i;
	unsigned char block=0;
	
	P2DR.BYTE &= ~0xc0;                  // ~0xc0 = 00111111：データレジスタは出力時に絶対条件として"L"
	
	if(page > 0x1FF)	block = 1;
	eepstart(0,s_ad,block);
	
	unsigned char bit = eepchange(byte);
	
	/* アドレスの指定 */
	writeb((unsigned char)(page >> (8-bit)));   // 下位へ移動とA8までに規制する
	writeb((unsigned char)(page << bit));   // 1ページ128バイトの為，128倍する
	
	/* データの送信 */
	for (i=0; i<byte; i++){
		writeb(*buf++);
	}
	eepstop();
	eepwait(WR_CYC);
}

/******************************************************************************
		EEPROM ソフトリセット
******************************************************************************/
void eepreset(void)
{
	unsigned char i;
	
	for (i=0; i<9; i++){
		SDA_L;
		eepwait(CYCLE);
		SCL_L;
		eepwait(CYCLE);
		SDA_H;
		eepwait(CYCLE);
		SCL_H;
		eepwait(CYCLE);
	}
}

/*******************************************************************************
		EEPROM用入出力初期化
*******************************************************************************/
unsigned char eepinit(void)
{
	P2PCR.BYTE |= 0xC0;				// プルアップ抵抗追加
	return 1;
}

/*******************************************************************************
		EEPROM用バイト数変換関数
		Entry unsigned long int:32byte→5
		　　　　　　　　　　　　128byte→7に変換
*******************************************************************************/
unsigned char eepchange(unsigned char byte) {
	if(byte == 32) {
		byte = 5;
	}
	else if(byte == 128) {
		byte = 7;
	}
	return byte;
}