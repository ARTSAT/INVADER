/*************************************************************************************
* ファイル名　：　message.c
* タイトル　　：　INVADERコマンド認識・返答プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.2.3
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "global.h"
#include "iodefine.h"
#include "sci.h"
#include "command.h"
#include "message.h"
#include "pwr.h"
#include "mission.h"
#include "fm_pllcontrol.h"
#include "fm_tx.h"
#include "fm_pindef.h"


#define MESSAGE_MAX_COUNT   4
#define MESSAGE_MAX_LENGTH  200

#define ON  1
#define OFF 0

static volatile unsigned char  message_buff[MESSAGE_MAX_COUNT][MESSAGE_MAX_LENGTH];
static volatile unsigned short message_length_buff[MESSAGE_MAX_COUNT];
static volatile unsigned short message_wp;
static volatile unsigned short message_rp;
static volatile unsigned short message_length;
static volatile unsigned long  message_counter;

static unsigned char mission_buff[MESSAGE_MAX_COUNT][MESSAGE_MAX_LENGTH];		//mission系専用のメッセージバッファ
static volatile unsigned short mission_length_buff[MESSAGE_MAX_COUNT];
static volatile unsigned short mission_wp;
static volatile unsigned short mission_rp;
static volatile unsigned short mission_length;
static volatile unsigned long  mission_counter;

static volatile unsigned char  message_fm[MESSAGE_MAX_LENGTH];

/*************************************************
		コマンド初期化関数
*************************************************/
unsigned char InitMessage(void)
{
	ClearFlags(flag_scirx);
	
	return 1;
}


/*************************************************
		SCI0コマンド認識関数
--------------------------------------------------
	SCI0のリングバッファのメッセージを、
	古い方から1つだけ探し出す
*************************************************/
unsigned char SplitMessage_SCI0(void)
{
	static volatile unsigned char split_sci0_buff[256];	// 通信系との干渉より、512から減じた
	static volatile unsigned short split_sci0_wp;
	
	static volatile unsigned short i;
	static volatile unsigned char found;
	static volatile short start;
	
	short uc = -1;
	
	do{
		uc = sci0_getc();
		//debug_puts("message_sci0: ");
		//debug_putc((unsigned char)uc);
		if(uc >= 0)
		{
			split_sci0_buff[split_sci0_wp] = (unsigned char)uc;
			
			if(split_sci0_wp >= 2)
			{
				unsigned char data   = split_sci0_buff[split_sci0_wp];		//                data
				unsigned char bdata  = split_sci0_buff[split_sci0_wp-1];	// before         data
				unsigned char bbdata = split_sci0_buff[split_sci0_wp-2];	// before before  data
				;
				// " \r\n" で終わるコードを見つけたら
				if((bbdata == 0x20)&&(bdata == '\r')&&(data == '\n'))
				{
					i=0;
					found = 0;
					start = (short)(split_sci0_wp-5);
					
					// split_sci0_buffer[]の最後から探し始めて、フォーマットと一致しているか確認する
					while(start >= 0)
					{
						unsigned char c;
						c = split_sci0_buff[start];
						
						if((c == 'c')||(c == 'r')){
							if(split_sci0_buff[start+1] == '-'){
									if(split_sci0_buff[start+3] == '-'){
											if(split_sci0_buff[start+5] == '-'){
												found = 1;
												break;	// exit from while(start >= 0)
											}
									}
							}
						}
						start--;
					}//end of while
					
					// for debug
					if(start > 0)	// コマンドとコマンドの間に誤コマンドが侵入したら
					{	
						unsigned short k;
						//debug_puts((unsigned char*)"INVALID message header with SCI0: ");
						//for(k=0;k<start;k++)	debug_putc(split_sci0_buff[k]);
						//debug_puts((unsigned char*)"\n");
						
					}//end of debug
				
				}//end of if((bbdata == '\t')&&(bdata == '\r')&&(data == '\n')
				
				// メッセージが発見されたら
				if(found)
				{
					for(i=start;i<=split_sci0_wp;i++)	message_buff[message_wp][i-start] = split_sci0_buff[i];
				
					// for debug
					//debug_puts((unsigned char*)"message(SCI0):");
					//debug_puts("\n message from power \t");
					//for(i=start;i<=split_sci0_wp;i++)	debug_putc(message_buff[message_wp][i-start]);
					//debug_puts("\n");
					//debug_puts((unsigned char*)"\n");
					// end of debug
					
					message_buff[message_wp][split_sci0_wp+1-start] = '\0';                          // null文字を付加
					message_length_buff[message_wp] = (unsigned short)(split_sci0_wp+1-start);       // メッセージの長さを記録(null文字まない)
					message_counter++;
					message_wp++;
					
					if(message_wp >= MESSAGE_MAX_COUNT)	message_wp = 0;
					
					message_length ++;
					split_sci0_wp = 0;
				}
			}//end of if(split_sci0_wp >= 2)
		
			if(!found)	split_sci0_wp++;
			else		found = 0;
		}
	}while(uc >= 0);
	
	return 1;
}


