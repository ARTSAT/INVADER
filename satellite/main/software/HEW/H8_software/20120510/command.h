/*************************************************************************************
* ファイル名　：　command.h
* タイトル　　：　グローバル変数初期化ヘッダファイル
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.2.3
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* 
*************************************************************************************/

#ifndef COMMAND_H_
#define COMMAND_H_


unsigned char RegistCommand	(unsigned char CommandOrReply,
							 unsigned char *cmd,
                             unsigned char receiver_ID,
                             unsigned char sender_ID,
                             volatile unsigned char *param,
                             unsigned char param_length,
                             unsigned char reply_num);
unsigned char ExecuteCommand(void);

#endif
