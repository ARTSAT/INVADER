/*************************************************************************************
* ファイル名　：　mission.c
* タイトル　　：　INVADERミッションテレメトリ送信プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.29
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "mission.h"
#include "global.h"
#include "sci.h"
#include "cdh_pindefine.h"
#include "inthandler.h"
#include "fm_pindef.h"
#include "fm_tx.h"
#include "iic.h"
#include "teleme.h"

#define MAIN 0		//共有メモリの切り替え
#define MISSION 1
/*************************************************
        ミッションテレメトリ初期化関数
*************************************************/
unsigned char InitMissionTelemetry(void)
{
	ClearFlags(flag_misst);
	
	return 1;
}

/*************************************************
        ミッションCPUテレメトリ送信
*************************************************/
unsigned char SendMissionTelemetry(void)
{
	unsigned char i;
	
	sci2_puts("c-m-c-smm-");
	for(i=0; i<SIZE_OBCTIME; i++)		sci2_putc(status.obctime[i]);
	for(i=0; i<SIZE_VOLTAGE; i++)		sci2_putc(status.voltage[i]);
	for(i=0; i<SIZE_AMPARE; i++)		sci2_putc(status.ampare[i]);
	for(i=0; i<SIZE_TEMPERATURE; i++)	sci2_putc(status.temperature[i]);
	for(i=0; i<SIZE_GYRO; i++)			sci2_putc(status.gyro[i]);
	for(i=0; i<SIZE_MAGNET; i++)		sci2_putc(status.magnet[i]);
	sci2_puts(" \r\n");
/*	
	debug_puts("c-m-c-smm-");
	for(i=0; i<SIZE_OBCTIME; i++)		debug_putc(status.obctime[i]);
	for(i=0; i<SIZE_VOLTAGE; i++)		debug_putc(status.voltage[i]);
	for(i=0; i<SIZE_AMPARE; i++)		debug_putc(status.ampare[i]);
	for(i=0; i<SIZE_TEMPERATURE; i++)	debug_putc(status.temperature[i]);
	for(i=0; i<SIZE_GYRO; i++)			debug_putc(status.gyro[i]);
	for(i=0; i<SIZE_MAGNET; i++)		debug_putc(status.magnet[i]);
	debug_puts(" \r\n");
*/
	
	return 1;
}

/*************************************************
        ミッションCPU起動
*************************************************/
unsigned char RequestMissionReboot(void)
{
	Ax_Send("r-g-c-msr \r\n",12);
	WriteIicBootTime(mission_cnt);		//起動時間を共有メモリに記録
	debug_puts("!!!!!!-- Request Mission Reboot --!!!!!!.\n");
//	delay_us(1000);
	sci0_puts("c-p-c-msr \r\n");		// ミッションCPU起動をPICに要求

	SROM_EN = 1;				//1017版変更点
	SROM_CH = MISSION;
	
	ITU.TISRA.BIT.IMIEA2 = 0;		// タイマ割り込み禁止
	ITU.TSTR.BIT.STR2 = 0;			// カウント停止
	ITU2.TCNT = 0;					// カウンタクリア	
	
//	debug_puts("r-g-c-msr \r\n");
	
	return 1;
}

