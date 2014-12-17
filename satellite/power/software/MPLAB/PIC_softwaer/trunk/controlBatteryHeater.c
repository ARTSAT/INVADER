#include"Powerboard.h"

void HeaterInit(void){
	task_bh=CONTROLLBH_TASKBH;
	HeaterTest=HEATER_OFF;
	HeaterEFTest=HEATER_NTEF;
	OFFHeater();
	return ;
}


void controlBatteryTemp(void){
	int8_t oncount=0;
	int16_t battemp16=0;
	write_ResetMarker(RESET_ADC);
	
	uint8_t battemps[BATTEMPNUM];
	
	#ifdef _DEBUG_
	if(HeaterEFTest==HEATER_ONEF){
		ONHeater();
		return ;
	}else if(HeaterEFTest==HEATER_OFFEF){
		OFFHeater();
		HeaterEFTest=HEATER_NTEF;
		return;
	}
	#endif
	
	if(HeaterTest==HEATER_OFF){
		battemps[0]=getCriticalDatas(BATTERYTEMP1);
		tlm[BAT_TEMP1]=battemps[0];
		battemp16=battemps[0];
		if((battemp16)>BATTERYONTEMP)
			oncount++;

		battemps[1]=getCriticalDatas(BATTERYTEMP2);
		tlm[BAT_TEMP2]=battemps[1];
		battemp16=battemps[1];
		if((battemp16)>BATTERYONTEMP)
			oncount++;
		
		battemps[2]=getCriticalDatas(BATTERYTEMP3);
		tlm[BAT_TEMP3]=battemps[2];
		battemp16=battemps[2];
		if((battemp16)>BATTERYONTEMP)
			oncount++;
	
		if(oncount>1){//ëΩêîåà
			ONHeater();
			HeaterTest=HEATER_ON;
		}
	}else{
		battemps[0]=getCriticalDatas(BATTERYTEMP1);
		tlm[BAT_TEMP1]=battemps[0];
		battemp16=battemps[0];
		if((battemp16)<BATTERYOFFTEMP)
			oncount++;

		battemps[1]=getCriticalDatas(BATTERYTEMP2);
		tlm[BAT_TEMP2]=battemps[1];
		battemp16=battemps[1];
		if((battemp16)<BATTERYOFFTEMP)
			oncount++;
		
		battemps[2]=getCriticalDatas(BATTERYTEMP3);
		tlm[BAT_TEMP3]=battemps[2];
		battemp16=battemps[2];
		if((battemp16)<BATTERYOFFTEMP)
			oncount++;

		if(oncount>1){//ëΩêîåà
			OFFHeater();
			HeaterTest=HEATER_OFF;
		}
	}
	write_ResetMarker(RESET_OTHERS);
	task_bh=CONTROLLBH_TASKBH;
	
	return;
}

void ONHeater(void){
	output_high(BATTERYHEATER);
	return;
}


void OFFHeater(void){
	output_low(BATTERYHEATER);
	return;
}

void BHerrCheck(){
	HeaterInit();
	return;
}