/*************************************************
		SCI1コマンド認識関数
--------------------------------------------------
	SCI1のリングバッファのメッセージを、
	古い方から1つだけ探し出す	
*************************************************/
unsigned char SplitMessage_SCI1(void)
{	
	static unsigned char split_sci1_buff[256];	// 通信系との干渉より、512から減じた
	static unsigned short split_sci1_wp;
	
	static volatile unsigned short i;
	static volatile unsigned char found;
	static volatile short start;
	
	short uc = -1;
	
	do{
		uc = sci1_getc();
		//debug_puts("message_sci1: ");
		//debug_putc((unsigned char)uc);
		if(uc >= 0)
		{
			split_sci1_buff[split_sci1_wp] = (unsigned char)uc;
			
			if(split_sci1_wp >= 2)
			{
				unsigned char data   = split_sci1_buff[split_sci1_wp];		//                data
				unsigned char bdata  = split_sci1_buff[split_sci1_wp-1];	// before         data
				unsigned char bbdata = split_sci1_buff[split_sci1_wp-2];	// before before  data
				
				// " \r\n" で終わるコードを見つけたら
				if((bbdata == 0x20)&&(bdata == '\r')&&(data == '\n'))
				{
					i=0;
					found = 0;
					start = (short)(split_sci1_wp-5);
					
					// split_sci1_buffer[]の最後から探し始めて、フォーマットと一致しているか確認する
					while(start >= 0)
					{
						unsigned char c;
						c = split_sci1_buff[start];
						
						if((c == 'c')||(c == 'r')){
							if(split_sci1_buff[start+1] == '-'){
								if(split_sci1_buff[start+3] == '-'){
									if(split_sci1_buff[start+5] == '-'){
										found = 1;
											break;	// exit from while(start >= 0)
									}
								}
							}
						}
						start--;
					}//end of while
					
					// for debug
					if(start > 0)	// コマンドとコマンドの間に誤コマンドが侵入したら
					{	
						unsigned short k;
						debug_puts((unsigned char*)"INVALID message header with SCI1: ");
						for(k=0;k<start;k++)	debug_putc(split_sci1_buff[k]);
						debug_puts((unsigned char*)"\n");
						
					}//end of debug
				
				}//end of if((bbdata == '\t')&&(bdata == '\r')&&(data == '\n')
				
/************************THD72用のためのdebug用************************
				// " \r" で終わるコードを見つけたら
				else if((bbdata == 0x20)&&(bdata == '\r'))
				{
					i=0;
					found = 0;
					start = (short)(split_sci1_wp-5);
					
					// split_sci1_buffer[]の最後から探し始めて、フォーマットと一致しているか確認する
					while(start >= 0)
					{
						unsigned char c;
						c = split_sci1_buff[start];
						
						if((c == 'c')||(c == 'r')){
							if(split_sci1_buff[start+1] == '-'){
								if(split_sci1_buff[start+3] == '-'){
									if(split_sci1_buff[start+5] == '-'){
										found = 1;
											break;	// exit from while(start >= 0)
									}
								}
							}
						}
						start--;
					}//end of while
					
					// for debug
					if(start > 0)	// コマンドとコマンドの間に誤コマンドが侵入したら
					{	
						unsigned short k;
						debug_puts((unsigned char*)"INVALID message header with SCI1: ");
						for(k=0;k<start;k++)	debug_putc(split_sci1_buff[k]);
						debug_puts((unsigned char*)"\n");
						
					}//end of debug
				
				}//end of if((bbdata == '\t')&&(bdata == '\r')&&(data == '\n')
				
************************THD72用のためのdebug用おしまい************************/
				
				
				// メッセージが発見されたら
				if(found)
				{
					
					/*宛先コードが'p'の場合、sci0に直接送り以降の処理を飛ばす*/
				
					if(split_sci1_buff[start+2]=='p'){
						debug_puts("\n message for power : ");
						
						unsigned char reply_buff[128];
						unsigned int k=0;
						
						for(i=start;i<=split_sci1_wp;i++){
							debug_putc(split_sci1_buff[i]);
							sci0_putc(split_sci1_buff[i]);
							
							reply_buff[k++]=split_sci1_buff[i];
							
						}
						found = 0;
						
						Ax_Send(reply_buff,k);
						
						message_counter++;
						message_wp++;
						if(message_wp >= MESSAGE_MAX_COUNT)	message_wp = 0;
						message_length ++;
						split_sci1_wp = 0;
						
						debug_puts("\n");
						return 1;
					}
									
					for(i=start;i<=split_sci1_wp;i++)	message_buff[message_wp][i-start] = split_sci1_buff[i];
				
					// for debug
					//debug_puts((unsigned char*)"message(SCI1):");
					//for(i=start;i<=split_sci1_wp;i++)	debug_putc(message_buff[message_wp][i-start]);
					//debug_puts((unsigned char*)"\n");
					// end of debug
					
					message_buff[message_wp][split_sci1_wp+1-start] = '\0';                          // null文字を付加
					message_length_buff[message_wp] = (unsigned short)(split_sci1_wp+1-start);       // メッセージの長さを記録(null文字まない)
					message_counter++;
					message_wp++;
					
					if(message_wp >= MESSAGE_MAX_COUNT)	message_wp = 0;
					
					message_length ++;
					split_sci1_wp = 0;
				}
			}//end of if(split_sci1_wp >= 2)
		
			if(!found)	split_sci1_wp++;
			else		found = 0;
		}
	}while(uc >= 0);
	
	return 1;
}