/*************************************************
        ミッションCPU起動(タイマ予約型)
*************************************************/
unsigned char ScheduledRequestMissionReboot(volatile unsigned char* param, unsigned char param_length){
	int i;
	smr_time=0;
	for(i=0;i<param_length;i++){
		smr_time=smr_time*10+(unsigned long int)(param[i]-48);
	}
	
	
	/* smr_timeが大きすぎないかを確かめる */
	/*
	if((smr_time>=4294967296)||(param_length>=10)){
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > the LIMIT. \r\n",50);

		debug_puts("\n INVALID message header with SCI1: smr_time beyonds the limit (4294967296) ");
		debug_puts("\n request is: ");
		debug_putdec(smr_time);
		debug_puts("\n");
		return 1;
	}
	*/
	/* smr_timeが現在の時間を超えていないかを確かめる */
	/*
	else if(smr_time<mission_cnt){
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > mission_cnt. \r\n",52);
		
		debug_puts("\n INVALID message header with SCI1: smr_time > mission_cnt ");
		debug_puts("\n request is: ");
		debug_putdec(smr_time);
		debug_puts("\n obc_time is: ");
		debug_putdec(mission_cnt);
		debug_puts("\n");
		return 1;
	}
	*/
	debug_puts("\n param_length: ");
	debug_putdec(param_length);
	debug_puts("\n request is: ");
	debug_putdec(smr_time);
	debug_puts("\n obc_time is: ");
	debug_putdec(mission_cnt);
	debug_puts("\n");
	
	unsigned char message_smr[21];
	message_smr[0]='r';
	message_smr[1]='-';
	message_smr[2]='g';
	message_smr[3]='-';
	message_smr[4]='c';
	message_smr[5]='-';
	message_smr[6]='s';
	message_smr[7]='m';
	message_smr[8]='r';
	message_smr[9]='-';
	message_smr[10]=(smr_time>>24);
	message_smr[11]=(smr_time>>16);
	message_smr[12]=(smr_time>>8);
	message_smr[13]=smr_time;
	message_smr[14]=(mission_cnt>>24);
	message_smr[15]=(mission_cnt>>16);
	message_smr[16]=(mission_cnt>>8);
	message_smr[17]=mission_cnt;
	message_smr[18]=0x20;
	message_smr[19]=0x0d;
	message_smr[20]=0x0a;
	
	Ax_Send(message_smr,21);
	
	debug_puts("\n r-g-c-smr-");
	debug_puthex(message_smr[10]);
	debug_puts(" ");
	debug_puthex(message_smr[11]);
	debug_puts(" ");
	debug_puthex(message_smr[12]);
	debug_puts(" ");
	debug_puthex(message_smr[13]);
	debug_puts("  ");
	debug_puthex(message_smr[14]);
	debug_puts(" ");
	debug_puthex(message_smr[15]);
	debug_puts(" ");
	debug_puthex(message_smr[16]);
	debug_puts(" ");
	debug_puthex(message_smr[17]);
	return 1;
}

