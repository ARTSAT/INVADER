


/*************************************************************************************
* ファイル名　：　command.c
* タイトル　　：　INVADERコマンド処理プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.2.3
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "iodefine.h"
#include "sci.h"
#include "put_status.h"
#include "debug.h"
#include "mission.h"


// 実行優先度
#define HIGH	3
#define MIDDLE	2
#define LOW		1

// 予約しておけるタスク数
#define HIGH_NUM	10
#define MIDDLE_NUM	10
#define LOW_NUM		10

// Low
#define DMP		1		//2,4,3
#define DST		2		//2,4,1
#define DPW		3		//2.2.4	//電源デバッグ用
#define DEP		4		//2,4,4
#define DRS		5		//2,4,5

#define DMS		6		//2,4,6
#define DWT		7		//???
#define DDS		8				//詳細履歴を記録する
#define DAL		9				//生存信号停止
#define GWA		10				//共有メモリ書き込み関数(General-Ascii)


#define DTR		11		//2,4,8	//粗履歴表示
#define DTD		12		//2,4,9	//詳細履歴表示
#define DTA		13		//2,4,10//APIデータ表示
#define DDR		14		//2,4,12// Display Routine EN/DIS
#define DSR		15		//2,4,13// Set Routine EN/DIS

#define DTI		16		//2,4,11
#define DWS		17				//共有メモリへの書き込み
#define DPS		18		//2.5.18//共有メモリからの読み出し
#define DCP     19		//2.5.21
#define ECO		20		//2.4.6	//Mission応答関数

#define TON		21		//2,5,1													//Ax_Send非対応
#define TOF		22		//2,5,2													//Ax_Send非対応
#define PSR		23		//2.5.15//PIC ON
#define PSD		24		//2.5.16//PIC OFF
#define PSS		25		//2.5.17//PIC on/off

#define GR0     26		//IIC0読み込み関数(General)
#define GR1		27		//IIC1読み込み関数(General)
#define GWS		28		//2.1.10//共有メモリ書き込み関数(General)
#define GRS		29		//2.1.11//共有メモリ読み込み関数(General)
#define CNT		30		//2.1.9	//時間チェック									//Ax_Send対応

//#define HDR		49
//#define HDS		50

// Middle

#define IBM		31		//2.1.12//共有メモリ特別領域BootMode書き込み関数
#define IBF		32		//2.1.13//共有メモリ特別領域BootFlag書き込み関数
#define ICP		33		//2.1.14//共有メモリ特別領域書き込み関数(Camera Param)
#define IMP		34		//2.1.15//共有メモリ特別領域書き込み関数(Morse Param)
#define IDP		35		//2.1.16//共有メモリ特別領域書き込み関数(Digitalker Param)

#define INP		36		//2.1.17//共有メモリ特別領域書き込み関数(Note Param)
#define ITX		37		//2.1.18//共有メモリ特別領域書き込み関数(TextX Param)
#define ITY		38		//2.1.19//共有メモリ特別領域書き込み関数(TextY Param)
#define ITZ		39		//2.1.20//共有メモリ特別領域書き込み関数(TextZ Param)
#define ITD		40		//		//共有メモリ特別領域書き込み関数(Text Debug)

#define PWT		41		//2,2,1
#define RST		42		//2,2,2
#define SMR		43		//2.4.8	//ミッションCPU起動(タイマ予約型)				//Ax_Send対応
#define SMS		44		//2.4.9	//ミッションCPU起動(タイマ予約&作動時間指定型)	//Ax_Send対応
#define SMM		45		//2,3,1

#define STA		46		//2,1,1													//Ax_Send対応
#define PTR		47		//2,1,2													//Ax_Send対応
#define PTD		48		//2,1,3													//Ax_Send対応
#define PTA		49		//2,1,4													//Ax_Send対応
#define PTI		50		//2,1,5													//Ax_Send対応

#define MSD		51		//2,2,3	//MissionCPUを終了させる						//Ax_Send対応
#define MSR		52		//2,3,4	//MissionCPUを起こす							//Ax_Send対応
#define NSD		53		//2,3,2
#define ASD		54		//2,3,3													//Ax_Send対応


#define RPT     56				//パワーテレメトリ・リクエスト送信関数			//Ax_Send非対応
#define DON		57		//2.4.4	//デジトーカーON
#define DOF		58		//2.4.5	//デジトーカーOFF
#define RSR		59		//2.5.19//共有メモリ読み出し
#define RSL		60		//2.5.20//共有メモリ読み出し(画像用)

// High
#define TXR		61		//2.2.5	//FM開始時に電源系とやり取りするコマンド		//Ax_Send非対応
#define TXS		62		//2.2.6	//FM終了時に電源系とやり取りするコマンド		//Ax_Send非対応


struct command{
	unsigned char CommandOrReply;
	unsigned char ID;
	unsigned char receiver_ID;
	unsigned char sender_ID;
	volatile unsigned char* param;
	unsigned char param_length;
	unsigned char reply_num;
}
command_H[HIGH_NUM],
command_M[MIDDLE_NUM],
command_L[LOW_NUM];

static volatile unsigned char h_wp, h_rp;
static volatile unsigned char m_wp, m_rp;
static volatile unsigned char l_wp, l_rp;

/*************************************************
        		コマンドIDを返す関数
*************************************************/
unsigned char GetCommandID(unsigned char* cmd)
{
	unsigned char com_ID = 0;
	
	if(cmd[0]=='s' && cmd[1]=='t' && cmd[2]=='a')		com_ID = STA;
	else if(cmd[0]=='i' && cmd[1]=='t' && cmd[2]=='d')	com_ID = ITD;
	else if(cmd[0]=='i' && cmd[1]=='c' && cmd[2]=='p')	com_ID = ICP;
	else if(cmd[0]=='n' && cmd[1]=='s' && cmd[2]=='d')	com_ID = NSD;
	else if(cmd[0]=='a' && cmd[1]=='s' && cmd[2]=='d')	com_ID = ASD;
	else if(cmd[0]=='s' && cmd[1]=='m' && cmd[2]=='m')	com_ID = SMM;
	else if(cmd[0]=='p' && cmd[1]=='w' && cmd[2]=='t')	com_ID = PWT;
	else if(cmd[0]=='r' && cmd[1]=='s' && cmd[2]=='t')	com_ID = RST;
	else if(cmd[0]=='m' && cmd[1]=='s' && cmd[2]=='r')	com_ID = MSR;
	else if(cmd[0]=='s' && cmd[1]=='m' && cmd[2]=='r')	com_ID = SMR;
	else if(cmd[0]=='s' && cmd[1]=='m' && cmd[2]=='s')	com_ID = SMS;
	else if(cmd[0]=='m' && cmd[1]=='s' && cmd[2]=='d')	com_ID = MSD;
	else if(cmd[0]=='d' && cmd[1]=='o' && cmd[2]=='n')	com_ID = DON;
	else if(cmd[0]=='d' && cmd[1]=='o' && cmd[2]=='f')	com_ID = DOF;
	else if(cmd[0]=='i' && cmd[1]=='b' && cmd[2]=='m')	com_ID = IBM;
	else if(cmd[0]=='i' && cmd[1]=='b' && cmd[2]=='f')	com_ID = IBF;
	else if(cmd[0]=='i' && cmd[1]=='m' && cmd[2]=='p')	com_ID = IMP;
	else if(cmd[0]=='i' && cmd[1]=='d' && cmd[2]=='p')	com_ID = IDP;
	else if(cmd[0]=='i' && cmd[1]=='n' && cmd[2]=='p')	com_ID = INP;
	else if(cmd[0]=='i' && cmd[1]=='t' && cmd[2]=='z')	com_ID = ITZ;
	else if(cmd[0]=='i' && cmd[1]=='t' && cmd[2]=='y')	com_ID = ITY;
	else if(cmd[0]=='i' && cmd[1]=='t' && cmd[2]=='x')	com_ID = ITX;
	else if(cmd[0]=='r' && cmd[1]=='s' && cmd[2]=='r')	com_ID = RSR;
	else if(cmd[0]=='r' && cmd[1]=='s' && cmd[2]=='l')	com_ID = RSL;
	else if(cmd[0]=='g' && cmd[1]=='w' && cmd[2]=='s')	com_ID = GWS;
	else if(cmd[0]=='g' && cmd[1]=='r' && cmd[2]=='s')	com_ID = GRS;
	else if(cmd[0]=='g' && cmd[1]=='w' && cmd[2]=='a')	com_ID = GWA;
	
	// debug用コマンド
	else if(cmd[0]=='d' && cmd[1]=='m' && cmd[2]=='p')	com_ID = DMP;
	else if(cmd[0]=='d' && cmd[1]=='s' && cmd[2]=='t')	com_ID = DST;
	else if(cmd[0]=='d' && cmd[1]=='e' && cmd[2]=='p')	com_ID = DEP;
	else if(cmd[0]=='d' && cmd[1]=='r' && cmd[2]=='s')	com_ID = DRS;
	else if(cmd[0]=='d' && cmd[1]=='m' && cmd[2]=='s')	com_ID = DMS;
	else if(cmd[0]=='d' && cmd[1]=='w' && cmd[2]=='t')	com_ID = DWT;
	
	else if(cmd[0]=='d' && cmd[1]=='t' && cmd[2]=='r')	com_ID = DTR;
	else if(cmd[0]=='d' && cmd[1]=='t' && cmd[2]=='d')	com_ID = DTD;
	else if(cmd[0]=='d' && cmd[1]=='t' && cmd[2]=='a')	com_ID = DTA;
	else if(cmd[0]=='d' && cmd[1]=='t' && cmd[2]=='i')	com_ID = DTI;
	
	else if(cmd[0]=='d' && cmd[1]=='d' && cmd[2]=='r')	com_ID = DDR;
	else if(cmd[0]=='d' && cmd[1]=='s' && cmd[2]=='r')	com_ID = DSR;
	else if(cmd[0]=='d' && cmd[1]=='w' && cmd[2]=='s')	com_ID = DWS;
	else if(cmd[0]=='d' && cmd[1]=='p' && cmd[2]=='s')	com_ID = DPS;
	else if(cmd[0]=='d' && cmd[1]=='c' && cmd[2]=='p')	com_ID = DCP;
	
	else if(cmd[0]=='t' && cmd[1]=='o' && cmd[2]=='n')	com_ID = TON;
	else if(cmd[0]=='t' && cmd[1]=='o' && cmd[2]=='f')	com_ID = TOF;
	else if(cmd[0]=='g' && cmd[1]=='r' && cmd[2]=='0')	com_ID = GR0;
	else if(cmd[0]=='g' && cmd[1]=='r' && cmd[2]=='1')	com_ID = GR1;
	else if(cmd[0]=='r' && cmd[1]=='p' && cmd[2]=='t')	com_ID = RPT;
	else if(cmd[0]=='t' && cmd[1]=='x' && cmd[2]=='s')	com_ID = TXS;
	else if(cmd[0]=='t' && cmd[1]=='x' && cmd[2]=='r')	com_ID = TXR;
	
//	else if(cmd[0]=='h' && cmd[1]=='d' && cmd[2]=='r')	com_ID = HDR;
//	else if(cmd[0]=='h' && cmd[1]=='d' && cmd[2]=='s')	com_ID = HDS;
	
	else if(cmd[0]=='p' && cmd[1]=='t' && cmd[2]=='r')	com_ID = PTR;
	else if(cmd[0]=='p' && cmd[1]=='t' && cmd[2]=='d')	com_ID = PTD;
	else if(cmd[0]=='p' && cmd[1]=='t' && cmd[2]=='a')	com_ID = PTA;
	else if(cmd[0]=='p' && cmd[1]=='t' && cmd[2]=='i')	com_ID = PTI;
	else if(cmd[0]=='c' && cmd[1]=='n' && cmd[2]=='t')	com_ID = CNT;
	
	else if(cmd[0]=='p' && cmd[1]=='s' && cmd[2]=='r')	com_ID = PSR;
	else if(cmd[0]=='p' && cmd[1]=='s' && cmd[2]=='d')	com_ID = PSD;
	else if(cmd[0]=='p' && cmd[1]=='s' && cmd[2]=='s')	com_ID = PSS;
	
	else if(cmd[0]=='d' && cmd[1]=='p' && cmd[2]=='w')	com_ID = DPW;
	else if(cmd[0]=='d' && cmd[1]=='d' && cmd[2]=='s')	com_ID = DDS;
	else if(cmd[0]=='d' && cmd[1]=='a' && cmd[2]=='l')	com_ID = DAL;
	else if(cmd[0]=='e' && cmd[1]=='c' && cmd[2]=='o')	com_ID = ECO;



	
	else {
	//	debug_puts("\n");
	//	debug_puts("Undefined Command.\t\t");
	//	debug_puts(cmd);
	//	debug_puts("\n");
		com_ID = 0;
	}
	
	return com_ID;
}

