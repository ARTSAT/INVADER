/*************************************************************************************
* ファイル名　：　adconv.c
* タイトル　　：　INVADER AD変換プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.31
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "adconv.h"
#include "sci.h"
#include "global.h"
#include "cdh_pindefine.h"
#include "inthandler.h"
#include "mission.h"


#define	MPX00   (AD.ADDRA & 0xff00) >> 8	// マルチプレクサ1-DA
#define	MPX01   (AD.ADDRB & 0xff00) >> 8	// マルチプレクサ1-DB
#define	MPX10   (AD.ADDRC & 0xff00) >> 8	// マルチプレクサ2-DA
#define	MPX11   (AD.ADDRD & 0xff00) >> 8	// PIC電流センサ

/*************************************************
		AD変換関数初期化関数
*************************************************/
unsigned char adconv_init(void)
{
	unsigned char i;
	
	// AD変換: AN0~AN3,SCANモード,266ステート
	AD.ADCSR.BYTE = 0x13;
	
	// AD変換タイマ割り込みフラグ
	ClearFlags(flag_ad);
	ClearFlags(flag_overa);
	ClearFlags(flag_dispAD);
	
	
	return 1;
}

/*************************************************
		AMI302(磁気センサ)スイッチング関数
*************************************************/
void ami_switch(unsigned char ch)
{
	if(ch == 3){
		// x軸変換
		AMICH1 = 1;
		AMICH2 = 1;
	}
	else if(ch == 4){
		// y軸変換
		AMICH1 = 0;
		AMICH2 = 0; 
	}
	else if(ch == 5){
		// z軸変換
		AMICH1 = 1;
		AMICH2 = 0;
	}
	else;
	/*
	debug_puts("AMI:");
	debug_putdec(AMICH1);
	debug_putdec(AMICH2);
	debug_puts("\n");
	*/
}

/*************************************************
		マルチプレクサ・スイッチング関数
*************************************************/
void mpx_switch(unsigned char ch)
{
	MPX0_A0 = ch & 0x01;
	MPX0_A1 = (ch & 0x02) >> 1;
	MPX0_A2 = (ch & 0x04) >> 2;
	
	MPX1_A0 = ch & 0x01;
	MPX1_A1 = (ch & 0x02) >> 1;
	MPX1_A2 = (ch & 0x04) >> 2;
	
	/*
	debug_puts("CH:");
	debug_putdec(ch);
	debug_puts(" ,PADR:");
	debug_putdec(PADR.BIT.B0);
	debug_putdec(PADR.BIT.B1);
	debug_putdec(PADR.BIT.B2);
	debug_putdec(PADR.BIT.B4);
	debug_putdec(PADR.BIT.B5);
	debug_putdec(PADR.BIT.B6);
	debug_puts("\n");
	*/
}

/*************************************************
		AD変換割り込みセット関数
*************************************************/
/*
unsigned char SetADConvert(void)
{
	
	AD.ADCSR.BIT.ADST = 1;		// AD変換開始
	AD.ADCSR.BIT.ADIE = 1;		// AD変換割り込み許可
	
	return 1;
}
*/

/*************************************************
		AD変換終了割り込み関数
*************************************************/
void get_sensor(void)
{
	static unsigned char mpx_ch;		// マルチプレクサCH:0~7
										// 磁気センサ(AMI302)CH0~2
	AD.ADCSR.BIT.ADST = 1;				// AD変換開始
	while(AD.ADCSR.BIT.ADF == 0);		// AD変換終了待ち
	
	mpx[0][mpx_ch] = MPX00;
	mpx[1][mpx_ch] = MPX01;
	mpx[2][mpx_ch] = MPX10;
	mpx[3][mpx_ch] = MPX11;
	//MPX11;	// dummy read
	
	//00:A
	//MPX01 
	
	switch(mpx_ch)
	{
		case 0:
			status.temperature[4] =	MPX00;	//-X
			status.temperature[3] =	MPX01;	//+X
			status.gyro[0] =		MPX10;
			break;
		case 1:
			status.temperature[15] =MPX00;	//TX
			status.temperature[5] =	MPX01;	//+Y1
			status.gyro[1] =		MPX10;
			break;
		case 2:
			status.temperature[14] =MPX00;	//Mission
			status.temperature[7] =	MPX01;	//-Y1
			status.gyro[2] =		MPX10;
			break;
		case 3:
			status.temperature[12] =MPX00;	//-Z2
			status.temperature[9] =	MPX01;	//+Z1（熱真空試験においてのみ+X）
			status.magnet[0] =		MPX10;
			break;
		case 4:
			status.temperature[16] =MPX00;	//RX
			status.temperature[17] =MPX01;	//Main
			status.magnet[1] =		MPX10;
			break;
		case 5:
			status.temperature[8] = MPX00;	//-Y2
			status.temperature[11] =MPX01;	//-Z1
			status.magnet[2] =		MPX10;
			break;
		case 6:
			status.temperature[10] = MPX00;	//+Z2
			break;
		case 7:
			status.temperature[6] = MPX00;	//+Y2
			break;
		default:
			break;
		
	}
	
	mpx_ch = mpx_ch + 1;
	if(mpx_ch > 7){
		mpx_ch = 0;
	}else;
	
	mpx_switch(mpx_ch);			// 次回の変換に備え、マルチプレクサのスイッチング
								// ADG707:max transition time = 60nsec
	
	ami_switch(mpx_ch);			// 磁気センサのスイッチング
								// AMI302:max transition time = 1ms
	
	AD.ADCSR.BIT.ADF = 0;		// フラグクリア
	AD.ADCSR.BIT.ADST = 0;		// AD変換停止
}

