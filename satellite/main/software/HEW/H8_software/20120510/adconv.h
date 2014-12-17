/******************************************************************************
* ファイル名　：　adconv.h
* タイトル　　：　INVADER AD変換ヘッダファイル
* 製作者　　　：　03-103317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.31
* 言語　　　　：　C
*******************************************************************************
* ※備考※
*
******************************************************************************/
#ifndef ADCONV_H_
#define ADCONV_H_

volatile unsigned char flag_ad[3];		// AD変換タイマ割り込みフラグ
volatile unsigned char flag_overa[3];	// 渦電流検知タイマ割り込みフラグ
volatile unsigned char flag_dispAD[3];	// AD変換結果表示割り込みフラグ
volatile unsigned char ena_ad[3];
volatile unsigned char ena_overa[3];
volatile unsigned char ena_dispAD[3];
volatile unsigned char flag_pic[3];


unsigned char adconv_init(void);
//unsigned char SetADConvert(void);
void get_sensor(void);
unsigned char disp_AD(void);

void PowerReset(void);
void RequestPowerReboot(void);
void RequestPowerShutdown(void);


#endif