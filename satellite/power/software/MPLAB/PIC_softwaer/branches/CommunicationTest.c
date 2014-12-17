/*************************************************************************************
* �t�@�C�����@�F�@CommunicationTest.c
* �^�C�g���@�@�F�@INVADER�p���[�{�[�h�R�}���h�F���e�X�g�p�v���O����
* ����ҁ@�@�@�F�@�F�������l
* �����@�@�@�@�F�@������w�H�w���d�C�d�q�H�w��
* ������@�@�@�F�@2012.3.19
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* H8�Ƃ̒ʐM�e�X�g(3/19���{)�p�ł�
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
	���̊֐��A������̏I���͕��A'\r'�ŔF�����Ă���̂ŁA���ӂ��Ă��������B
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