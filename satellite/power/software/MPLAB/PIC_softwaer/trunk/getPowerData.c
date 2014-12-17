#include "Powerboard.h"

void collectpdata(){
	/*for(i=0;i<8;i++){
		tlm[i]=readADC(pdcount+0b00001000);
		tlm[i+8]=readADC(pdcount+0b01001000);
		if(i<MPX2BMAX)
			tlm[i+16]=readADC(pdcount+0b01010010);
	}*/
	
	write_ResetMarker(RESET_ADC);
	tlm[pdcount]=readADC(pdcount+0b00001000);
	tlm[pdcount+8]=readADC(pdcount+0b01001000);
	if(pdcount<MPX2BMAX)
		tlm[pdcount+16]=readADC(pdcount+0b01010010);
	
	//tlm[pdcount+24]=readADC(pdcount+0b01010000);
	//マルチプレクサ2のAポートは未使用
	
	pdcount++;
	if(pdcount>=8)
		pdcount=0;
	if(pdcount>6 && tlm[BAT_VOL]==0){
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-adcd \r\n");
		#endif
		delay_ms(60000);
	}
}

void PDInit(){
	pdcount=0;
	task_pd=COLLECTPDATA_TASKPD;
	int8_t v;
	for(v=0;v<TLMMAX;v++){
		tlm[i]=0;
	}
}

void PDerrCheck(){
	PDInit();
	return;
}