/*************************************************
		SCI2コマンド認識関数
--------------------------------------------------
	SCI2のリングバッファのメッセージを、
	古い方から1つだけ探し出す	
*************************************************/
unsigned char SplitMessage_SCI2(void)
{	
	static unsigned char split_sci2_buff[256];	// 通信系との干渉より、512から減じた
	static unsigned short split_sci2_wp;
	
	static volatile unsigned short i;
	static volatile unsigned char found;
	static volatile short start;
	
	short uc = -1;
	
	do{
		uc = sci2_getc();
		//debug_puts("message_sci2: ");
		//debug_putc((unsigned char)uc);	//missionとのかみ合わせで送信されてくるデータを受け取れるか確認した名残り
		if(uc >= 0)
		{
			split_sci2_buff[split_sci2_wp] = (unsigned char)uc;
			//debug_putc((unsigned char)uc);
			
			if(split_sci2_wp >= 2)
			{
				unsigned char data   = split_sci2_buff[split_sci2_wp];		//                data
				unsigned char bdata  = split_sci2_buff[split_sci2_wp-1];	// before         data
				unsigned char bbdata = split_sci2_buff[split_sci2_wp-2];	// before before  data
				
				// " \r\n" で終わるコードを見つけたら
				if((bbdata == 0x20)&&(bdata == '\r')&&(data == '\n'))
				{
					i=0;
					found = 0;
					start = (short)(split_sci2_wp-5);
					
					// split_sci1_buffer[]の最後から探し始めて、フォーマットと一致しているか確認する
					while(start >= 0)
					{
						unsigned char c;
						c = split_sci2_buff[start];
						
						if((c == 'c')||(c == 'r')){
							if(split_sci2_buff[start+1] == '-'){
								if(split_sci2_buff[start+3] == '-'){
									if(split_sci2_buff[start+5] == '-'){
										found = 1;
											break;	// exit from while(start >= 0)
									}
								}
							}
						}
						start--;
					}//end of while
					
					// for debug
					if(start > 0)	// コマンドとコマンドの間に誤コマンドが侵入したら
					{	
						unsigned short k;
						debug_puts((unsigned char*)"INVALID message header with SCI2: ");
						for(k=0;k<start;k++)	debug_putc(split_sci2_buff[k]);
						debug_puts((unsigned char*)"\n");
						
					}//end of debug
				
				}//end of if((bbdata == '\t')&&(bdata == '\r')&&(data == '\n')
				
				// メッセージが発見されたら
				if(found)
				{
					/*宛先コードが'r'の場合、sci0に直接送り以降の処理を飛ばす*/
					if(split_sci2_buff[start+2]=='p'){
						debug_puts("\n message for power (mission): ");
						for(i=start;i<split_sci2_wp;i++){
							debug_putc(split_sci2_buff[i]);
							sci0_putc(split_sci2_buff[i]);
						}
						found = 0;
						
						mission_counter++;
						mission_wp++;
						if(mission_wp >= MESSAGE_MAX_COUNT)	mission_wp = 0;
						mission_length ++;
						split_sci2_wp = 0;
						
						debug_puts("\n");
						return 1;
					}
					
					for(i=start;i<=split_sci2_wp;i++)	mission_buff[mission_wp][i-start] = split_sci2_buff[i];
				
					// for debug
					//debug_puts((unsigned char*)"message(SCI2):");
					//for(i=start;i<=split_sci2_wp;i++)	debug_putc(message_buff[message_wp][i-start]);
					//debug_puts((unsigned char*)"\n");
					// end of debug
					
					mission_buff[mission_wp][split_sci2_wp+1-start] = '\0';                          // null文字を付加
					mission_length_buff[mission_wp] = (unsigned short)(split_sci2_wp+1-start);       // メッセージの長さを記録(null文字まない)
					mission_counter++;
					mission_wp++;
					
					if(mission_wp >= MESSAGE_MAX_COUNT)	mission_wp = 0;
					
					mission_length ++;
					split_sci2_wp = 0;
				}
			}//end of if(split_sci1_wp >= 2)
		
			if(!found)	split_sci2_wp++;
			else		found = 0;
		}
	}while(uc >= 0);
	
	return 1;
}