/*************************************************
       	ミッション用コマンドIDを返す関数
*************************************************/
unsigned char GetMissionCommandID(unsigned char* cmd)
{
	unsigned char com_ID = 0;
	
	//以下のコマンド以外は無視する
	//ミッションから送られてくるデータによって誤コマンドが発生するのを防ぐため
	if(cmd[0]=='t' && cmd[1]=='o' && cmd[2]=='n')		com_ID = TON;
	else if(cmd[0]=='t' && cmd[1]=='o' && cmd[2]=='f')	com_ID = TOF;
	else if(cmd[0]=='e' && cmd[1]=='c' && cmd[2]=='o')	com_ID = ECO;
	else if(cmd[0]=='s' && cmd[1]=='m' && cmd[2]=='m')	com_ID = SMM;
	else if(cmd[0]=='a' && cmd[1]=='s' && cmd[2]=='d')	com_ID = ASD;
	else if(cmd[0]=='n' && cmd[1]=='s' && cmd[2]=='d')	com_ID = NSD;
	else if(cmd[0]=='d' && cmd[1]=='o' && cmd[2]=='n')	com_ID = DON;
	else if(cmd[0]=='d' && cmd[1]=='o' && cmd[2]=='f')	com_ID = DOF;
		
	else {
	//	debug_puts("\n");
	//	debug_puts("Undefined Command(mission).\t\t");
	//	debug_puts(cmd);
	//	debug_puts("\n");
		com_ID = 0;
	}
	
	return com_ID;
}

