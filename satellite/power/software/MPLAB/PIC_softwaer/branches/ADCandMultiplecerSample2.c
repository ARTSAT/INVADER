#include "16F877.h"
#include "stdint.h"

#device ADC=8
#fuses HS,WDT
#use delay(CLOCK=20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)

void main(void){
	int8_t data=0;
	setup_adc(ADC_CLOCK_DIV_8);
	//ADCのサンプリング設定	
	
	setup_wdt(WDT_1152MS);

	while(1){
		restart_wdt();
		
		/*
		output_high(PIN_A0);
		output_low(PIN_A1);
		output_high(PIN_A2);
		//以上アドレスピン設定
		output_low(PIN_A3);
		output_low(PIN_A4);
		output_high(PIN_A5);
		//以上エネーブルピン設定*/
		
		delay_us(50);
		output_a(0b00001111);
		setup_adc_ports(ALL_ANALOG);
		set_adc_channel(6);
		delay_us(20);
		restart_wdt();
		data=read_adc();
		data=data>>1;
		printf("Data%d:%d\n",1,data);

		delay_us(50);
		output_a(0b00001110);
		setup_adc_ports(ALL_ANALOG);
		set_adc_channel(6);
		delay_us(20);
		restart_wdt();
		data=read_adc();
		data=data>>1;
		printf("Data%d:%d\n",2,data);


		delay_us(50);
		output_a(0b00001101);
		setup_adc_ports(ALL_ANALOG);
		set_adc_channel(6);
		delay_us(20);
		restart_wdt();
		data=read_adc();
		data=data>>1;
		printf("Data%d:%d\n",3,data);

		setup_adc_ports(NO_ANALOGS);//一応戻しています。
		sleep();
	}
}