/*************************************************
		渦電流検知関数
*************************************************/
void check_overAmpare(void)
{
	AD.ADCSR.BIT.ADST = 1;				// AD変換開始
	while(AD.ADCSR.BIT.ADF == 0);		// AD変換終了待ち
	
	mpx[3][0] = MPX11;
//	status.ampare[11] = MPX11;
	
	// dummy read
	MPX00;
	MPX01;
	MPX10;
	
	debug_puts("<");
//	debug_putdec(status.ampare[0]);
	debug_putdec(mpx[3][0]);
	debug_puts(">");

	
//	if(status.ampare[0] >= 40) {
	if(mpx[3][0] >= 200) {
		num_overAmpare++;
		debug_puts("\n !!!!!!! OverAmpare ");
		debug_putdec(num_overAmpare);
		debug_puts("th !!!!!!! \n");
	}
	else{
		num_overAmpare=0;
	}
	
	if(num_overAmpare>3){
		mission_timeout=0;

		debug_puts("\n !!!!!!!-- Power OFF --!!!!!!!\n");
		num_overAmpare=0;
		
		PIC_KILL1 = 0;
		PIC_KILL2 = 1;
		
		// タイムアウト処理用タイマ(ITU2)カウント開始
		ITU2.GRB = 31250-1;			// 周期10ms
//		ITU2.TCR.BIT.CCLR = 2;		// GRBのコンペアマッチでカウントクリア
		ITU.TISRB.BIT.IMFB2 = 0;	// 割り込みステータスフラグクリア
		ITU.TISRB.BIT.IMIEB2 = 1;	// タイマ割り込み許可
		ITU.TSTR.BIT.STR2 = 1;		// カウント開始
		ITU.TISRA.BIT.IMIEA2 = 0;	// タイマ割り込み禁止
	} 
	
	
	AD.ADCSR.BIT.ADF = 0;		// フラグクリア
	AD.ADCSR.BIT.ADST = 0;		// AD変換停止
}
/*************************************************
		電源系ON/OFF
*************************************************/
void PowerReset(void){
	mission_timeout=0;
	debug_puts("\n !!!!!!!-- Power OFF --!!!!!!!\n");
		
	PIC_KILL1 = 0;
	PIC_KILL2 = 1;
		
	// タイムアウト処理用タイマ(ITU2)カウント開始
	ITU2.GRB = 31250-1;			// 周期10ms
//	ITU2.TCR.BIT.CCLR = 2;		// GRBのコンペアマッチでカウントクリア
	ITU.TISRB.BIT.IMFB2 = 0;	// 割り込みステータスフラグクリア
	ITU.TISRB.BIT.IMIEB2 = 1;	// タイマ割り込み許可
	ITU.TSTR.BIT.STR2 = 1;		// カウント開始 
	ITU.TISRA.BIT.IMIEA2 = 0;	// タイマ割り込み禁止
}
/*************************************************
		電源系ON
*************************************************/
void RequestPowerReboot(void){
	debug_puts("\n !!!!!!!-- Power On (5 minutes')--!!!!!!!\n");
		
	PIC_KILL1 = 1;
	PIC_KILL2 = 0;

}
/*************************************************
		電源系OFF
*************************************************/
void RequestPowerShutdown(void){
	debug_puts("\n !!!!!!!-- Power Off --!!!!!!!\n");
		
	PIC_KILL1 = 0;
	PIC_KILL2 = 1;

}

#define TIMEOUT	1000	// Arduino強制シャットダウン：1000*10msec = 10sec
/*************************************************
		PIC再起動
*************************************************/
void INT_IMIB2(void)
{
	static volatile unsigned long cnt=0;
	
	ITU.TISRB.BIT.IMFB2 = 0;		// 割り込みステータスフラグクリア
	cnt++;
	
	if(cnt == TIMEOUT){
		debug_puts("\n !!!!!!!-- Power ON --!!!!!!!\n");
		PIC_KILL1 = 1;
		PIC_KILL2 = 0;	
		
		cnt = 0;
//		ITU2.GRB = 0;			// 周期10ms
		ITU.TISRA.BIT.IMIEA2 = 0;	// タイマ割り込み禁止
		ITU.TISRB.BIT.IMIEB2 = 0;	// タイマ割り込み禁止
		ITU.TSTR.BIT.STR2 = 0;		// カウント停止
		ITU2.TCNT = 0;				// カウンタクリア
//		ITU2.TCR.BIT.CCLR = 1;		// GRAに戻す
	}
}



/*************************************************
		スペース挿入関数
*************************************************/
void insertSpace(unsigned char dat)
{
	if(dat >= 100)	debug_puts(" ");
	else if(dat >= 10)	debug_puts("  ");
	else debug_puts("   ");
	
	return;
}

/*************************************************
		AD変換結果表示関数
*************************************************/
unsigned char disp_AD(void)
{
	unsigned char i;
	
	debug_puts("\n");
	
	for(i=0;i<8;i++)
	{
		insertSpace(mpx[0][i]);
		debug_putdec(mpx[0][i]);
		//debug_puts(" ");
	}
	
	debug_puts("\t");
	
	for(i=0;i<8;i++)
	{
		insertSpace(mpx[1][i]);
		debug_putdec(mpx[1][i]);
		//debug_puts(" ");
	}
	
	debug_puts("\t");
	for(i=0;i<8;i++)
	{
		insertSpace(mpx[2][i]);
		debug_putdec(mpx[2][i]);
		//debug_puts(" ");
	}
	
	debug_puts("\t");
	for(i=0;i<6;i++)
	{
		insertSpace(mpx[3][i]);
		debug_putdec(mpx[3][i]);
		//debug_puts(" ");
	}
	debug_puts("\n");
	
	return 1;
}