/*************************************************
        		コマンド登録関数
*************************************************/
unsigned char RegistCommand	(unsigned char CommandOrReply,
							 unsigned char *cmd,
                             unsigned char receiver_ID,
                             unsigned char sender_ID,
                             volatile unsigned char *param,
                             unsigned char param_length,
                             unsigned char reply_num)
{
	unsigned char i;
	unsigned char ID = GetCommandID(cmd);
	unsigned char level;
	
	unsigned char *message;//debug
	message=param;//debug
	
	if(!ID){
		debug_puts("\n");
		debug_puts("Undefined Command.\t");	// for debug
		debug_puts(cmd);
		debug_puts("\t");
		debug_puts(message);
		debug_puts("\n");
		return 0;
	}else;
	
	if(ID<=30)		level = LOW;
	else if(ID<=60)	level = MIDDLE;
	else if(ID<=90) level = HIGH;
	
	switch(level){;
	
		case HIGH:
			command_H[h_wp].CommandOrReply = CommandOrReply;
			command_H[h_wp].ID = ID;
			command_H[h_wp].param_length = param_length;
			command_H[h_wp].sender_ID = sender_ID;
			command_H[h_wp].receiver_ID = receiver_ID;
			command_H[h_wp].reply_num = reply_num;
			command_H[h_wp].param = &param[0];
			
			h_wp ++;
			if(h_wp >= HIGH_NUM)	h_wp = 0;
			break;
			
		case MIDDLE:
			command_M[m_wp].CommandOrReply = CommandOrReply;
			command_M[m_wp].ID = ID;
			command_M[m_wp].param_length = param_length;
			command_M[m_wp].sender_ID = sender_ID;
			command_M[m_wp].receiver_ID = receiver_ID;
			command_M[m_wp].reply_num = reply_num;
			command_M[m_wp].param = &param[0];
			
			m_wp ++;
			if(m_wp >= MIDDLE_NUM)	m_wp = 0;
			break;
			
		case LOW:
			command_L[l_wp].CommandOrReply = CommandOrReply;
			command_L[l_wp].ID = ID;
			command_L[l_wp].param_length = param_length;
			command_L[l_wp].sender_ID = sender_ID;
			command_L[l_wp].receiver_ID = receiver_ID;
			command_L[l_wp].reply_num = reply_num;
			command_L[l_wp].param = &param[0];
			
			l_wp ++;
			if(l_wp >= LOW_NUM)	l_wp = 0;
			break;
			
		default:
			break;
	}
	
	return 1;
}

