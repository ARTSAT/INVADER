/*************************************************************************************
* ファイル名　：　put_status.h
* タイトル　　：　INVADERステータス送信用ヘッダファイル
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.15
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#ifndef PUTSTATUS_H_
#define PUTSTATUS_H_

unsigned char SendStatusToGS(unsigned char reply_num);
unsigned char SendTelemetryToGS(unsigned char reply_num, volatile unsigned char *param, unsigned char param_length);

#endif