/*************************************************
		コマンド処理関数
*************************************************/
unsigned char RecognizeCommand(void)
{
	unsigned char CommandOrReply = 0;
	unsigned char header = 0;
	unsigned char sender_ID = 0;
	unsigned char receiver_ID = 0;
	unsigned char cmd[4];
	unsigned char param_length = 0;
	unsigned char param_start = 0;
	unsigned char reply_num = 0;
	
	unsigned char index = 0;
	
	if(message_length > 0)
	{
		// format matching
		if(message_buff[message_rp][1] != '-')
		{
			message_rp++;
			if(message_rp >= MESSAGE_MAX_COUNT)	message_rp = 0;
			message_length--;
			return 1;
		}
		
		// format matching
		else if(message_buff[message_rp][3] != '-')
		{
			message_rp++;
			if(message_rp >= MESSAGE_MAX_COUNT)	message_rp = 0;
      		message_length--;
			return 1;
		}
		
		// ヘッダを取得
		header = message_buff[message_rp][0];
		
		// コマンド/リプライ認識
		if(header == 'c'|| header == 'r')
		{
			CommandOrReply = header;
			
			// 受信者識別
			if((message_buff[message_rp][2] == 'c') ||
			   (message_buff[message_rp][2] == 'p') ||
			   (message_buff[message_rp][2] == 'm') ||
			   (message_buff[message_rp][2] == 't') ||
			   (message_buff[message_rp][2] == 'd') ||
			   (message_buff[message_rp][2] == 'g')   )	receiver_ID = message_buff[message_rp][2];
			
			// コマンド認識本体
			// 応答回数を記述 c-c-*-***-(reply_num)-data
			if(receiver_ID)
			{
				sender_ID = message_buff[message_rp][4];
				
				//コマンド文字列の取得
				cmd[0] = message_buff[message_rp][6];
				cmd[1] = message_buff[message_rp][7];
				cmd[2] = message_buff[message_rp][8];
				cmd[3] = '\0';
				
				// with param
				if(message_buff[message_rp][9] == '-')
				{
					// c-*-*-***-(reply_num)-param
					if(message_buff[message_rp][11] == '-')
					{
						reply_num = message_buff[message_rp][10];
						param_start = 12;
					}
				
					// c-*-*-***-param
					else
					{
						reply_num = '1';
						param_start = 10;
					}
				}
				
				// no param(c-*-*-***)
				else
				{
					reply_num = '1';
					param_start = 9;
				}
				
				if(message_length_buff[message_rp] > param_start + 3)
					param_length = message_length_buff[message_rp] - (param_start+3);	// messageの長さ(space,\r,\nを含まない)
				
				// for debug
				//debug_puts((unsigned char*)"Command regist.\n");
				// end of debug
				
				RegistCommand(CommandOrReply,cmd,receiver_ID,sender_ID,&message_buff[message_rp][param_start],param_length,reply_num);
					//ExecuteCommand();
			}
			
			message_rp++;
			if(message_rp >= MESSAGE_MAX_COUNT)	message_rp = 0;
			message_length--;
			return 1;
		}
		
		else
		{
			debug_puts((unsigned char*)"Format Error.\n");
			message_rp++;
			
			if(message_rp >= MESSAGE_MAX_COUNT)	message_rp = 0;
			message_length--;
			return 0;
		}
	}
	return 0;
}

