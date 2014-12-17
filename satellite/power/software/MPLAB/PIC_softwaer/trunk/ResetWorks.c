#include "Powerboard.h"

void write_ResetMarker(uint8_t m){
	//write_eeprom(RESETMARKERAD,m);
	return;
}

uint8_t read_ResetMarker(void){
	return read_eeprom(RESETMARKERAD);
}

void setAntennaExpandedFlag(void){
	write_eeprom(ANTENNAEXFLAGAD1,ANTENNAEXPANDED);
	write_eeprom(ANTENNAEXFLAGAD2,ANTENNAEXPANDED);
	write_eeprom(ANTENNAEXFLAGAD3,ANTENNAEXPANDED);
	return;
}

short getAntennaExpandFlag(void){
	uint8_t excount=0;
	
	#ifdef _ANTEXPNDTEST_
		return 0;//for debug
	#endif
	
	uint8_t exflag=read_eeprom(ANTENNAEXFLAGAD1);
	int8_t ii;
	for(ii=0;ii<8;ii++){
		if(bit_test(exflag,ii))
			excount++;
	}

	exflag=read_eeprom(ANTENNAEXFLAGAD2);
	
	for(ii=0;ii<8;ii++){
		if(bit_test(exflag,ii))
			excount++;
	}

	exflag=read_eeprom(ANTENNAEXFLAGAD3);
	
	for(ii=0;ii<8;ii++){
		if(bit_test(exflag,ii))
			excount++;
	}

	#ifdef _DEBUGf_
		printf("r-c-p-dpw-%d \r\n",excount);
	#endif

	if(excount>ANTENNATH){
		return 0;
	}else{
		//output_high(CWLINE);
		return 1;
	}
}

void incResetCounts(uint8_t ad){
	uint8_t rscount=read_eeprom(ad);
	rscount++;
	write_eeprom(ad,rscount);
	return;
}

uint8_t getResetCounts(uint8_t ad){
	if(rcv_wp==0)
	return read_eeprom(ad);
}

void clrResetCounts(uint8_t ad){
	write_eeprom(ad,0);
	return;
}

void RSTInit(void){
	if(getAntennaExpandFlag()){
		task_ae=WAIT_TASKAE;
		task_cw=SKIP_TASKCW;
		antennacount=0;
		antennaminute=0;
		antennahour=0;
		antennaExpandflag=0;
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-ae \r\n");
		#endif
	}else{
		task_ae=SKIP_TASKAE;
		task_cw=SETPLL_TASKCW;
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-nae \r\n");
		#endif
	}//‚Ü‚¸‚ÍƒAƒ“ƒeƒi
}