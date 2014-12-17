#include "PowerBoard.h"

uint8_t readADC(uint8_t id){
	//MPXを通したデータ取得用
	uint8_t data=0;
	int8_t port;
	//6(7)bit目はどのポートに対応しているかを表しています。
	if(bit_test(id,6))
		port=MUL_ANPIN_B;//PortB
	else
		port=MUL_ANPIN_A;//PortA	
	setMUL(id);
	
	set_adc_channel(port);
	restart_wdt();
	delay_us(20);
	data=read_adc();
	return data;
}

 void ADCInit(void){
	setup_adc(ADC_CLOCK_DIV_8);
	setup_adc_ports(ALL_ANALOG);
	task_pw=0;
	for(j=0;j<TLMMAX;j++){
		tlm[j]=0;
	}
	return ;
}

uint8_t getCriticalDatas(uint8_t port){
	uint8_t data;	
	set_adc_channel(port);
	restart_wdt();
	delay_us(20);
	data=read_adc();
	return data;
}