/*************************************************
		ミッション用コマンド処理関数
*************************************************/
unsigned char RecognizeMissionCommand(void)
{
	unsigned char CommandOrReply = 0;
	unsigned char header = 0;
	unsigned char sender_ID = 0;
	unsigned char receiver_ID = 0;
	unsigned char cmd[4];
	unsigned char param_length = 0;
	unsigned char param_start = 0;
	unsigned char reply_num = 0;
	
	unsigned char index = 0;
	
	if(mission_length > 0)
	{
		// format matching
		if(mission_buff[mission_rp][1] != '-')
		{
			mission_rp++;
			if(mission_rp >= MESSAGE_MAX_COUNT)	mission_rp = 0;
			mission_length--;
			return 1;
		}
		
		// format matching
		else if(mission_buff[mission_rp][3] != '-')
		{
			mission_rp++;
			if(mission_rp >= MESSAGE_MAX_COUNT)	mission_rp = 0;
      		mission_length--;
			return 1;
		}
		
		// ヘッダを取得
		header = mission_buff[mission_rp][0];
		
		// コマンド/リプライ認識
		if(header == 'c'|| header == 'r')
		{
			CommandOrReply = header;
			
			// 受信者識別
			if((mission_buff[mission_rp][2] == 'c') ||
			   (mission_buff[mission_rp][2] == 'p') ||
			   (mission_buff[mission_rp][2] == 'm') ||
			   (mission_buff[mission_rp][2] == 't') ||
			   (mission_buff[mission_rp][2] == 'd') ||
			   (mission_buff[mission_rp][2] == 'g')   )	receiver_ID = mission_buff[mission_rp][2];
			
			// コマンド認識本体
			// 応答回数を記述 c-c-*-***-(reply_num)-data
			if(receiver_ID)
			{
				sender_ID = mission_buff[mission_rp][4];
				
				//コマンド文字列の取得
				cmd[0] = mission_buff[mission_rp][6];
				cmd[1] = mission_buff[mission_rp][7];
				cmd[2] = mission_buff[mission_rp][8];
				cmd[3] = '\0';
				
				// with param
				if(mission_buff[mission_rp][9] == '-')
				{
					// c-*-*-***-(reply_num)-param
					if(mission_buff[mission_rp][11] == '-')
					{
						reply_num = mission_buff[mission_rp][10];
						param_start = 12;
					}
				
					// c-*-*-***-param
					else
					{
						reply_num = '1';
						param_start = 10;
					}
				}
				
				// no param(c-*-*-***)
				else
				{
					reply_num = '1';
					param_start = 9;
				}
				
				if(mission_length_buff[mission_rp] > param_start + 3)
					param_length = mission_length_buff[mission_rp] - (param_start+3);	// messageの長さ(space,\r,\nを含まない)
				
				// for debug
				//debug_puts((unsigned char*)"Command regist.\n");
				// end of debug
				
				RegistMissionCommand(CommandOrReply,cmd,receiver_ID,sender_ID,&mission_buff[mission_rp][param_start],param_length,reply_num);
					//ExecuteCommand();	
			}
			
			mission_rp++;
			if(mission_rp >= MESSAGE_MAX_COUNT)	mission_rp = 0;
			mission_length--;
			return 1;
		}
		
		else
		{
			//debug_puts((unsigned char*)"Format Error.\n");
			//debug_puts((unsigned char *)mission_buff);
			message_rp++;
			
			if(mission_rp >= MESSAGE_MAX_COUNT)	mission_rp = 0;
			mission_length--;
			return 0;
		}
	}
	return 0;
}


