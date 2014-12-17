#include "16F877.h"
#include "stdint.h"
#fuses HS,WDT
#use delay(CLOCK=20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)

#define FIRST 0x11
#define SECOND 0x22
#define THIRD 0x33
#define FOURTH 0x44

#int_rda
void isr_rcv(){
	disable_interrupts(GLOBAL);
	volatile int i=1;
	printf("RESET!!\n");
	while(i){
	};
}

void main(void){
	char data=0;
	setup_wdt(WDT_1152MS);
	if(restart_cause()!=NORMAL_POWER_UP){
		data=read_eeprom(0);
		printf("Restart:%d\n",restart_cause());
		switch(data){
			case	FIRST:printf("I was at FIRST!!\n");
						break;
			case	SECOND:printf("I was at SECOND!!\n");
						break;
			case	THIRD:printf("I was at THIRD!!\n");
						break;
			case	FOURTH:printf("I was at FOURTH!!\n");
						break;
			default :printf("????????\n");
						break;
		}
	}
	write_eeprom(0,0);
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);
	
	while(1){
		restart_wdt();
		printf("Now I'm at 1.\n");
		write_eeprom(0,FIRST);
		delay_ms(500);
		restart_wdt();
		printf("Now I'm at 2.\n");
		write_eeprom(0,SECOND);
		delay_ms(500);
		restart_wdt();
		printf("Now I'm at 3.\n");
		write_eeprom(0,THIRD);
		delay_ms(500);
		restart_wdt();
		printf("Now I'm at 4.\n");
		write_eeprom(0,FOURTH);
		delay_ms(500);
	}
}