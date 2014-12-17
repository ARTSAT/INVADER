#include "16F877.h"
#include "stdint.h"

#device ADC=8
#fuses HS,WDT
#use delay(clock=20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)


void main(void){

	int8_t data=0;
	setup_adc(ADC_CLOCK_DIV_8);
	//ADC�̃T���v�����O�ݒ�	

	setup_wdt(WDT_1152MS);
	while(1){
		restart_wdt();/*
		output_high(PIN_A0);
		output_low(PIN_A1);
		output_high(PIN_A2);
		//�ȏ�A�h���X�s���ݒ�
		output_low(PIN_A3);
		output_low(PIN_A4);
		output_high(PIN_A5);
		//�ȏ�G�l�[�u���s���ݒ�*/
		output_a(0b00001111);
		setup_adc_ports(ALL_ANALOG);
		set_adc_channel(6);
		/*
		������ADC��K�p�ł���s���Ǝ��ۂɓK�p����s�������߂܂��B
		���Ȃ݂Ɏd�l�Ƃ��Ċe�s���̖������ʂɐݒ�ł��Ȃ��悤�ł��B
		�ł������ׂăA�i���O�ɐݒ肵�Ă����Ȃ��G�l�[�u���s����A�h���X�s���͓����Ă���Ă���̂Ō��Ȃ��������Ƃɂ��܂��傤�B
		*/
		delay_us(20);
		restart_wdt();
		data=read_adc();
		setup_adc_ports(NO_ANALOGS);//�ꉞ�߂��Ă��܂��B
		putc((char)data);
		sleep();
	}
}