/*************************************************
        ミッションCPU起動(タイマ予約&作動時間指定型)
*************************************************/
unsigned char ScheduledRequestMissionSwitch(volatile unsigned char* param, unsigned char param_length){
	int i,sms_length=0;
	sms_time=0;
	smr_time=0;
	
	/* . の前は起動している時間を表す */
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			sms_length=i;
			break;
		}
		sms_time=sms_time*10+(unsigned long int)(param[i]-48);
	}
	/* . が一回も出てこない場合はエラー */	
	if(sms_length==0){
		Ax_Send("r-g-c-sms-Error1 \r\n",19);
//		debug_puts("\n INVALID message header with SCI1: SMS needs . between two variables \n");
		return 1;
	}
	/* . の後は起動する時間を表す */
	for(i=sms_length+1;i<param_length;i++){
		smr_time=smr_time*10+(unsigned long int)(param[i]-48);
	}
	
	/* sms_timeを終了する時間にする */
	sms_time=sms_time+smr_time;
	
	/* smr_timeが大きすぎないかを確かめる */
	/*
	if(smr_time>=4294967296||(param_length-sms_length>10)){
		int ex_smr=smr_time;
		sms_time=0;
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > the LIMIT. \r\n",50);
		debug_puts("\n INVALID message header with SCI1: smr_time beyonds the limit (4294967296) ");
		debug_puts("\n request is: ");
		debug_putdec(ex_smr);
		debug_puts("\n");
		return 1;
	}
	*/
	
	/* sms_timeが大きすぎないかを確かめる 
	if(sms_time>=(4294967296-smr_time)||(sms_length>10)){
		int ex_sms=sms_time;
		sms_time=0;	
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:sms_time > the LIMIT. \r\n",50);
		debug_puts("\n INVALID message header with SCI1: sms_time beyonds the limit (4294967296) ");
		debug_puts("\n request is: ");
		debug_putdec(ex_sms);
		debug_puts("\n");
		return 1;
	}
	*/
	

	/* smr_timeが現在の時間を超えているかを確かめる */
	/*
	else if(smr_time<mission_cnt){
		int ex_smr=smr_time;
		sms_time=0;
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > mission_cnt. \r\n",52);
		debug_puts("\n INVALID message header with SCI1: smr_time beyonds mission_cnt ");
		debug_puts("\n request is: ");
		debug_putdec(ex_smr);
		debug_puts("\n obc_time is: ");
		debug_putdec(mission_cnt);
		debug_puts("\n");
		return 1;
	}
	*/
	/*
	debug_puts("\n param_length: ");
	debug_putdec(param_length-1);
	debug_puts("\n sms_length: ");
	debug_putdec(sms_length);
	debug_puts("\n smr_length: ");
	debug_putdec(param_length-sms_length-1);
	
	debug_puts("\n itu1_cnt time: ");
	debug_putdec(mission_cnt);
	debug_puts("\n sms_time time: ");
	debug_putdec(sms_time);	
	debug_puts("\n smr_time time: ");
	debug_putdec(smr_time);
	debug_puts("\n");
	*/
	i=0;
	unsigned char message_sms[32];
	message_sms[i++]='r';
	message_sms[i++]='-';
	message_sms[i++]='g';
	message_sms[i++]='-';
	message_sms[i++]='c';
	message_sms[i++]='-';
	message_sms[i++]='s';
	message_sms[i++]='m';
	message_sms[i++]='s';
	message_sms[i++]='-';
	message_sms[i++]=(smr_time>>24);
	message_sms[i++]=(smr_time>>16);
	message_sms[i++]=(smr_time>>8);
	message_sms[i++]=smr_time;
	message_sms[i++]=(sms_time>>24);
	message_sms[i++]=(sms_time>>16);
	message_sms[i++]=(sms_time>>8);
	message_sms[i++]=sms_time;
	message_sms[i++]=(mission_cnt>>24);
	message_sms[i++]=(mission_cnt>>16);
	message_sms[i++]=(mission_cnt>>8);
	message_sms[i++]=mission_cnt;
	message_sms[i++]=0x20;
	message_sms[i++]=0x0d;
	message_sms[i++]=0x0a;

	Ax_Send(message_sms,i);//たぶん長さは２５
	
	debug_puts("\n smr_time time:\t");
	debug_puthex(message_sms[10]);
	debug_puthex(message_sms[11]);
	debug_puthex(message_sms[12]);
	debug_puthex(message_sms[13]);
	debug_puts("\n sms_time time:\t");
	debug_puthex(message_sms[14]);
	debug_puthex(message_sms[15]);
	debug_puthex(message_sms[16]);
	debug_puthex(message_sms[17]);
	debug_puts("\n obc time:\t");
	debug_puthex(message_sms[18]);
	debug_puthex(message_sms[19]);
	debug_puthex(message_sms[20]);
	debug_puthex(message_sms[21]);
	return 1;
}

/*************************************************
        ミッションCPUシャットダウン
*************************************************/
unsigned char RequestMissionShutdown(void)
{	
	sci0_puts("c-p-c-msd \r\n");		// ミッションCPUシャットダウンをPICに要求

	debug_puts("!!!!!!-- Request Mission Shutdown to Power --!!!!!!.\n");
	ITU2.TCR.BIT.CCLR = 1;		// GRAのコンペアマッチでカウントクリア
	ITU.TISRA.BIT.IMIEA2 = 0;		// タイマ割り込み禁止
	ITU.TSTR.BIT.STR2 = 0;			// カウント停止
	ITU2.TCNT = 0;					// カウンタクリア
	
	SROM_EN = 0;					//1017版変更点
	SROM_CH = MAIN;
	
	Ax_Send("r-g-c-nsd \r\n",12);	// 地上へのリプライのタイミングについて要調整
//	debug_puts("r-g-c-nsd \r\n");
	
	return 1;
}



/*************************************************
        ミッションCPU強制終了
*************************************************/
unsigned char PronounceMissionAbort(void)
{
	debug_puts("!!!!!!-- Pronounce Shutodown to Mission --!!!!!!.\n");
	debug_puts("r-g-c-asd \r\n");
	
	sci2_puts("c-m-c-asd \r\n");
	
	// タイムアウト処理用タイマ(ITU2)カウント開始
	ITU.TISRA.BIT.IMFA2 = 0;	// 割り込みステータスフラグクリア
	ITU.TISRA.BIT.IMIEA2 = 1;	// タイマ割り込み許可
	ITU.TSTR.BIT.STR2 = 1;		// カウント開始
	
//	Ax_Send("r-g-c-asd \r\n",12);
	
	return 1;
}