/*************************************************
		SCIコマンド認識/処理関数
*************************************************/
unsigned char FindCommand(void)
{	

	SplitMessage_SCI0();
	SplitMessage_SCI1();
	SplitMessage_SCI2();
	
	RecognizeCommand();
	RecognizeMissionCommand();
			
	return 1;
}

/*************************************************
		データ送信開始関数
*************************************************/
unsigned char TransmitMessage(void)
{	
	
	fmptt_off();
	sci0_puts("c-p-c-txr \r\n");
	FMCWSW 	= 1;//0
	FMDTSW = 1;
	debug_puts("Start FM set up.\n");
	flag_ton=1;
			
	return 1;
}

/*************************************************
		デジトーカー送信開始関数
*************************************************/
unsigned char TransmitDigitalker(void)
{	
	fmptt_off();
	sci0_puts("c-p-c-txr \r\n");
	FMCWSW 	= 1;//0
	FMDTSW = 0;
	debug_puts("Start Digitalker set up.\n");
	flag_ton=1;
			
	return 1;
}

/*************************************************
		PLL初期化関数
*************************************************/
unsigned char InitPLL(void){
	int i;
	debug_puts("\n Init PLL...");	
	if(FM_mode==1){
		i=0;
		FM_mode=5;
		set_pll_fm();
		fmptt_off();
		flag_ton=0;
//	if(FM_mode!=0){	//if(FM_mode==1) の入る前
		FM_mode=2;
		debug_puts("Init PLL done(FM).\n");	
	}
//	else{			//if(FM_mode==1) の入る前
	else if(DT_mode==2){
		i=0;
		DT_mode=5;
		set_pll_fm();
		fmptt_off();
		flag_ton=0;
		
		FM_mode=0;
		DT_mode=3;
		debug_puts("Init PLL done(DT).\n");	
	}
	else{
		i++;
		if(i>10){
			debug_puts("Init PLL Stop.\n");	
			FM_mode=3;
			DT_mode=0;
			i=0;
		}
	}
			
	delay_ms(4000);
	return 1;
}

/*************************************************
		データ送信関数
*************************************************/
unsigned char SendMessage(void)
{
	//InitPLL();
	int i;
	int j=0;
	for(i=0;i<SIZE_OBCTIME;i++)			message_fm[j++] = status.obctime[i];
	for(i=0;i<SIZE_VOLTAGE;i++)			message_fm[j++] = status.voltage[i];
	for(i=0;i<SIZE_AMPARE;i++)			message_fm[j++] = status.ampare[i];
	for(i=0;i<SIZE_TEMPERATURE;i++)		message_fm[j++] = status.temperature[i];
	for(i=0;i<SIZE_GYRO;i++)			message_fm[j++] = status.gyro[i];
	for(i=0;i<SIZE_MAGNET;i++)			message_fm[j++] = status.magnet[i];
	for(i=0;i<SIZE_RSSI;i++)			message_fm[j++] = status.rssi[i];
	for(i=0;i<SIZE_MISSION;i++)			message_fm[j++] = status.mission[i];
	for(i=0;i<SIZE_POWER;i++)			message_fm[j++] = status.power[i];
	for(i=0;i<SIZE_INIT;i++)			message_fm[j++] = status.init[i];
	for(i=0;i<SIZE_MODE;i++)			message_fm[j++] = status.mode[i];
	for(i=0;i<SIZE_CHARGEFLAG;i++)		message_fm[j++] = status.chargeflag[i];
	for(i=0;i<SIZE_FLAG;i++)			message_fm[j++] = status.flag[i];
	for(i=0;i<SIZE_ERROR;i++)			message_fm[j++] = status.error_counter[i];
	for(i=0;i<SIZE_MESSAGE;i++)			message_fm[j++] = status.message_counter[i];
	
	send_packet(message_fm, SIZE_STATUS);
	
	return 1;
}

/*************************************************
		データ送信完了通知関数
*************************************************/
unsigned char FinishSendMessage(void)
{	
	FMDTSW=0;
	FMCWSW 	= 0;//1
	fmptt_off();
	
	sci0_puts("c-p-c-txs \r\n");
	debug_puts("Send TXS\n");
	flag_tof=1;
	
	return 1;
}

