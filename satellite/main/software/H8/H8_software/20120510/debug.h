/*************************************************************************************
* ファイル名　：　debug.h
* タイトル　　：　INVADERデバッグコマンドヘッダファイル
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.19
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/

#ifndef DEBUG_H_
#define DEBUG_H_

unsigned char DebugPut_Status(void);
unsigned char DebugMultiplexer(void);
unsigned char DebugEEPROM(volatile unsigned char* param,unsigned char param_length);
unsigned char DebugPut_Rough(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_Detail(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_API(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_Initial();
unsigned char Debug_SharedROM(unsigned long int page, unsigned char *dat, unsigned char s_ad, unsigned char byte);
unsigned char DebugWrite_SharedROM(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_SharedROM(volatile unsigned char* param, unsigned char param_length);
unsigned char DisplayRoutine(void);
unsigned char SetRoutine(volatile unsigned char* param);
unsigned char StopAlivePeriod(void);

#endif