/*************************************************
	ミッションCPU強制シャットダウン用タイマ設定
*************************************************/
unsigned char itu2_init(void)
{
	ITU.TSTR.BIT.STR2 = 0;		// カウント停止
	ITU.TSTR.BIT.STR2 = 0;		// カウント停止
	ITU2.TCNT = 0;				// カウントクリア
	
	ITU2.TCR.BIT.CCLR = 1;		// GRAのコンペアマッチでカウントクリア
	ITU2.TCR.BIT.TPSC = 3;		// タイマプリスケーラ 25MHz/8=3.125MMHz T=0.32[usec]
	ITU2.GRA = 31250-1;			// 周期10ms
	ITU2.GRB = 31250-1;			// 周期10ms

	
	ITU.TISRA.BIT.IMIEA2 = 0;	// タイマ割り込み禁止
	ITU.TISRB.BIT.IMIEB2 = 0;	// タイマ割り込み禁止

	
	return 1;
}

#define TIMEOUT	1000	// Arduino強制シャットダウン：1000*10msec = 10sec
/*************************************************
		ミッションCPU強制終了タイムアウト
*************************************************/
void INT_IMIA2(void){		
	ITU.TISRA.BIT.IMFA2 = 0;		// 割り込みステータスフラグクリア
	mission_timeout++;
	
	if(mission_timeout == TIMEOUT){
		debug_puts("\n !!!!!!!-- Mission Time Out --!!!!!!!\n");		
		sci0_puts("c-p-c-msd \r\n");// PICにシャットダウンを依頼
		
		mission_timeout = 0;
		ITU.TISRA.BIT.IMIEA2 = 0;	// タイマ割り込み禁止
		ITU.TSTR.BIT.STR2 = 0;		// カウント停止
		ITU2.TCNT = 0;				// カウンタクリア
		Ax_Send("r-g-p-msd \r\n",12);
//		debug_puts("r-g-p-msd \r\n");
		
		SROM_EN = 0;		//1017版変更点
		SROM_CH = MAIN;		//1017版変更点
		
		
	}
}

/*************************************************
		OBC時間の確認
*************************************************/
unsigned char CheckNowTime(void){
	unsigned char message_cnt[17];
	message_cnt[0]='r';
	message_cnt[1]='-';
	message_cnt[2]='g';
	message_cnt[3]='-';
	message_cnt[4]='c';
	message_cnt[5]='-';
	message_cnt[6]='c';
	message_cnt[7]='n';
	message_cnt[8]='t';
	message_cnt[9]='-';
	message_cnt[10]=(mission_cnt>>24);
	message_cnt[11]=(mission_cnt>>16);
	message_cnt[12]=(mission_cnt>>8);
	message_cnt[13]=mission_cnt;
	message_cnt[14]=0x20;
	message_cnt[15]=0x0d;
	message_cnt[16]=0x0a;
	
	Ax_Send(message_cnt,17);
/*	
	debug_puts("\n r-g-c-cnt-");
	debug_puthex(message_cnt[10]);
	debug_puts(" ");
	debug_puthex(message_cnt[11]);
	debug_puts(" ");
	debug_puthex(message_cnt[12]);
	debug_puts(" ");
	debug_puthex(message_cnt[13]);	
*/	
	debug_puts("\n itu1_cnt time: ");
	debug_putdec(mission_cnt);
	debug_puts("\n sms_time time: ");
	debug_putdec(sms_time);	
	debug_puts("\n smr_time time: ");
	debug_putdec(smr_time);
	debug_puts("\n");
}

/*************************************************
		デジトーカーON
*************************************************/
unsigned char DegitalkerON(void){
	DT_mode=1;
	fmptt_off();
	sci0_puts("c-p-c-don \r\n");
	debug_puts("DegitalkerON.\n");
	Ax_Send("r-g-c-don \r\n",12);	
	return 1;
}
/*************************************************
		デジトーカーOFF
*************************************************/
unsigned char DegitalkerOFF(void){
	DT_mode=0;
	fmptt_off();	
	sci0_puts("c-p-c-dof \r\n");
	debug_puts("DegitalkerOFF.\n");	
	Ax_Send("r-g-c-dof \r\n",12);	//FinishCommunication()に移動
	return 1;
}

/*************************************************
		ミッションからの返事
*************************************************/
unsigned char Eco(void){
	debug_puts("\n Got ECO message \n");
	return 1;
}

