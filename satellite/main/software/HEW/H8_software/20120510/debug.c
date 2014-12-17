/*************************************************************************************
* ファイル名　：　debug.c
* タイトル　　：　INVADERデバッグコマンドプログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.19
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "debug.h"
#include "global.h"
#include "sci.h"
#include "iic.h"
#include "cdh_pindefine.h"

#include "message.h"
#include "pwr.h"
#include "mission.h"
#include "adconv.h"
#include "teleme.h"
#include "itu1.h"
#include "teleme.h"

/*************************************************
        マルチプレクサ動作確認関数
*************************************************/
unsigned char DebugMultiplexer(void)
{
	unsigned char i;
	
	debug_puts("\n");
	debug_puts("AN0: ");
	for(i=0;i<8;i++)
	{
		debug_putdec(mpx[0][i]);
		debug_puts(" ");
	}
	debug_puts("\n");
	debug_puts("AN1: ");
	for(i=0;i<8;i++)
	{
		debug_putdec(mpx[1][i]);
		debug_puts(" ");
	}
	debug_puts("\n");
	debug_puts("AN2: ");
	for(i=0;i<8;i++)
	{
		debug_putdec(mpx[2][i]);
		debug_puts(" ");
	}
	debug_puts("\n");
	debug_puts("AN3: ");
	for(i=0;i<1;i++)
	{
		debug_putdec(mpx[3][i]);
		debug_puts(" ");
	}
	debug_puts("\n");
	
	return 1;
}

/*************************************************
		デバッグ用ステータスコマンド
*************************************************/
unsigned char DebugPut_Status()
{
		
	unsigned char i,j,cnt;
	unsigned char sta[128];
	j=0;
	
	for(i=0;i<SIZE_OBCTIME;i++)			sta[j++] = status.obctime[i];
	for(i=0;i<SIZE_VOLTAGE;i++)			sta[j++] = status.voltage[i];
	for(i=0;i<SIZE_AMPARE;i++)			sta[j++] = status.ampare[i];
	for(i=0;i<SIZE_TEMPERATURE;i++)		sta[j++] = status.temperature[i];
	for(i=0;i<SIZE_GYRO;i++)			sta[j++] = status.gyro[i];
	for(i=0;i<SIZE_MAGNET;i++)			sta[j++] = status.magnet[i];
	for(i=0;i<SIZE_RSSI;i++)			sta[j++] = status.rssi[i];
	for(i=0;i<SIZE_MISSION;i++)			sta[j++] = status.mission[i];
	for(i=0;i<SIZE_POWER;i++)			sta[j++] = status.power[i];
	for(i=0;i<SIZE_INIT;i++)			sta[j++] = status.init[i];
	for(i=0;i<SIZE_MODE;i++)			sta[j++] = status.mode[i];
	for(i=0;i<SIZE_CHARGEFLAG;i++)		sta[j++] = status.chargeflag[i];
	for(i=0;i<SIZE_FLAG;i++)			sta[j++] = status.flag[i];
	for(i=0;i<SIZE_ERROR;i++)			sta[j++] = status.error_counter[i];
	for(i=0;i<SIZE_MESSAGE;i++)			sta[j++] = status.message_counter[i];
	
	debug_puts("\n");
	for(i=0;i<SIZE_STATUS;i++){
		/*
		debug_puts("status[");
		debug_putdec(i);
		debug_puts("]: ");
		*/
		debug_putdec(sta[i]);
		debug_puts(" ");
	}
	
	return 1;
}

