/*************************************************************************************
* ファイル名　：　LiveCheckR.c
* タイトル　　：　INVADERパワーボード生存信号受信テスト用プログラム
* 製作者　　　：　宇佐美尚人
* 所属　　　　：　東京大学工学部電気電子工学科
* 製作日　　　：　2012.3.16
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* PWMで作られたパルスを一定時間内に16回受信できるかを確認しています。
* LiveCheckS.cとセットです。
*************************************************************************************/


#include "16F877.h"
#include "stdint.h"
#fuses HS,WDT
#use delay(CLOCK=20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)

short a=0;

#int_ccp2
void ccp_isr(){
	a=1;
	disable_interrupts(GLOBAL);
}

void main(void){
	setup_ccp2(CCP_CAPTURE_FE|CCP_CAPTURE_DIV_16);
	setup_timer_1(T1_DIV_BY_8);
	enable_interrupts(INT_CCP2);
	enable_interrupts(GLOBAL);
	while(1){
		a=0;
		set_timer1(0);
		enable_interrupts(GLOBAL);
		delay_ms(1);
		if(a)
			printf("Get Pulses!!\n");
		else
			printf("NO Pulses!!\n");
		disable_interrupts(GLOBAL);
		delay_ms(1000);
	}
}