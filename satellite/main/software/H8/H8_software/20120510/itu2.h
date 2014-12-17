/*************************************************************************************
* ファイル名　：　itu2.h
* タイトル　　：　INVADERタイマ(CH2)ヘッダファイル
* 製作者　　　：　03-120495 西山万里
* 所属　　　　：　東京大学工学部電気電子工学科
* 製作日　　　：　2012.6.4
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* H8内部タイマ(ITU-CH2)
* intimia0(): 10msec周期(100Hz)で割り込み処理を発生。
* H8外部クロックとしては25MHzを前提。
* 
*************************************************************************************/

#ifndef ITU0_H_
#define ITU0_H_

static volatile unsigned long itu0_cnt;           // 10msecの割り込み毎にインクリメント
static volatile unsigned long my_wdt; 	          // 10msecの割り込み毎にインクリメント mainloopを一回終えるとリセット

unsigned char itu0_init(void);
unsigned char itu0_start(void);
unsigned char itu0_stop(void);
unsigned char itu0_display(void);

#endif
