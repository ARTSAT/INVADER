#include "16F877.h"
#include "stdint.h"

#device ADC=8
#fuses HS,WDT
#use delay(clock=20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)


void main(void){

	int8_t data=0;
	setup_adc(ADC_CLOCK_DIV_8);
	//ADCのサンプリング設定	

	setup_wdt(WDT_1152MS);
	while(1){
		restart_wdt();/*
		output_high(PIN_A0);
		output_low(PIN_A1);
		output_high(PIN_A2);
		//以上アドレスピン設定
		output_low(PIN_A3);
		output_low(PIN_A4);
		output_high(PIN_A5);
		//以上エネーブルピン設定*/
		output_a(0b00001111);
		setup_adc_ports(ALL_ANALOG);
		set_adc_channel(6);
		/*
		ここでADCを適用できるピンと実際に適用するピンを決めます。
		ちなみに仕様として各ピンの役割を個別に設定できないようです。
		ですがすべてアナログに設定しても問題なくエネーブルピンやアドレスピンは動いてくれているので見なかったことにしましょう。
		*/
		delay_us(20);
		restart_wdt();
		data=read_adc();
		setup_adc_ports(NO_ANALOGS);//一応戻しています。
		putc((char)data);
		sleep();
	}
}