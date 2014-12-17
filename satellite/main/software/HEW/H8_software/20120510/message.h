/*************************************************************************************
* ファイル名　：　message.h
* タイトル　　：　INVADERコマンド認識・返答ヘッダファイル
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.2.3
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/


#ifndef MESSAGE_H_
#define MESSAGE_H_

volatile unsigned char flag_scirx[3];
volatile unsigned char flag_exe[3];

unsigned char InitMessage(void);
unsigned char FindCommand(void);
unsigned char TransmitMessage(void);
unsigned char InitPLL(void);
unsigned char SendMessage(void);

#endif