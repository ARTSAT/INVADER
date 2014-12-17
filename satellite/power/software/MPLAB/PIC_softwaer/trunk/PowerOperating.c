#include "Powerboard.h"

//Arduinoを落とす
void missionOBCKill(void){
	missionOBCKillflag1=MISSIONOBCNOTKILLING;
	missionOBCKillflag2=MISSIONOBCNOTKILLING;
	missionOBCKillflag3=MISSIONOBCNOTKILLING;
	MISSIONSWSND=SWMISSIONLINE;
	MISSIONSWMSG=MISSIONKILLMSG;	
	output_low(ARKILL);//ミッションオフ
}

//Arduinoを起動する
void missionOBCStandup(void){
	output_low(ARKILL);//ミッションオフ(下準備)
	missionOBCRunflag=MISSIONOBCNOTRUN;
	missionOBCKillflag1=MISSIONOBCNOTKILLING;
	missionOBCKillflag2=MISSIONOBCNOTKILLING;
	missionOBCKillflag3=MISSIONOBCNOTKILLING;
	MISSIONSWSND=SWMISSIONLINE;
	MISSIONSWMSG=MISSIONRUNMSG;
	missionOBCLiveflag=MISSIONOBCLIVING;
	task_po=WAIT_TASKPO;
	output_high(ARKILL);//ミッションオン
}

//H8を落とす
void mainOBCKill(void){
	output_high(H8KILL);
	mainOBCTimer=1;
	if(mainOBCResetReason==MAINOBCKILLTORST)
		mainStandupTime=MAINOBCOVERCURRENT;
	else
		mainStandupTime=MAINOBCRESET;
	mainOBCKillflag1=MAINOBCNOTKILLING;
	mainOBCKillflag2=MAINOBCNOTKILLING;
	mainOBCKillflag3=MAINOBCNOTKILLING;
	mainOBCLiveflag=MAINOBCDEAD;
	return;
}

	
void mainOBCStandup(void){
	output_low(H8KILL);
	mainOBCLiveflag=MAINOBCLIVING;
	mainOBCTimer=0;
	mainStandupTime=0;
	task_po=WAIT_TASKPO;
}

void comUnitsStandup(void){
	if(RXTimer>RXSTANDUPTIME){
		output_low(RXLINE);//RXon
		RXLiveflag=1;
		RXTimer=0;
		printf("r-c-p-rxr \r\n");
		//onしたことをメインに伝える
	}
	
	if(TXflag==TXONFLAG){
		output_low(CWDAT);
		output_low(CWSTB);
		output_low(CWCLK);
		cwptt_off();
		output_high(TXLINE);//TXon
		TXflag=TXLIVING;
		PLLflag=CWOFF;
		int8_t v;
		for(v=0;v<45;v++){
			restart_wdt();
			delay_ms(10);//delay関数によるwdt発動対策
		}
		printf("r-c-p-txr \r\n");
		//onしたことをメインに伝える
	}
	task_po=WAIT_TASKPO;
	return;
}
 

void OBCKill(void){
	if(((missionOBCKillflag1==MISSIONOBCKILLING)&&(missionOBCKillflag2==MISSIONOBCKILLING)&&(missionOBCKillflag3==MISSIONOBCKILLING))){
		missionOBCKill();
	}else{
		missionOBCKillflag1=MISSIONOBCNOTKILLING;
		missionOBCKillflag2=MISSIONOBCNOTKILLING;
		missionOBCKillflag3=MISSIONOBCNOTKILLING;
	}
	if((mainOBCKillflag1==MAINOBCKILLING)&&(mainOBCKillflag2==MAINOBCKILLING)&&(mainOBCKillflag3==MAINOBCKILLING)){
		mainOBCKill();
	}else{
		mainOBCKillflag1=MAINOBCNOTKILLING;
		mainOBCKillflag2=MAINOBCNOTKILLING;
		mainOBCKillflag3=MAINOBCNOTKILLING;
	}
	task_po=WAIT_TASKPO;
}

void RXUNITKill(void){
	output_high(RXLINE);
	RXTimer=1;
	RXLiveflag=0;
	RXKillflag1=RXNOTKILLING;
	RXKillflag2=RXNOTKILLING;
	RXKillflag3=RXNOTKILLING;
	//offしたことをメインに伝える
	task_po=WAIT_TASKPO;
	return;
}

