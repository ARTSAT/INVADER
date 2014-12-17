/*************************************************************************************
* ファイル名　：　itu0.c
* タイトル　　：　INVADERタイマ(CH0)プログラム
* 製作者　　　：　03-110351 三好賢聖　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.5.18
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* H8内部タイマ(ITU-CH1)
* intimia1(): 10msec周期(100Hz)で割り込み処理を発生。
* H8外部クロックとしては25MHzを前提。
* 
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "itu1.h"
#include "cdh_pindefine.h"
#include "sci.h"
#include "teleme.h"
#include "adconv.h"
#include "message.h"
#include "global.h"
#include "pwr.h"
#include "mission.h"
#include "debug.h"
#include "Rx_protcol.h"
#include "fm_tx.h"

//#define DIV_MPX   		  	5                   // 20msec毎に全てのマルチプレクサでチャンネル切り替え
#define DIV_MPX   		  	10                   // 100msec毎に全てのマルチプレクサでチャンネル切り替え
//#define DIV_AD				50					// AD変換は500msec周期で行う
#define DIV_AD				100					// AD変換は1000msec周期で行う
#define DIV_OVERAMPARE		100					// 1.0sec毎に渦電流検知
#define DIV_PTELEME     	100					// 1sec周期でパワーCPUに電源情報を送信要求
#define DIV_MTELEME			100					// 1sec周期でミッションCPUにテレメトリ送信
#define DIV_ROUGHTELEME     36000	//36000				// 6分周期で1セットの粗テレメトリ保存
#define DIV_DETAILEDTELEME  200	//200      		// 2sec周期で1セットの詳細テレメトリ保存
#define DIV_APITELEME		60000	//60000				// 10分周期でAPI配信のテレメトリ保存
#define DIV_ITELEME			1000				// 10sec周期で初期化情報のテレメトリ保存
#define DIV_MESSAGE 		10					// 0.1sec周期でコマンド確認
#define DIV_EXE		 		10					// 0.1sec周期でコマンド実行
#define DIV_RX				10					// 0.1sec周期でRXからのコマンド確認
#define DIV_TX				100					// 1.0sec周期でTXとかかわる変数に関する確認
#define DIV_CPR				50					// 0.5sec周期でパワーから返事が返って来ているかの確認
#define DIV_OUTPUTTELEME	30000				// 5分周期でテレメトリを出力
#define	DIV_PIC				30000				// 5分周期でPICをONする。

#define ALV_PERIOD	200					// 生存信号周期（新型）（2sec）
#define OVERFLOW	1000				// my_wdtがこの値以上になったら自己リセット要求が発生

//static volatile unsigned long itu1_cnt;         // 10msecの割り込み毎にインクリメント

/*************************************************
            ITU1初期化関数
--------------------------------------------------
* H8外部クロック25MHz
* GRAとのコンペアマッチでカウントクリア
*
*************************************************/
unsigned char itu1_init(void)
{
	ITU.TSTR.BIT.STR1 = 0;                // カウント停止
	ITU1.TCNT = 0;                        // カウントクリア
	
	ITU1.TCR.BIT.CCLR = 1;                // GRAのコンペアマッチでカウントクリア
	ITU1.TCR.BIT.TPSC = 3;                // タイマプリスケーラ 25MHz/8=3.125MHz T=0.32[usec]
	ITU1.GRA = 31250-1;                   // 周期10ms
	
	itu1_cnt = 1;
	my_wdt = 0;
	
	ITU.TISRA.BIT.IMFA1 = 0;                // 割り込みステータスフラグクリア
	ITU.TISRA.BIT.IMIEA1 = 1;               // タイマ割り込み許可
	ITU.TSTR.BIT.STR1 = 1;                  // カウント開始
	
	return 1;
}