// IIC通信接続先
#define IIC_0	0
#define IIC_1	1
#define IIC_2 	2
// 共有メモリ接続先
#define MAIN	0	// MainCPU
#define MISSION	1	// MissionCPU
/*************************************************
        デバッグ用EEPROM書込/読出関数
-------------------------------------------------
	param[0] 			:ROM_CH
	param[1] 			:SLAVE_ADDRESS
	
	 param[2]*100000
	+param[3]*10000
	+param[4]*1000
	+param[5]*100
	+param[6]*10
	+param[7]			:Write/Read page
	
	param[8]~			:Write data
*************************************************/
unsigned char DebugEEPROM(volatile unsigned char* param,unsigned char param_length)
{
	// ROM_CH (0:TLM, 1:RPR, 2:SHR) 
	unsigned char ch = (unsigned char)(param[0]-48);
	// SLAVE_AD (0~3)
	unsigned char s_ad = (unsigned char)(param[1]-48);
	// PAGE(param[2]~param[7]　:6ケタ)
	unsigned long int page =  (unsigned long int)(param[2]-48)*100000
						     +(unsigned long int)(param[3]-48)*10000
							 +(unsigned long int)(param[4]-48)*1000
							 +(unsigned long int)(param[5]-48)*100
							 +(unsigned long int)(param[6]-48)*10
							 +(unsigned long int)(param[7]-48)*1;
	// 書き込むデータ(param[8]以降)
	unsigned char *dat = (unsigned char*)&param[8];
	
	// EEPROM読み出しデータ用バッファ
	static unsigned char buf[MODE128];
	
	// for debug
	debug_puts("\n");
	debug_puts("CH:");		debug_putdec((unsigned long)ch);	debug_puts("\n");
	debug_puts("SLAVE:");	debug_putdec((unsigned long)s_ad);	debug_puts("\n");
	debug_puts("PAGE:");	debug_putdec((unsigned long)page);	debug_puts("\n");
	debug_puts("DATA:");	debug_puts((unsigned char*)dat);	debug_puts("\n");
	
	switch(ch){
		case IIC_0:
			IIC0_EEPRESET();
			IIC0_EEPSTOP();
			// 書き込む
			debug_puts("\nWriting...\n");
			IIC0_EEPWRITE(page,dat,s_ad,MODE128);
			// 読み出す
			debug_puts("Reading...\n");
			IIC0_EEPREAD(page,buf,s_ad,MODE128);
			// 読み出したデータの表示
			debug_puts("READ_DAT:");
			debug_puts(buf);
			debug_puts("\n");
			debug_puts("Finish reading.\n\n");
		break;
		
		case IIC_1:
			IIC1_EEPRESET();
			IIC1_EEPSTOP();
			// 書き込む
			debug_puts("\nWriting...\n");
			IIC1_EEPWRITE(page,dat,s_ad,MODE128);
			// 読み出す
			debug_puts("Reading...\n");
			IIC1_EEPREAD(page,buf,s_ad,MODE128);
			// 読み出したデータの表示
			debug_puts("READ_DAT:");
			debug_puts(buf);
			debug_puts("\n");
			debug_puts("Finish reading.\n\n");
		break;
		
		// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
		unsigned char SharedMemoryEnBuffer;
		unsigned char SharedMemoryChBuffer;
		
		case IIC_2:
			SharedMemoryEnBuffer = SROM_EN;	// 一時退避
			SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
			SROM_EN = 1;	// SharedMemory許可
			SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
			debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ
			//for debug
			debug_puts("EN:");
			debug_putdec(SROM_EN);
			debug_puts("\n");
			debug_puts("CH:");
			debug_putdec(SROM_CH);
			debug_puts("\n");
			//end of debug
			
			IIC2_EEPRESET();
			IIC2_EEPSTOP();
			// 書き込む
			debug_puts("\nWriting...\n");
			IIC2_EEPWRITE(page,dat,s_ad,MODE128);
			// 読み出す
			debug_puts("Reading...\n");
			IIC2_EEPREAD(page,buf,s_ad,MODE128);
			// 読み出したデータの表示
			debug_puts("READ_DAT:");
			debug_puts(buf);
			debug_puts("\n");
			debug_puts("Finish reading.\n\n");
			
			// Enable,CHを元に戻す
			SROM_EN = SharedMemoryEnBuffer;
			SROM_CH = SharedMemoryChBuffer;
			
			//for debug
			debug_puts("Re Switch & Enable SharedMemory.\n");
			debug_puts("EN:");
			debug_putdec(SROM_EN);
			debug_puts("\n");
			debug_puts("CH:");
			debug_putdec(SROM_CH);
			debug_puts("\n");
			//end of debug
		break;
		
		default:
		break;
	}
	
	return 1;
}