/*************************************************
       　	ミッション用コマンド登録関数
*************************************************/
unsigned char RegistMissionCommand	(unsigned char CommandOrReply,
									 unsigned char *cmd,
                          			  unsigned char receiver_ID,
                          			   unsigned char sender_ID,
                          			   volatile unsigned char *param,
                          			   unsigned char param_length,
                          			   unsigned char reply_num)
{
	unsigned char i;
	unsigned char ID = GetMissionCommandID(cmd);
	unsigned char level = LOW;
	
	unsigned char *message;//debug
	message=param;//debug

	
	if(!ID){
		debug_puts("\n");
		debug_puts("Undefined Command(mission).\t");	// for debug
		debug_puts(cmd);
		debug_puts("\t");
		debug_puts(message);
		debug_puts("\n");

		return 0;
	}
	
	command_L[l_wp].CommandOrReply = CommandOrReply;
	command_L[l_wp].ID = ID;
	command_L[l_wp].param_length = param_length;
	command_L[l_wp].sender_ID = sender_ID;
	command_L[l_wp].receiver_ID = receiver_ID;
	command_L[l_wp].reply_num = reply_num;
	command_L[l_wp].param = &param[0];
			
	l_wp ++;
	if(l_wp >= LOW_NUM)	l_wp = 0;	
	
	return 1;
}