/*************************************************
            ITU0同期割り込み関数
--------------------------------------------------
* H8外部クロック25MHz
* 100Hzで割り込み発生
*
**************************************************/
void INT_IMIA1(void)
{
	unsigned char i;
	
	//生存信号
	if(itu1_cnt % ALV_PERIOD == 0)	SetFlags(flag_alive);
	
	// AD変換,マルチプレクサスイッチング
//	if((itu1_cnt % DIV_AD) % DIV_MPX == 0 && (itu1_cnt % DIV_AD) <=35){	// DIV_AD/10 * 7

	if((itu1_cnt % DIV_AD) % DIV_MPX == 0 && (itu1_cnt % DIV_AD) <=70){	// DIV_AD/10 * 7
		SetFlags(flag_ad);
	}
	
	// AD変換結果表示
	if(itu1_cnt % DIV_AD == 0)	SetFlags(flag_dispAD);
	
	//コマンド実行
	if(itu1_cnt % DIV_EXE == 0)	SetFlags(flag_exe);
	
	//渦電流検知テレメトリ
	if(itu1_cnt % DIV_OVERAMPARE == 0)	SetFlags(flag_overa);
	
	// 粗テレメトリ
	if(itu1_cnt % DIV_ROUGHTELEME == 0)	SetFlags(flag_rteleme);
	
	// 詳細テレメトリ
	if(itu1_cnt % DIV_DETAILEDTELEME == 0)	SetFlags(flag_dteleme);
	
	// APIテレメトリ
	if(itu1_cnt % DIV_APITELEME == 0)	SetFlags(flag_ateleme);
	
	// 初期化情報テレメトリ
	if(itu1_cnt % DIV_ITELEME == 0)	SetFlags(flag_iteleme);
	
	// パワーテレメトリ
	if(itu1_cnt % DIV_PTELEME == 0)	SetFlags(flag_pwrt);
	
	// ミッションテレメトリ
	if(itu1_cnt % DIV_MTELEME == 0) SetFlags(flag_misst);
	
	// シリアル通信メッセージチェック
	if(itu1_cnt % DIV_MESSAGE == 0)	SetFlags(flag_scirx);
	
	// RXチェック
	if(itu1_cnt % DIV_RX == 0)	SetFlags(flag_rx);
	
	// TXチェック
	if(itu1_cnt % DIV_TX == 0)	SetFlags(flag_tx);
	
	//パワーから返事が返って来ているかの確認
	if(itu1_cnt % DIV_CPR == 0)  SetFlags(flag_cpr);
	
	// 試験用テレメトリ出力
	if(itu1_cnt % DIV_OUTPUTTELEME == 0) SetFlags(flag_oteleme);
	
	//ミッションCPU起動(タイマ予約型、タイマ予約&作動時間指定型)
	if((smr_time>0)&&(itu1_cnt > smr_time)){
		SetFlags(flag_smr);
	}
	//ミッションCPUシャットダウン(タイマ予約&作動時間指定型)
	if((sms_time>0)&&(itu1_cnt > sms_time)){
		SetFlags(flag_sms);
	}
	//PICオン要求
	if(itu1_cnt % DIV_PIC == 0) SetFlags(flag_pic);
	
	/*デバッグ用*/
	mission_cnt=itu1_cnt;
	
	itu1_cnt++;
	
	if(itu1_cnt >= 0xFFFFFFFE)	// 0.001*4294967296(sec) = 500(day)でクリア
	{
		itu1_cnt = 0;
	}
	
	if(CheckFlags(ena_mywdt)){
		my_wdt++;		// main loopでクリア
		if(my_wdt >= OVERFLOW)	RequestMainShutdown();	// 自己リセット要求
	}
	
	status.obctime[3] =  itu1_cnt			& 0x00ff;
	status.obctime[2] = (itu1_cnt >>  8)	& 0x00ff;
	status.obctime[1] = (itu1_cnt >> 16)	& 0x00ff;
	status.obctime[0] = (itu1_cnt >> 24)	& 0x00ff;
	
	ITU.TISRA.BIT.IMFA1 = 0;				// 割り込みステータスフラグクリア
}