/*************************************************
		デバッグ用粗履歴読み出しコマンド
*************************************************/
unsigned char DebugPut_Rough(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char slave = 0;
	// param の最初の6ケタは読み出しはじめのページ数
	unsigned long int param0 = (unsigned long int)param[0]-48;
	unsigned long int param1 = (unsigned long int)param[1]-48;
	unsigned long int param2 = (unsigned long int)param[2]-48;
	unsigned long int param3 = (unsigned long int)param[3]-48;
	unsigned long int param4 = (unsigned long int)param[4]-48;
	unsigned long int param5 = (unsigned long int)param[5]-48;
	unsigned long int start_page = 100000*param0 + 10000*param1 + 1000*param2 + 100*param3 + 10*param4 + param5;
	while(start_page >= PAGE_PER_EEPROM_MODE128) {
		start_page = start_page - PAGE_PER_EEPROM_MODE128;
		slave++;
	}
	
	// param の7ケタ目が読み出すページ数
	unsigned long int range = (unsigned long int)param[6]-48;
	
	// for debug
	debug_puts("Rough data");
	debug_puts("\n");
	debug_puts((unsigned char*)"param = ");
	debug_puts((unsigned char*)param);
	debug_puts("\n");
	debug_puts("start_page = ");
	debug_putdec(start_page);
	debug_puts("\n");
	debug_puts("range = ");
	debug_putdec(range);
	debug_puts("\n");
	
	unsigned long int page;
	unsigned char i;
	static unsigned char tlm[MODE128];		// EEPROM読込バッファ 128Byte/page
	
	debug_puts("Reading...\n");
	
	for(page = start_page; page < start_page+range; page++)
	{	
		IIC1_EEPRESET();
		IIC1_EEPSTOP();
		IIC1_EEPREAD(page,tlm,slave,MODE128);
		;;
		debug_puts("\n");
		debug_puts("---------------page:");
		debug_putdec(page);
		debug_puts("---------------\n");		
		
		for(i=0;i<SIZE_STATUS;i++){
			debug_puts("tlm[");
			debug_putdec(i);
			debug_puts("]: ");
			debug_putdec(tlm[i]);
			debug_puts("\n");
		}
		
		// tlm[] をASCIIで表示
		debug_puts("text:");
		debug_puts(tlm);
		debug_puts("\n");
	}
	//end of debug
	
	debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		デバッグ用詳細履歴読み出しコマンド
*************************************************/
unsigned char DebugPut_Detail(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char slave = 0;
	// param の最初の6ケタは読み出しはじめのページ数
	unsigned long int param0 = (unsigned long int)param[0]-48;
	unsigned long int param1 = (unsigned long int)param[1]-48;
	unsigned long int param2 = (unsigned long int)param[2]-48;
	unsigned long int param3 = (unsigned long int)param[3]-48;
	unsigned long int param4 = (unsigned long int)param[4]-48;
	unsigned long int param5 = (unsigned long int)param[5]-48;
	unsigned long int start_page = 100000*param0 + 10000*param1 + 1000*param2 + 100*param3 + 10*param4 + param5;
	while(start_page >= PAGE_PER_EEPROM_MODE128) {
		start_page = start_page - PAGE_PER_EEPROM_MODE128;
		slave++;
	}
	
	// param の7ケタ目が読み出すページ数
	unsigned long int range = (unsigned long int)param[6]-48;
	
	// for debug
	debug_puts("Detailed data");
	debug_puts("\n");
	debug_puts((unsigned char*)"param = ");
	debug_puts((unsigned char*)param);
	debug_puts("\n");
	debug_puts("start_page = ");
	debug_putdec(start_page);
	debug_puts("\n");
	debug_puts("range = ");
	debug_putdec(range);
	debug_puts("\n");
	
	unsigned long int page;
	unsigned char i;
	static unsigned char tlm[MODE128];		// EEPROM読込バッファ 128Byte/page
	
	debug_puts("Reading...\n");
	
	for(page = start_page; page < start_page+range; page++)
	{	
		IIC0_EEPRESET();
		IIC0_EEPSTOP();
		IIC0_EEPREAD(page,tlm,slave,MODE128);
		;;
		debug_puts("\n");
		debug_puts("---------------page:");
		debug_putdec(page);
		debug_puts("---------------\n");		
		
		for(i=0;i<SIZE_STATUS;i++){
			debug_puts("tlm[");
			debug_putdec(i);
			debug_puts("]: ");
			debug_putdec(tlm[i]);
			debug_puts("\n");
		}
		
		// tlm[] をASCIIで表示
		debug_puts("text:");
		debug_puts(tlm);
		debug_puts("\n");
	}
	//end of debug
	
	debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		デバッグ用API配信データ読み出しコマンド
*************************************************/
unsigned char DebugPut_API(volatile unsigned char* param, unsigned char param_length)
{
	// param の最初の6ケタは読み出しはじめのページ数
	unsigned long int param0 = (unsigned long int)param[0]-48;
	unsigned long int param1 = (unsigned long int)param[1]-48;
	unsigned long int param2 = (unsigned long int)param[2]-48;
	unsigned long int param3 = (unsigned long int)param[3]-48;
	unsigned long int param4 = (unsigned long int)param[4]-48;
	unsigned long int param5 = (unsigned long int)param[5]-48;
	unsigned long int start_page = 100000*param0 + 10000*param1 + 1000*param2 + 100*param3 + 10*param4 + param5;
	
	// param の7ケタ目が読み出すページ数
	unsigned long int range = (unsigned long int)param[6]-48;
	
	// for debug
	debug_puts("API data");
	debug_puts("\n");
	debug_puts((unsigned char*)"param = ");
	debug_puts((unsigned char*)param);
	debug_puts("\n");
	debug_puts("start_page = ");
	debug_putdec(start_page);
	debug_puts("\n");
	debug_puts("range = ");
	debug_putdec(range);
	debug_puts("\n");
	
	unsigned long int page;
	unsigned char i;
	static unsigned char tlm[MODE32];
	
	debug_puts("Reading...\n");
	
	for(page = start_page; page < start_page+range; page++)
	{	
		IIC1_EEPRESET();
		IIC1_EEPSTOP();
		IIC1_EEPREAD(page,tlm,API_SLAVE,MODE32);
		;;
		debug_puts("\n");
		debug_puts("---------------page:");
		debug_putdec(page);
		debug_puts("---------------\n");		
		
		for(i=0;i<MODE32;i++){
			debug_puts("tlm[");
			debug_putdec(i);
			debug_puts("]: ");
			debug_putdec(tlm[i]);
			debug_puts("\n");
		}
		
		// tlm[] をASCIIで表示
		debug_puts("text:");
		debug_puts(tlm);
		debug_puts("\n");
	}
	//end of debug
	
	debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		デバッグ用初期化情報読み出しコマンド
*************************************************/
unsigned char DebugPut_Initial()
{
	// for debug
	debug_puts("Initial data");
	debug_puts("\n");
	
	unsigned char i;
	static unsigned char buf_obc[MODE32];
	static unsigned char buf_error[MODE32];
	
	debug_puts("Reading...\n");
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPREAD(OBCTIME_PAGE,buf_obc,INITIAL_SLAVE,MODE32);
	;;
	debug_puts("\n");
	debug_puts("---------------page: 0x");
	debug_puthex(OBCTIME_PAGE);
	debug_puts("---------------\n");		
	
	for(i=0;i<SIZE_OBCTIME*3;i++){
		debug_puts("tlm[");
		debug_putdec(i);
		debug_puts("]: ");
		debug_putdec(buf_obc[i]);
		debug_puts("\n");
	}
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPREAD(ERRORCOUNT_PAGE,buf_error,INITIAL_SLAVE,MODE32);
	;;
	debug_puts("\n");
	debug_puts("---------------page: 0x");
	debug_puthex(ERRORCOUNT_PAGE);
	debug_puts("---------------\n");		
	
	for(i=0;i<SIZE_ERROR*3;i++){
		debug_puts("tlm[");
		debug_putdec(i);
		debug_puts("]: ");
		debug_putdec(buf_error[i]);
		debug_puts("\n");
	}
	debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		デバッグ用共有メモリ読み出しコマンド
*************************************************/
unsigned char DebugPut_SharedROM(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char slave = 0;
	// param の最初の6ケタは読み出しはじめのページ数
	unsigned long int param0 = (unsigned long int)param[0]-48;
	unsigned long int param1 = (unsigned long int)param[1]-48;
	unsigned long int param2 = (unsigned long int)param[2]-48;
	unsigned long int param3 = (unsigned long int)param[3]-48;
	unsigned long int param4 = (unsigned long int)param[4]-48;
	unsigned long int param5 = (unsigned long int)param[5]-48;
	unsigned long int start_page = 100000*param0 + 10000*param1 + 1000*param2 + 100*param3 + 10*param4 + param5;
	while(start_page >= PAGE_PER_EEPROM_MODE128) {
		start_page = start_page - PAGE_PER_EEPROM_MODE128;
		slave++;
	}
	
	// param の7ケタ目が読み出すページ数
	unsigned long int range = (unsigned long int)param[6]-48;
	
	// for debug
	debug_puts("SharedROM data");
	debug_puts("\n");
	debug_puts((unsigned char*)"param = ");
	debug_puts((unsigned char*)param);
	debug_puts("\n");
	debug_puts("start_page = ");
	debug_putdec(start_page);
	debug_puts("\n");
	debug_puts("range = ");
	debug_putdec(range);
	debug_puts("\n");
	
	unsigned long int page;
	unsigned char i;
	static unsigned char tlm[MODE128];		// EEPROM読込バッファ 128Byte/page
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
	debug_puts("Reading...\n");
	
	for(page = start_page; page < start_page+range; page++)
	{	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		IIC2_EEPREAD(page,tlm,slave,MODE128);
		;;
		debug_puts("\n");
		debug_puts("---------------page:");
		debug_putdec(page);
		debug_puts("---------------\n");		
		
		for(i=0;i<MODE128;i++){
			debug_puts("tlm[");
			debug_putdec(i);
			debug_puts("]: ");
			debug_putdec(tlm[i]);
			debug_puts("\n");
		}
		
		// tlm[] をASCIIで表示
		debug_puts("text:");
		debug_puts(tlm);
		debug_puts("\n");
	}
	//end of debug
	
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	
	debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		デバッグ用共有メモリ書き込み関数
*************************************************/
unsigned char Debug_SharedROM(unsigned long int page, unsigned char *dat, unsigned char s_ad, unsigned char byte)
{	
	// EEPROM読み出しデータ用バッファ
	static unsigned char buf[128];
	
	// for debug
	debug_puts("\n");
	debug_puts("SLAVE:");	debug_putdec((unsigned long)s_ad);	debug_puts("\n");
	debug_puts("PAGE:");	debug_putdec((unsigned long)page);	debug_puts("\n");

		
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ
	//for debug
	debug_puts("EN:");
	debug_putdec(SROM_EN);
	debug_puts("\n");
	debug_puts("CH:");
	debug_putdec(SROM_CH);
	debug_puts("\n");
	//end of debug
		
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	// 書き込む
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(page,dat,s_ad,byte);
	// 読み出す
	debug_puts("Reading...\n");
	IIC2_EEPREAD(page,buf,s_ad,byte);
	// 読み出したデータの表示
	debug_puts("READ_DAT:");
	debug_puts(buf);
	debug_puts("\n");
	debug_puts("Finish reading.\n\n");
			
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	
	//for debug
	debug_puts("Re Switch & Enable SharedMemory.\n");
	debug_puts("EN:");
	debug_putdec(SROM_EN);
	debug_puts("\n");
	debug_puts("CH:");
	debug_putdec(SROM_CH);
	debug_puts("\n");
	//end of debug

	return 1;
}
	

#define STARTUP 	0
#define OBC 		1
#define MODE 		2
#define CAMERA_1 	3
#define CAMERA_2 	4
#define DIGI_PARAM 	5
#define LED_1 		6
#define LED_2 		7
#define LED_3 		8
#define TEXT1_1 	9
#define TEXT1_2 	10
#define TEXT1_3 	11
#define TEXT2_1 	12
#define TEXT2_2 	13
#define TEXT2_3 	14
#define DIGI_TEXT_1 15
#define DIGI_TEXT_2 16
#define DIGI_TEXT_3 17
/*************************************************
		デバッグ用共有メモリ書き込みコマンド
*************************************************/
unsigned char DebugWrite_SharedROM(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char byte = 128;
	// ROM_CH (0:TLM, 1:RPR, 2:SHR) 
	unsigned char ch = (unsigned char)2;
	// SLAVE_AD (0~3)
	unsigned char s_ad = (unsigned char)0;
	// PAGE
	unsigned long int page =  (unsigned long int)0;
	// CHANGE_NUM(param[0])
	//0:Startup times 1:OBCtime 2:MODE 3:CAMERA PARAM 4:Digitalker Param 5:LED Param 6:Text 7:Text2 8:Digitalker Text)
	unsigned long int param0 = (unsigned long int)param[0]-48;
	unsigned long int param1 = (unsigned long int)param[1]-48;
	unsigned long int change_num = param0*10 + param1;
	debug_puts("CHANGE_NUM:");
	debug_putdec(change_num);
	debug_puts("\n");
	//CHANGE_DATA  書き込むデータ(param[1]以降)
	unsigned char *change_dat = (unsigned char*)&param[2];
	debug_puts("DATA:");	debug_puts(change_dat);	debug_puts("\n");
	unsigned char dat[128];
	int i, j;
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	IIC2_EEPREAD(page,dat,s_ad,byte);   // device = 0
	;;
	
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	
	switch(change_num) {
		//0ページ目の内容
		case STARTUP:
			j = 0;
			page = 0;
			for (i = 0; i < 1; i++) {
				dat[i] = change_dat[j++];
			}
			//debug_puts("check_out_1\n");
			//debug_puts("check_out_2\n");
		break;
		case OBC:
			j = 0;
			page = 0;
			for (i = 1; i < 5; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case MODE:
			j = 0;
			page = 0;
			for (i = 5; i < 6; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case CAMERA_1:
			j = 0;
			page = 0;
			for (i = 6; i < 54; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case CAMERA_2:
			j = 0;
			page = 0;
			for (i = 54; i < 70; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case DIGI_PARAM:
			j = 0;
			page = 0;
			for (i = 70; i < 102; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		//1ページ目の内容
		case LED_1:
			j = 0;
			page = 1;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case LED_2:
			j = 0;
			page = 1;
			for (i = 48; i < 96; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case LED_3:
			j = 0;
			page = 1;
			for (i = 96; i < 128; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		//2, 3ページ目の内容
		case TEXT1_1:
			j = 0;
			page = 2;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case TEXT1_2:
			j = 0;
			page = 2;
			for (i = 48; i < 96; i++) {
				dat[i] = change_dat[j++];
			}
			
		case TEXT1_3:
			j = 0;
			page = 3;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		//4, 5ページ目の内容
		case TEXT2_1:
			j = 0;
			page = 4;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case TEXT2_2:
			page = 4;
			j = 0;
			for (i = 48; i < 96; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case TEXT2_3:
			page = 5;
			j = 0;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		//6, 7ページ目の内容
		case DIGI_TEXT_1:
			j = 0;
			page = 6;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		case DIGI_TEXT_2:
			j = 0;
			page = 6;
			for (i = 48; i < 96; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		case DIGI_TEXT_3:
			j = 0;
			page = 7;
			for (i = 0; i < 48; i++) {
				dat[i] = change_dat[j++];
			}
			
		break;
		default:
		break;	
	}
	
	Debug_SharedROM(page, dat, s_ad, byte);
		
	return 1;
}

/*************************************************
		ルーチンDIS/EN表示関数
*************************************************/
unsigned char DisplayRoutine(void)
{
	debug_puts("\n-------------Routine EN/DIS-------------\n");
	
	debug_puts("01.pwrt:	");
	if(CheckFlags(ena_pwrt))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("02.misst:	");
	if(CheckFlags(ena_misst))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("03.ad:	");
	if(CheckFlags(ena_ad))		debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("04.rteleme:	");
	if(CheckFlags(ena_rteleme))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("05.dteleme:	");
	if(CheckFlags(ena_dteleme))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("06.ateleme:	");
	if(CheckFlags(ena_ateleme))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("07.mywdt:	");
	if(CheckFlags(ena_mywdt))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("08.overa:	");
	if(CheckFlags(ena_overa))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("09.iteleme:	");
	if(CheckFlags(ena_iteleme))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("10.alive:	");
	if(CheckFlags(ena_alive))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	debug_puts("11.dispAD:	");
	if(CheckFlags(ena_dispAD))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	return 1;
	
	debug_puts("12.oteleme:	");
	if(CheckFlags(ena_oteleme))	debug_puts("EN\n");
	else						debug_puts("DIS\n");
	
	return 1;
	
}

#define PWRT	1
#define MISST	2
#define ADC		3
#define RTELEME	4
#define DTELEME	5
#define ATELEME	6
#define MYWDT	7
#define OVERA	8
#define ITELEME	9
#define ALIVE   10
#define DISP_AD	11
#define OTELEME 12
/*************************************************
		ルーチンDIS/EN切替関数
*************************************************/
unsigned char SetRoutine(volatile unsigned char* param)
{
	unsigned char RoutineID = (param[0] - 48)*10
							+ (param[1] - 48);
	
	unsigned char EN = param[2] - 48;
	
	if(EN == 1){
		switch(RoutineID){
			
			case PWRT:
			SetFlags(ena_pwrt);
			break;
			
			case MISST:
			SetFlags(ena_misst);
			break;
			
			case ADC:
			SetFlags(ena_ad);
			break;
			
			case RTELEME:
			SetFlags(ena_rteleme);
			break;
			
			case DTELEME:
			SetFlags(ena_dteleme);
			break;
			
			case ATELEME:
			SetFlags(ena_ateleme);
			break;
			
			case MYWDT:
			SetFlags(ena_mywdt);
			break;
			
			case OVERA:
			SetFlags(ena_overa);
			break;
			
			case ITELEME:
			SetFlags(ena_iteleme);
			break;
			
			case ALIVE:
			SetFlags(ena_alive);
			break;
			
			case DISP_AD:
			SetFlags(ena_dispAD);
			break;
			
			case OTELEME:
			SetFlags(ena_oteleme);
			break;
			
			default:
			debug_puts("error: Routine is not existent.\n");
			break;
		}
		debug_puts("Routine has set up:\n");
		DisplayRoutine();
	}
	
	else if(EN == 0){
		switch(RoutineID){
			
			case PWRT:
			ClearFlags(ena_pwrt);
			break;
			
			case MISST:
			ClearFlags(ena_misst);
			break;
			
			case ADC:
			ClearFlags(ena_ad);
			break;
			
			case RTELEME:
			ClearFlags(ena_rteleme);
			break;
			
			case DTELEME:
			ClearFlags(ena_dteleme);
			break;
			
			case ATELEME:
			ClearFlags(ena_ateleme);
			break;
			
			case MYWDT:
			ClearFlags(ena_mywdt);
			break;
			
			case OVERA:
			ClearFlags(ena_overa);
			break;
			
			case ITELEME:
			ClearFlags(ena_iteleme);
			break;
			
			case ALIVE:
			ClearFlags(ena_alive);
			break;
			
			case DISP_AD:
			ClearFlags(ena_dispAD);
			break;
			
			case OTELEME:
			ClearFlags(ena_oteleme);
			break;
			
			default:
			debug_puts("error: Routine is not existent.\n");
			break;
		}
		debug_puts("Routine has set up:\n");
		DisplayRoutine();
		
	}
	
	else{
		debug_puts("error: param[1] must be 0 or 1.\n");
	}
}



/*************************************************
		PowerCPUセンサデータチェック関数
*************************************************/
unsigned char Check_PowerData()
{
	debug_puts("Ampare Data:\n");
	
	unsigned char i;
	for (i=0; i<SIZE_AMPARE; i++) {
		debug_puts("ampare[");
		debug_putdec(i);
		debug_puts("]:");
		debug_putdec(status.ampare[i]);
		debug_puts("\n");
	}
	
	debug_puts("finish\n");

}

/*************************************************
		生存信号停止
*************************************************/
unsigned char StopAlivePeriod(){
	debug_puts("\n !!!!!! Stop alive period !!!!!! \n");
	ClearFlags(ena_alive);
}