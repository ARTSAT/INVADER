#include "PowerBoard.h"

void antennaInit(){
	task_ae=INTERVAL_TASKAE;
	antennacount=0;
	antennaminute=0;
	antennahour=0;
	antennaExpandflag=0;
}


void wait_AntennaExpand(){
	int8_t oncount=0;
	int16_t battemp16=0;
	uint8_t battemps[BATTEMPNUM];

	battemps[0]=getCriticalDatas(BATTERYTEMP1);
	tlm[BAT_TEMP1]=battemps[0];
	battemp16=battemps[0];
	if((battemp16)<EXPANDANTENNAON)
		oncount++;

	battemps[1]=getCriticalDatas(BATTERYTEMP2);
	tlm[BAT_TEMP2]=battemps[1];
	battemp16=battemps[1];
	if((battemp16)<EXPANDANTENNAON)
		oncount++;
	
	battemps[2]=getCriticalDatas(BATTERYTEMP3);
	tlm[BAT_TEMP3]=battemps[2];
	battemp16=battemps[2];
	if((battemp16)<EXPANDANTENNAON)
		oncount++;

	uint8_t panelcur;
	panelcur=tlm[PNLALL_CUR];

	if(oncount>1 && panelcur>SUNSHINECUR){//‘½”Œˆ
		antennacount++;
		#ifdef _DEBUGf_
			int16_t x = antennacount;
			if(x%100==0)
				printf("r-c-p-dpw-no%c \r\n",x/100+48);
		#endif
	}

	if(antennacount>ANTENNAWAITTIME){
		task_ae=ANTENNAEXPAND_TASKAE;
		antennacount=0;
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-sae \r\n",);
		#endif
	}
}//WAIT_TASKAE


void interval_AntennaExpand(){
	int8_t oncount=0;
	int16_t battemp16=0;
	uint8_t battemps[BATTEMPNUM];

	battemps[0]=getCriticalDatas(BATTERYTEMP1);
	tlm[BAT_TEMP1]=battemps[0];
	battemp16=battemps[0];
	if((battemp16)<EXPANDANTENNAON)
		oncount++;

	battemps[1]=getCriticalDatas(BATTERYTEMP2);
	tlm[BAT_TEMP2]=battemps[1];
	battemp16=battemps[1];
	if((battemp16)<EXPANDANTENNAON)
		oncount++;
	
	battemps[2]=getCriticalDatas(BATTERYTEMP3);
	tlm[BAT_TEMP3]=battemps[2];
	battemp16=battemps[2];
	if((battemp16)<EXPANDANTENNAON)
		oncount++;

	uint8_t panelcur;
	panelcur=tlm[PNLALL_CUR];

	if(oncount>1 && panelcur>SUNSHINECUR){//‘½”Œˆ
		antennacount++;
	}

	if(antennaExpandflag==1){
		task_ae=SKIP_TASKAE;
		antennacount=0;
		antennaminute=0;
		antennahour=0;
		setAntennaExpandedFlag();
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-aef \r\n");
		#endif
	}
	if(antennacount>ONEMINUTE){
		antennaminute++;
		antennacount=0;
	}
	if(antennaminute>=ONEHOUR){
		antennahour++;
		antennacount=0;
		antennaminute=0;
	}
	if(antennahour>=ANTENNAINTERVALTIME){
		task_ae=ANTENNAEXPAND_TASKAE;
		antennacount=0;
		antennaminute=0;
		antennahour=0;
		#ifdef _DEBUG_
			printf("r-c-p-dpw-inf \r\n");
		#endif
	}
}//INTERVAL_TASKAE


void antennaExpander(){
	task_bh=SKIP_TASKBH;
	OFFHeater();
	antennacount++;
	output_high(ANTENNAEXPAND);

	if(tlm[PNLALL_CUR]<SUNSHINECUR){
		antennacount=0;
		output_low(ANTENNAEXPAND);
		task_ae=WAIT_TASKAE;
		task_bh=CONTROLLBH_TASKBH;
	}

	if(antennacount>ANTENNAEXPANDTIME){
		ADCInit();
		LSInit();
		WCInit();
		CWInit();
		HKInit();
		HeaterInit();
		PDInit();
		antennaInit();
		WBInit();
		antennacount=0;
		output_low(ANTENNAEXPAND);
	}
	
	#ifdef _DEBUGf_
		int16_t x = antennacount;
		if(x%100==0)
				printf("r-c-p-dpw-ae%c \r\n",x/100+48);
	#endif
	return;
}//ANTENNAEXPAND_TASKAE


void AEerrCheck(void){
	if(getAntennaExpandFlag()){
		task_ae=WAIT_TASKAE;
		antennacount=0;
	}
	else
		task_ae=SKIP_TASKAE;
	return;
}