/*************************************************************************************
* ファイル名　：　CommunicationTest.c
* タイトル　　：　INVADERパワーボードコマンド認識テスト用プログラム
* 製作者　　　：　宇佐美尚人
* 所属　　　　：　東京大学工学部電気電子工学科
* 製作日　　　：　2012.3.19
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* H8との通信テスト(3/19実施)用です
* 
*************************************************************************************/

#include "CommunicationTest.h"

#int_rda
void isr_rcv(){
	disable_interrupts(GLOBAL);
	int8_t i=0;
	//restart_wdt();
	gets(rcvBuffer);
	/*
	この関数、文字列の終わりは復帰'\r'で認識しているので、注意してください。
	*/
	//printf("OK?\n");
	if(!strcmp(rcvBuffer,tlg)){
		printf(send);
		/*for(i=0;i<10;i++)
			putc(send[i]);*/
		
		for(i=0;i<10;i++){
			putc((char)i);
		}
		putc(' ');
		putc('\r');
		putc('\n');
	}
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);

	
}

void main(void){
	t=0;
	setup_wdt(WDT_1152MS);
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);
	
	while(1){
		restart_wdt();
		delay_ms(1000);
	}
}