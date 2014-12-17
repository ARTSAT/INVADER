#include"Powerboard.h"

void WCInit(){
	task_pw=WATCHCURRENT_TASKPW;
	mainOCtimer=0;
	mainOCcount=0;
}

void watchCurrent(){
	write_ResetMarker(RESET_ADC);
	
	//uint8_t msncur;
	uint8_t rxcur;
	uint8_t maincur;
	uint8_t txcur;
	
	maincur=getCriticalDatas(MAINOBCCURRENT);
	tlm[MAIN_CUR]=maincur;
	if(maincur>MAINCURRENTLIMIT){
		mainOCcount=1;
	}

	if(mainOCcount>0){
		mainOCtimer++;
		if(mainOCtimer>20){
			if(maincur>MAINCURRENTLIMIT){
				mainOCcount++;
			}else{
				mainOCcount=0;
			}
			mainOCtimer=0;
		}
	}
	
	if(mainOCcount>3){
		mainOCcount=0;
		POInit();
		mainOBCKillflag1=MAINOBCKILLING;
		mainOBCKillflag2=MAINOBCKILLING;
		mainOBCKillflag3=MAINOBCKILLING;
		TXKillflag1=TXKILLING;
		TXKillflag2=TXKILLING;
		TXKillflag3=TXKILLING;
		task_po=OBCKILL_TASKPO;//ほんとはだめだが、ここだけは仕方ない
		return;
	}

	//msncur=getCriticalDatas(MISSIONOBCCURRENT);
	//tlm[MISSION_CUR]=msncur;
	/*if(msncur>MISSIONCURRENTLIMIT){
		missionOBCKillflag1=MISSIONOBCKILLING;
		missionOBCKillflag2=MISSIONOBCKILLING;
		missionOBCKillflag3=MISSIONOBCKILLING;
		task_po=OBCKILL_TASKPO;//ほんとはだめだが、ここだけは仕方がない
		writeError(MISSIONError);
		return;
	}*/
	
	txcur=readADC((TX_CUR&0b00000111)+0b01001000);
	tlm[TX_CUR]=txcur;
	/*if(txcur>TXCURRENTLIMIT){
		TXKillflag1=TXKILLING;
		TXKillflag2=TXKILLING;
		TXKillflag3=TXKILLING;
		task_po=TXUNITKILL_TASKPO;
		return;
	}
	(11/24)*/
	rxcur=getCriticalDatas(RXCURRENT);
	tlm[RX_CUR]=rxcur;
	/*if(rxcur>RXCURRENTLIMIT){
		RXKillflag1=RXKILLING;
		RXKillflag2=RXKILLING;
		RXKillflag3=RXKILLING;
		task_po=RXUNITKILL_TASKPO;
		writeError(RXError);
		return;
	}*/

	write_ResetMarker(RESET_OTHERS);
}

void WCerrCheck(){
	WCInit();
	return;
}