void TXUNITKILL(){
	output_low(CWDAT);
	output_low(CWSTB);
	output_low(CWCLK);
	output_low(TXLINE);
	TXLiveflag=TXDEAD;
	TXKillflag1=TXNOTKILLING;
	TXKillflag2=TXNOTKILLING;
	TXKillflag3=TXNOTKILLING;
	
	
	
	PLLflag=CWWILL;//CWPLL再設定
	//offしたことをメインに伝える
	task_po=WAIT_TASKPO;
	printf("r-c-p-txs \r\n");
	return;
}

void POInit(void){
	output_low(ARKILL);//off
	//output_high(H8KILL);//off
	//output_high(RXLINE);//off
	output_low(TXLINE);//off
	//output_high(CWLINE);//off
	//delay_ms(10);
	output_low(H8KILL);//on
	//delay_ms(10);
	output_low(CWLINE);//on
	//delay_ms(10);
	output_low(RXLINE);//on
	mainOBCResetReason=0;
	mainOBCKillflag1=MAINOBCNOTKILLING;
	mainOBCKillflag2=MAINOBCNOTKILLING;
	mainOBCKillflag3=MAINOBCNOTKILLING;
	mainOBCLiveflag=MAINOBCLIVING;
	mainOBCRTimerflag=0;

	missionOBCLiveflag=MISSIONOBCDEAD;
	missionOBCKillflag1=MISSIONOBCNOTKILLING;
	missionOBCKillflag2=MISSIONOBCNOTKILLING;
	missionOBCKillflag3=MISSIONOBCNOTKILLING;
	missionOBCRunflag=MISSIONOBCNOTRUN;

	mainStandupTime=0;
	mainOBCTimer=0;
	mainOBCRTimer=0;
	mainOBCRMinute=0;
	mainOBCRHour=0;
	RXTimer=0;
	TXflag=0;
	TXKillflag1=TXNOTKILLING;
	TXKillflag2=TXNOTKILLING;
	TXKillflag3=TXNOTKILLING;
	RXKillflag1=RXNOTKILLING;
	RXKillflag2=RXNOTKILLING;
	RXKillflag3=RXNOTKILLING;
	TXLiveflag=TXLIVING;
	RXLiveflag=1;
	task_po=WAIT_TASKPO;
}

//電流超過による電源カットなら20サイクル、ただのリセットなら5サイクルだけ待ってからH8を起動する。
void wait_PO(void){
	if(mainOBCTimer>0){
		mainOBCTimer++;
		if(mainOBCTimer>mainStandupTime){
			task_po=MAINOBCSTANDUP_TASKPO;
			return;
		}
	}

	mainOBCRTimer++;
	
	if(mainOBCRTimer>ONEMINUTE){
		mainOBCRMinute++;
		mainOBCRTimer=0;
	}

	if(mainOBCRMinute>=ONEHOUR){
		mainOBCRHour++;
		mainOBCRMinute=0;
		mainOBCRTimer=0;
	}

	if(mainOBCRHour>=MAINOBCRESETCYCLE){
		mainOBCKillflag1=MAINOBCKILLING;
		mainOBCKillflag2=MAINOBCKILLING;
		mainOBCKillflag3=MAINOBCKILLING;
		mainOBCResetReason=MAINOBCKILLTORST;
		mainOBCRTimer=0;
		mainOBCRMinute=0;
		mainOBCRHour=0;
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-mr \r\n");
		#endif
	}


	if(mainOBCKillflag1==MAINOBCKILLING||missionOBCKillflag1==MISSIONOBCKILLING){
		task_po=OBCKILL_TASKPO;
	}else if(missionOBCRunflag==MISSIONOBCRUN){
		task_po=MISSIONOBCSTANDUP_TASKPO;
	
	}else if(RXTimer>0&&RXTimer++>RXSTANDUPTIME){
		task_po=COMUNITSTANDUP_TASKPO;

	}else if(TXflag==TXONFLAG){
		task_po=COMUNITSTANDUP_TASKPO;

	}else if(RXKillflag1==RXKILLING&&RXKillflag2==RXKILLING&&RXKillflag3==RXKILLING){
		task_po=RXUNITKILL_TASKPO;	

	}else if(TXKillflag1==TXKILLING&&TXKillflag2==TXKILLING&&TXKillflag3==TXKILLING){
		task_po=TXUNITKILL_TASKPO;	
	}

}
