/*************************************************************************************
* ファイル名　：　init.c
* タイトル　　：　INVADER初期化プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.5.10
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "iodefine.h"
#include "cdh_pindefine.h"
#include "sci.h"
#include "itu1.h"
#include "adconv.h"
#include "teleme.h"
#include "pwr.h"
#include "mission.h"
#include "message.h"
#include "iic.h"
#include "global.h"
#include "fm_tx.h"
#include "fm_pindef.h"

/*************************************************
			内部関数初期化関数
--------------------------------------------------
* H8の内部関数の初期化を行う
*
*************************************************/
unsigned char Init_MyFunction(void)
{
	unsigned char flag = 0;		// 内部関数初期化フラグ
	
	flag += sci0_init(19200,8,0,1);		// シリアルポート(CH0)1初期化 br=19200,bit=8,parity=none,stopbit=1
	flag += sci1_init(19200,8,0,1);		// シリアルポート(CH1)1初期化 br=19200,bit=8,parity=none,stopbit=1
	flag += sci2_init(4800,8,0,1);
	
	flag += InitMessage();
		
	flag += itu0_init();
	flag += itu1_init();
	flag += itu2_init();
	
	flag += adconv_init();
	
	flag += InitStatus();
	flag += InitTelemetry();
	flag += InitPowerTelemetry();
	flag += InitMissionTelemetry();
	
	flag += InitRoutine();	// 各ルーチンの許可/不許可を初期化
	
	flag += init_fm();
	
	return flag;
}

/*************************************************
			内部レジスタ初期化関数
--------------------------------------------------
* H8のCPUバス機能及びピンの設定を行う
* 
*************************************************/
unsigned char Init_Bus(void)
{
	// 入出力設定
	APLS_OUT;
	MPX_OUT;
	AMI_OUT;
	SROM_OUT;
	
	// データレジスタ初期化
	APLS = 0;
	
	MPX0_A0 = 0;
	MPX0_A1 = 0;
	MPX0_A2 = 0;
	MPX1_A0 = 0;
	MPX1_A1 = 0;
	MPX1_A2	= 0;
	
	SROM_CH = 0;
	SROM_EN = 0;
	
	AMICH1 = 0;
	AMICH2 = 0;
	
	FX_RDYN	= 0;
	FX_CLK	= 0;
	FX_TXD	= 0;
	FX_M1	= 0;

	FMSTB	= 0;
	FMDAT	= 0;
	FMCLK	= 0;
	FMPTT	= 0;
	
	FMCWSW 	= 0;//1
//	FMDTSW 	= 1;
	FMDTSW 	= 0;
	
	PIC_KILL1 = 1;
	PIC_KILL2 = 0;
	
	return 1;					// 内部レジスタ初期化フラグ ~0x01
}

/*************************************************
			デバイス初期化関数
--------------------------------------------------
* H8に接続されている各種素子の設定を行う
*
*************************************************/
unsigned char Init_Peripherals(void)
{
	unsigned char flag = 0;		// デバイス初期化フラグ ~0x01
	
	flag += IIC0_EEPINIT();			// EEPROM初期化
	flag += IIC1_EEPINIT();
	flag += IIC2_EEPINIT();
	
	return flag;
}