/*************************************************
		データ送信終了関数
*************************************************/
unsigned char FinishCommunication(void)
{	
	FX_RDYN	= 0;
	FX_CLK	= 0;
	FX_TXD	= 0;
	FX_M1	= 0;
	
	FMSTB	= 0;
	FMDAT	= 0;
	FMCLK	= 0;
	FMPTT	= 0;
	FMCWSW 	= 0;//1
	flag_tof=0;
	if(DT_mode!=0){
		sci0_puts("c-p-c-cwp \r\n");
		FM_mode=0;
		debug_puts("Finish Send Message.\n");
	}
	else{
		DT_mode=0;
		sci0_puts("c-p-c-cwp \r\n");
		FM_mode=0;
		debug_puts("Finish Send Message.\n");
	}
	return 1;
/*	if((DT_mode!=1)||(DT_mode!=4)){
		DT_mode=0;
	}	
	else if(DT_mode==4){
		DT_mode=0;
		Ax_Send("r-g-c-dof \r\n",12);	
		sci0_puts("c-p-c-cwp \r\n");
		debug_puts("Finish Send DT Message.\n");
		return 1;
	}
	sci0_puts("c-p-c-cwp \r\n");
	debug_puts("Finish Send Message.\n");
	
	return 1;
*/
}

/*************************************************
		　　　ステータス通信関数
*************************************************/
unsigned char SendStatusMessage(void)
{
	if(FX_RDYN != 0){
		debug_puts("\n TX's RDYN error!!! \n");
		return 0;
	}
	//FMPTT = 0;
	fmptt_on();
	int i;
	
	for (i=0; i<MESSAGE_GLOBAL_LENGTH; i++) {
		message_global[i] = 0x00;
	}
	
	int j=0;
	message_global[j++] = 'r';
	message_global[j++] = '-';
	message_global[j++] = 'c';
	message_global[j++] = '-';
	message_global[j++] = 'g';
	message_global[j++] = '-';
	message_global[j++] = 's';
	message_global[j++] = 't';
	message_global[j++] = 'a';
	message_global[j++] = '-';
	
	for(i=0;i<SIZE_OBCTIME;i++)			message_global[j++] = status.obctime[i];
	for(i=0;i<SIZE_VOLTAGE;i++)			message_global[j++] = status.voltage[i];
	for(i=0;i<SIZE_AMPARE;i++)			message_global[j++] = status.ampare[i];
	for(i=0;i<SIZE_TEMPERATURE;i++)		message_global[j++] = status.temperature[i];
	for(i=0;i<SIZE_GYRO;i++)			message_global[j++] = status.gyro[i];
	for(i=0;i<SIZE_MAGNET;i++)			message_global[j++] = status.magnet[i];
	for(i=0;i<SIZE_RSSI;i++)			message_global[j++] = status.rssi[i];
	for(i=0;i<SIZE_MISSION;i++)			message_global[j++] = status.mission[i];
	for(i=0;i<SIZE_POWER;i++)			message_global[j++] = status.power[i];
	for(i=0;i<SIZE_INIT;i++)			message_global[j++] = status.init[i];
	for(i=0;i<SIZE_MODE;i++)			message_global[j++] = status.mode[i];
	for(i=0;i<SIZE_CHARGEFLAG;i++)		message_global[j++] = status.chargeflag[i];
	for(i=0;i<SIZE_FLAG;i++)			message_global[j++] = status.flag[i];
	for(i=0;i<SIZE_ERROR;i++)			message_global[j++] = status.error_counter[i];
	for(i=0;i<SIZE_MESSAGE;i++)			message_global[j++] = status.message_counter[i];
	
	//デバッグ用
	
	debug_puts("Send Status\n");
	/*
	for(i=0; i<MESSAGE_GLOBAL_LENGTH; i++) {
		debug_putdec(message_global[i]);
	}
	*/
	AX_protocol(message_global, MESSAGE_GLOBAL_LENGTH);
	debug_puts("\n");
	FMDTSW=0;
	fmptt_off();
	
	/*
	FMPTT = 1;
	*/
	sci0_puts("c-p-c-txs \r\n");
	debug_puts("Send TXS\n");

	
	return 1;
}

/*************************************************
		パワーから返事が返って来ているかの確認
*************************************************/
unsigned char CheckPowerReply(void){
	if(flag_ton==1){
		sci0_puts("c-p-c-txr \r\n");
	}
	if(flag_tof==1){
		sci0_puts("c-p-c-txs \r\n");
	}
}


		