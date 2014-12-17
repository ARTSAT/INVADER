/*************************************************************************************
* ファイル名　：　pwr.h
* タイトル　　：　INVADERパワーテレメトリ送信リクエスト/受信プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.19
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#ifndef PWRT_H_
#define PWRT_H_

volatile unsigned char flag_pwrt[3];
volatile unsigned char ena_pwrt[3];

volatile unsigned char flag_alive[3];
volatile unsigned char ena_alive[3];

unsigned char InitPowerTelemetry(void);
unsigned char RequestPowerTelemetry(void);
unsigned char RequestMainShutdown(void);
unsigned char UpdatePowerTelemetry(volatile unsigned char *pwrt);

#endif