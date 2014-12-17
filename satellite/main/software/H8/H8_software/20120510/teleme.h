/*************************************************************************************
* ファイル名　：　teleme.h
* タイトル　　：　INVADERテレメトリ・セット用ヘッダファイル
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.31
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#ifndef TELEME_H_
#define TELEME_H_

// EEPROMのアドレスマップ
// IIC0-0~3(128byte mode)	:詳細履歴
// IIC1-0~2(128byte mode)	:粗履歴
// IIC1-3(32byte mode)		:API配信履歴(0x0000~0x0BFF)，初期化情報(0x0C00,0x0C01)

#define MODE128	128						// 128byte書き込み・読み込みモード
#define PAGE_PER_EEPROM_MODE128 0x0400	// 128byte書き込み時のEEPROM1つ当たりのページ数

#define MODE32 32						// 32byte書き込み・読み込みモード

#define API_SLAVE 3						// API配信情報スレーブアドレス
#define API_LASTPAGE 0x0BFF				// API配信書き込みアドレスの最大値

#define INITIAL_SLAVE 3					// 初期化情報スレーブアドレス
#define OBCTIME_PAGE 0x0C00				// 初期化情報書き込みアドレス（OBC時刻）
#define ERRORCOUNT_PAGE 0x0C01			// 初期化情報書き込みアドレス（エラーカウント）
#define ROUGH_SLAVE_PAGE 0x0C02	
#define ROUGH_PAGE 0x0C03
#define API_PAGE 0x0C04

#define BootModeLength 1				//　BootModeの長さ
#define BootFlagLength 32				//　BootFlagの長さ
#define CameraParamLength 18
#define MorseParamLength 18
#define DigitalkerParamLength 18
#define NoteParamLength 18
#define TextLength 18

volatile unsigned char flag_rteleme[3];	// 粗テレメトリ書き込みタイマ割り込みフラグ
volatile unsigned char flag_dteleme[3];	// 詳細テレメトリ書き込みタイマ割り込みフラグ
volatile unsigned char flag_ateleme[3];	// APIテレメトリ書き込みタイマ割り込みフラグ
volatile unsigned char flag_iteleme[3];	// 初期化情報書き込みタイマ割り込みフラグ
volatile unsigned char flag_oteleme[3];	// 試験用テレメトリ出力タイマ割り込みフラグ

volatile unsigned char ena_rteleme[3];	// 粗履歴ルーチンイネーブル
volatile unsigned char ena_dteleme[3];	// 詳細履歴ルーチンイネーブル
volatile unsigned char ena_ateleme[3];	// APIデータ履歴ルーチンイネーブル
volatile unsigned char ena_iteleme[3];	// 初期化情報ルーチンイネーブル
volatile unsigned char ena_oteleme[3];	// 試験用テレメトリルーチンイネーブル

unsigned char InitTelemetry(void);
unsigned char SetTelemetry_Rough(void);
unsigned char SetTelemetry_Detail(unsigned char mode);
unsigned char SetTelemetry_API(void);
unsigned char SetTelemetry_Initial(void);
unsigned char Read_Initial(void);
unsigned char OBC_Majority(unsigned char data_set[], unsigned char i);
unsigned char Error_Majority(unsigned char data_set[], unsigned char i);

unsigned char Setup_DTelemetry(volatile unsigned char* param, unsigned char param_length);

unsigned char WriteIicBootMode(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicBootFlag(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicBootTime(volatile unsigned long time);
unsigned char WriteIicCameraParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicMorseParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicDigitalkerParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicNoteParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextDebug(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextZParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextYParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextXParam(volatile unsigned char* param, unsigned char param_length);

unsigned char ReadSharedROM(volatile unsigned char* param, unsigned char param_length);
unsigned char LongReadSharedROM(volatile unsigned char* param, unsigned char param_length);

unsigned char GeneralWriteSharedMomery(volatile unsigned char* param, unsigned char param_length);
unsigned char GeneralWriteSharedAscii(volatile unsigned char* param, unsigned char param_length);
unsigned char GeneralReadSharedMomery(volatile unsigned char* param, unsigned char param_length);

unsigned char GeneralReadIIC0(volatile unsigned char* param, unsigned char param_length);
unsigned char GeneralReadIIC1(volatile unsigned char* param, unsigned char param_length);

static unsigned char rough_slave;
static unsigned long int rough_eep_page;
static unsigned long int api_eep_page;		// EEPROMのページ番号



#endif