/*************************************************************************************
* ファイル名　：　global.h
* タイトル　　：　グローバル変数初期化ヘッダファイル
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.30
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* 
*************************************************************************************/
#ifndef GLOBAL_H_
#define GLOBAL_H_

#define	SIZE_OBCTIME		4
#define SIZE_VOLTAGE		6
#define SIZE_AMPARE			20
#define	SIZE_TEMPERATURE	20
#define	SIZE_GYRO			3
#define SIZE_MAGNET			6
#define SIZE_RSSI			2
#define	SIZE_MISSION		19
#define SIZE_POWER			11
#define SIZE_INIT			8
#define SIZE_MODE			1
#define SIZE_CHARGEFLAG		2
#define SIZE_FLAG			3
#define SIZE_ERROR			9
#define SIZE_MESSAGE		13
#define SIZE_STATUS			4+6+20+20+3+6+2+19+11+8+1+2+3+9+13
#define MESSAGE_GLOBAL_LENGTH 204

struct dataset_cdh{
	volatile unsigned char	obctime[SIZE_OBCTIME];
	volatile unsigned char	voltage[SIZE_VOLTAGE];
	volatile unsigned char	ampare[SIZE_AMPARE];
	volatile unsigned char	temperature[SIZE_TEMPERATURE];
	volatile unsigned char	gyro[SIZE_GYRO];
	volatile unsigned char	magnet[SIZE_MAGNET];
	volatile unsigned char	rssi[SIZE_RSSI];
	volatile unsigned char	mission[SIZE_MISSION];
	volatile unsigned char	power[SIZE_POWER];
	volatile unsigned char	init[SIZE_INIT];
	volatile unsigned char	mode[SIZE_MODE];
	volatile unsigned char	chargeflag[SIZE_CHARGEFLAG];
	volatile unsigned char	flag[SIZE_FLAG];
	volatile unsigned char	error_counter[SIZE_ERROR];
	volatile unsigned char	message_counter[SIZE_MESSAGE];
}status;

volatile unsigned char mpx[4][8];
volatile unsigned char num_overAmpare;

volatile unsigned char  message_global[MESSAGE_GLOBAL_LENGTH];	//通信系もアクセスする

unsigned char InitStatus(void);
unsigned char InitRoutine(void);

unsigned char CheckFlags(volatile unsigned char *flag);
void ClearFlags(volatile unsigned char *flag);
void SetFlags(volatile unsigned char *flag);

#endif