/*************************************************
        		コマンド処理関数
*************************************************/
unsigned char ExecuteCommand(void)
{
	
	unsigned char i;
	
	unsigned char CommandOrReply;
	unsigned char command_ID;
	volatile unsigned char *param;
	unsigned char param_length;
	unsigned char sender_ID;
	unsigned char receiver_ID;
	unsigned char reply_num;
	
	if(h_rp!=h_wp){
		CommandOrReply  = command_H[h_rp].CommandOrReply;
		command_ID 		= command_H[h_rp].ID;
		param_length 	= command_H[h_rp].param_length;
		sender_ID 		= command_H[h_rp].sender_ID;
		receiver_ID 	= command_H[h_rp].receiver_ID;
		reply_num 		= command_H[h_rp].reply_num;
		param = &command_H[h_rp].param[0];
		h_rp++;
		if(h_rp >= HIGH_NUM)	h_rp = 0;
	}
	else if(m_rp!=m_wp){
		CommandOrReply  = command_M[m_rp].CommandOrReply;
		command_ID = command_M[m_rp].ID;
		param_length 	= command_M[m_rp].param_length;
		sender_ID 		= command_M[m_rp].sender_ID;
		receiver_ID 	= command_M[m_rp].receiver_ID;
		reply_num 		= command_M[m_rp].reply_num;
		param = &command_M[m_rp].param[0];
		m_rp++;
		if(m_rp >= MIDDLE_NUM)	m_rp = 0;
	}
	else if(l_rp!=l_wp){
		CommandOrReply  = command_L[l_rp].CommandOrReply;
		command_ID = command_L[l_rp].ID;
		param_length 	= command_L[l_rp].param_length;
		sender_ID 		= command_L[l_rp].sender_ID;
		receiver_ID 	= command_L[l_rp].receiver_ID;
		reply_num 		= command_L[l_rp].reply_num;
		param = &command_L[l_rp].param[0];
		l_rp++;
		if(l_rp >= LOW_NUM)		l_rp = 0;
	}	
	else return 0;
	
	if(CommandOrReply == 'c'){
		
		switch(command_ID){
			case DMP:
			DebugMultiplexer();
			break;
			
			case DST:
			DebugPut_Status();
			break;
			
			case DEP:
			DebugEEPROM(param,param_length);
			break;
			
			case DRS:
			RequestMainShutdown();
			break;
			
			case DMS:
			PronounceMissionAbort();
			break;
			
			case DWT:
			while(1);
			break;
			
			case DTR:
			DebugPut_Rough(param, param_length);
			break;
			
			case DTD:
			DebugPut_Detail(param, param_length);
			break;
			
			case DTA:
			DebugPut_API(param, param_length);
			break;
			
			case DTI:
			DebugPut_Initial();
			break;
			
			case DDR:
			DisplayRoutine();
			break;
			
			case DSR:
			SetRoutine(param);
			break;
			
			case DWS:
			DebugWrite_SharedROM(param, param_length);
			break;
			
			case DPS:
			DebugPut_SharedROM(param, param_length);
			break;
			
			case DCP:
			Check_PowerData();
			break;
			
			case STA:
			SendStatusToGS(reply_num);
			break;
			
			case ITD:
			WriteIicTextDebug(param, param_length);
			break;
			
			case ICP:
			WriteIicCameraParam(param, param_length);
			break;
			
			case NSD:
			RequestMissionShutdown();
			break;
			
			case SMM:
			SendMissionTelemetry();
			break;
			
			case IBM:
			WriteIicBootMode(param, param_length);
			break;
			
			case IBF:
			WriteIicBootFlag(param, param_length);
			break;
			
			case IMP:
			WriteIicMorseParam(param, param_length);
			break;
			
			case IDP:
			WriteIicDigitalkerParam(param, param_length);
			break;
			
			case INP:
			WriteIicNoteParam(param, param_length);
			break;
			
			case ITZ:
			WriteIicTextZParam(param, param_length);
			break;
			
			case ITY:
			WriteIicTextYParam(param, param_length);
			break;
			
			case ITX:
			WriteIicTextXParam(param, param_length);
			break;
			
			case MSD:
			PronounceMissionAbort();
			break;
			
			case MSR:
			RequestMissionReboot();
			break;
			
			
			case TON:
			TransmitMessage();
			break;
			
			case TOF:
			FinishSendMessage();
			break;
			
			case RPT:
			RequestPowerTelemetry();
			break;
/*			
			case HDR:
			SendHDRtoPIC();
			break;
			
			case HDS:
			SendHDStoPIC();
			break;
*/			
			case PTR:
			PutOut_Rough(param, param_length);
			break;
			
			case PTD:
			PutOut_Detail(param, param_length);
			break;
			
			case PTA:
			PutOut_API(param, param_length);
			break;
			
			case PTI:
			PutOut_Initial();
			break;
			
			case SMR:
			ScheduledRequestMissionReboot(param, param_length);
			break;
			
			case CNT:
			CheckNowTime();
			break;
			
			case SMS:
			ScheduledRequestMissionSwitch(param, param_length);
			break;
			
			case DON:
			DegitalkerON();
			break;
			
			case DOF:
			DegitalkerOFF();
			break;
			
			case PSR:
			RequestPowerReboot();
			break;
			
			case PSD:
			RequestPowerShutdown();
			break;
			
			case PSS:
			PowerReset();
			break;
			
			case RSR:
			ReadSharedROM(param, param_length);
			break;
			
			case RSL:
			LongReadSharedROM(param, param_length);
			break;
			
			case GWS:
			GeneralWriteSharedMomery(param, param_length);
			break;
			
			case GWA:
			GeneralWriteSharedAscii(param, param_length);
			break;
			
			case GRS:
			GeneralReadSharedMomery(param, param_length);
			break;
			
			case ECO:
			Eco();
			break;

			case GR0:
			GeneralReadIIC0(param, param_length);
			break;
			
			case GR1:
			GeneralReadIIC1(param, param_length);
			break;
			
			case DDS:
			Setup_DTelemetry(param, param_length);
			break;
			
			case DAL:
			StopAlivePeriod();
			break;

			
			default:
			return 0;
			break;
		}
	}
	else if(CommandOrReply == 'r'){
		
		switch(command_ID){
			case PWT:
			UpdatePowerTelemetry(param);
			break;
			
			case ASD:
			RequestMissionShutdown();
			break;
			
			case RST:
			debug_puts("rst command regist.\n");
			break;
			
			case TXR:
			InitPLL();
			break;
			
			
			case TXS:
			FinishCommunication();
			break;
			
			case DPW:
			DebugPowerMessage(param, param_length);
			break;;
			
			default:
			return 0;
			break;
		}
	}
	else	return 0;
	
	return 1;	
}