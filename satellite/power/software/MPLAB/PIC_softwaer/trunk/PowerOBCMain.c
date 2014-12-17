#include "Powerboard.h"
#include "ADC.c"
#include "LiveSign.c"
#include "controlCW.c"
#include "ResetWorks.c"
#include "watchCurrent.c"
#include "controlBatteryHeater.c"
#include "PowerOperating.c"
#include "expandAntenna.c"
#include "Communication.c"
#include "getPowerData.c"
#include "watchBatteryVOL.c"

void main(void){
	POInit();
	delay_ms(10);
	
	#ifdef _DEBUG_
		printf("c-c-p-oso-%u \r\n",restart_cause());
	#endif
	
	//incResetCounts(read_ResetMarker());
	//write_ResetMarker(RESET_OTHERS);
	ADCInit();
	LSInit();
	WCInit();
	CWInit();
	HKInit();
	HeaterInit();
	PDInit();
	RSTInit();
	WBInit();

	enable_interrupts(INT_RDA);//受信割り込み許可
	enable_interrupts(GLOBAL);//割り込み許可、これで割り込みが初めて可能になる

	//マルチタスキング実装中
	while(1){
		restart_wdt();//wdtはここだけで使いましょう。
		
		while(get_timer1()<CWINTERVAL);//ポーリングにしました。
		set_timer1(0);
		//CWタスク
		switch(task_cw){
			case CONTROLLCW_TASKCW:	controllCW();
									break;
			case SETCW_TASKCW:		setCW();
									break;
			case SETPLL_TASKCW:		set_pll_cw();
									break;
			case INTERVAL_TASKCW:	intervalCW();
									break;
			case SKIP_TASKCW: 		//何もしない
									break;
			default:				CWInit();
									task_cw=SETPLL_TASKCW;//レジスタ反転用
									break;			
		}//CW終
		
		//通信タスク
		switch(task_hk){
			case WAIT_TASKHK:	wait_hk();
									break;
			case RCVCMD_TASKHK:		rcv_cmd();
									break;
			case SENDTLM_TASKHK:	sendtl();
									break;
			case SKIP_TASKHK:		//何もしない
									break;
			default:				HKerrCheck();;//EEPROMなどのフラグをチェックすべき
									break;
		}//通信終
		
		//電流チェックタスク
		switch(task_pw){
			case WATCHCURRENT_TASKPW: 	watchCurrent();
										break;
			case SKIP_TASKPW:			//なにもしない
										break;
			default:					WCerrCheck();//レジスタ反転用
										break;
		}//電流監視終
		
		//電源操作タスク
		switch(task_po){
			case WAIT_TASKPO: wait_PO();
					break;
			case OBCKILL_TASKPO: OBCKill();
					break;
			case MAINOBCSTANDUP_TASKPO: mainOBCStandup();
					break;
			case MISSIONOBCSTANDUP_TASKPO: missionOBCStandup();
					break;
			case TXUNITKILL_TASKPO		:TXUNITKILL();
					break;
			case RXUNITKILL_TASKPO		:RXUNITKILL();
					break;
			case COMUNITSTANDUP_TASKPO	:comUnitsStandup();
					break;
			case SKIP_TASKPO://何もしない
					break;
			default:task_po=WAIT_TASKPO;//レジスタ反転用
					break;
		}//電源操作終
	
		//バッテリーヒーター制御
		switch(task_bh){
			case CONTROLLBH_TASKBH: controlBatteryTemp();
									break;
			case SKIP_TASKBH:		//何もしない
									break;
			default:				BHerrCheck();//EEPROMのフラグをチェックすべき
									break;
		}//バッテリヒーター終
		//要ADCアノマリ対応
		
		
		//生存信号チェック
		switch(task_ls){
			case GETLS_TASKLS: 	getLiveSign();
								break;
			case WAIT_TASKLS:	wait_LS();
								break;
			case SKIP_TASKLS:	//何もしない
								break;
			default:			task_ls=WAIT_TASKLS;//レジスタ反転用
								break;
		}//生存信号終

		//アンテナ展開
		switch(task_ae){
			case ANTENNAEXPAND_TASKAE:antennaExpander();
									break;
			case WAIT_TASKAE		:wait_AntennaExpand();
									break;
			case INTERVAL_TASKAE	:interval_AntennaExpand();//アンテナ展開を周期的に行うための休止期間
									break;
			case SKIP_TASKAE		://何もしない
									break;
			default:				AEerrCheck();//EEPROMのフラグをチェック
									break;
		}//アンテナ終
		
		//各種電源データ収集		
		switch(task_pd){
			case COLLECTPDATA_TASKPD:collectpdata();
									break;
			case SKIP_TASKPD		://何もしない
									break;
			default:				PDerrCheck();//EEPROM内のフラグをチェックすべき
									break;	
		}//pd終
		
		//電池充電操作
		switch(task_wb){
			case CHRGING_TASKWB		:chrging();
									break;
			case FULLED_TASKWB		:fulled();
									break;
			case DPLTD_TASKWB		:dpltd();
									break;
			default					:wbInit();
									task_wb=CHRGING_TASKWB;				
									break;
		}//充電終わり
		
		

		/*
		//hogehoge
		switch(task_hoge){
			case 	:

		}//hoge終


		*/

	}//while終

}//main終