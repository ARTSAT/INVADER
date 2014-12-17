/*************************************************************************************
* ファイル名　：　teleme.c
* タイトル　　：　INVADERテレメトリセットプログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.31
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* 
*************************************************************************************/
#include "iodefine.h"
#include "iic.h"
#include "teleme.h"
#include "adconv.h"
#include "sci.h"
#include "global.h"
#include "itu1.h"
#include "cdh_pindefine.h"

#define MAIN	0	// MainCPU


/*************************************************
         テレメトリ書き込み初期化関数
*************************************************/
unsigned char InitTelemetry(void)
{
	ClearFlags(flag_rteleme);
	ClearFlags(flag_dteleme);
	ClearFlags(flag_ateleme);
	ClearFlags(flag_iteleme);
	ClearFlags(flag_oteleme);
	
	return 1;
}

/*************************************************
		テレメトリ書き込み関数(粗履歴)
*************************************************/
unsigned char SetTelemetry_Rough(void)
{
	unsigned char i;
	unsigned char j = 0;
//	static unsigned char rough_slave;
//	static unsigned long int rough_eep_page;
	static unsigned char data_set[MODE128];
	
	for(i=0; i<SIZE_OBCTIME; i++)		data_set[j++] = status.obctime[i];
	for(i=0; i<SIZE_VOLTAGE; i++)		data_set[j++] = status.voltage[i];
	for(i=0; i<SIZE_AMPARE; i++)		data_set[j++] = status.ampare[i];
	for(i=0; i<SIZE_TEMPERATURE; i++)	data_set[j++] = status.temperature[i];
	for(i=0; i<SIZE_GYRO; i++)			data_set[j++] = status.gyro[i];
	for(i=0; i<SIZE_MAGNET; i++)		data_set[j++] = status.magnet[i];
	for(i=0; i<SIZE_RSSI; i++)			data_set[j++] = status.rssi[i];
	for(i=0; i<SIZE_MISSION; i++)		data_set[j++] = status.mission[i];
	for(i=0; i<SIZE_POWER; i++)			data_set[j++] = status.power[i];
	for(i=0; i<SIZE_INIT; i++)			data_set[j++] = status.init[i];
	for(i=0; i<SIZE_MODE; i++)			data_set[j++] = status.mode[i];
	for(i=0; i<SIZE_CHARGEFLAG; i++)	data_set[j++] = status.chargeflag[i];
	for(i=0; i<SIZE_FLAG; i++)			data_set[j++] = status.flag[i];
	for(i=0; i<SIZE_ERROR; i++)			data_set[j++] = status.error_counter[i];
	for(i=0; i<SIZE_MESSAGE; i++)		data_set[j++] = status.message_counter[i];
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	
	IIC1_EEPWRITE(rough_eep_page, data_set, rough_slave, MODE128);
	/*
	// codes for debug
	debug_puts("I've written Rough tlm at.\r\n");
	debug_puts("IIC1, ");
	debug_puts("slave ");
	debug_putdec(slave);
	debug_puts(", ");
	debug_puts("page ");
	debug_putdec(eep_page);
	debug_puts("\r\n\r\n");
	
	for(i=0;i<SIZE_STATUS;i++){
		debug_puts("tlm[");
		debug_putdec(i);
		debug_puts("]: ");
		debug_putdec(data_set[i]);
		debug_puts("\r\n");
	}
	*/
	debug_puts("\n Write Rough\t");
	debug_putdec((rough_eep_page+rough_slave*1024)*128);
	debug_puts("\ttime: ");
	debug_putdec((data_set[3] & 0x00ff)+((data_set[2] << 8) & 0xff00)+((data_set[1] << 16) & 0x00ff0000)+((data_set[0] << 24) & 0xff000000));
	debug_puts("\n");
	
	// end of debug codes            
	
	rough_eep_page++;
	if(rough_eep_page > PAGE_PER_EEPROM_MODE128) {// 1つのEEPROMの最後まで書ききったら	
		rough_slave++;							// 次のEEPROMの最初のページに移る
		rough_eep_page = 0;
		
		if(rough_slave>2){	// IIC1上のスレーブアドレス2のEEPROMの最後まで書ききったら
			rough_slave=0;	// IIC1上のスレーブアドレス0のEEPROMの最初に戻る
		}else;
	}else;
	
	return 1;
}

unsigned char Setup_DTelemetry(volatile unsigned char* param, unsigned char param_length){
	unsigned char set_num = param[0] - 48;
	if(set_num==0){
		SetTelemetry_Detail(0);
		SetFlags(ena_dteleme);
		debug_puts("\n Start Dteleme\n");
	}
	else if(set_num==1){
		ClearFlags(ena_dteleme);
		debug_puts("\n Stop Dteleme\n");
	}
	else{
		debug_puts("Setup_DTelemetry param error\n");
	}
}
		

/*************************************************
		テレメトリ書き込み関数(精履歴)
*************************************************/
unsigned char SetTelemetry_Detail(unsigned char mode)
{
	unsigned char i;
	unsigned char j = 0;
	static unsigned char slave;
	static unsigned long int eep_page;
	static unsigned char data_set[MODE128];
	
	if(mode==0){
		slave=0;
		eep_page=0;
		debug_puts("\n Reset Dteleme address \n");
	}
	
	for(i=0; i<SIZE_OBCTIME; i++)		data_set[j++] = status.obctime[i];
	for(i=0; i<SIZE_VOLTAGE; i++)		data_set[j++] = status.voltage[i];
	for(i=0; i<SIZE_AMPARE; i++)		data_set[j++] = status.ampare[i];
	for(i=0; i<SIZE_TEMPERATURE; i++)	data_set[j++] = status.temperature[i];
	for(i=0; i<SIZE_GYRO; i++)			data_set[j++] = status.gyro[i];
	for(i=0; i<SIZE_MAGNET; i++)		data_set[j++] = status.magnet[i];
	for(i=0; i<SIZE_RSSI; i++)			data_set[j++] = status.rssi[i];
	for(i=0; i<SIZE_MISSION; i++)		data_set[j++] = status.mission[i];
	for(i=0; i<SIZE_POWER; i++)			data_set[j++] = status.power[i];
	for(i=0; i<SIZE_INIT; i++)			data_set[j++] = status.init[i];
	for(i=0; i<SIZE_MODE; i++)			data_set[j++] = status.mode[i];
	for(i=0; i<SIZE_CHARGEFLAG; i++)	data_set[j++] = status.chargeflag[i];
	for(i=0; i<SIZE_FLAG; i++)			data_set[j++] = status.flag[i];
	for(i=0; i<SIZE_ERROR; i++)			data_set[j++] = status.error_counter[i];
	for(i=0; i<SIZE_MESSAGE; i++)		data_set[j++] = status.message_counter[i];

	IIC0_EEPRESET();
	IIC0_EEPSTOP();
	
	IIC0_EEPWRITE(eep_page, data_set, slave, MODE128);
	/*
	// codes for debug
	debug_puts("I've written Detailed tlm at.\r\n");
	debug_puts("IIC0, ");
	debug_puts("slave ");
	debug_putdec(slave);
	debug_puts(", ");
	debug_puts("page ");
	debug_putdec(eep_page);
	debug_puts("\r\n\r\n");
	
	for(i=0;i<SIZE_STATUS;i++){
		debug_puts("tlm[");
		debug_putdec(i);
		debug_puts("]: ");
		debug_putdec(data_set[i]);
		debug_puts("\r\n");
	}	*/
	
	debug_puts("\n Write Detail\t");
	debug_putdec((eep_page+slave*1024)*128);
	debug_puts("\ttime: ");
	debug_putdec((data_set[3] & 0x00ff)+((data_set[2] << 8) & 0xff00)+((data_set[1] << 16) & 0x00ff0000)+((data_set[0] << 24) & 0xff000000));
	debug_puts("\n");
	

	// end of debug codes           
	
	
	eep_page++;
	if(eep_page > PAGE_PER_EEPROM_MODE128) {// 1つのEEPROMの最後まで書ききったら	
		slave++;							// 次のEEPROMの最初のページに移る
		eep_page = 0;
		
		if(slave>3){	// IIC0上のスレーブアドレス3のEEPROMの最後まで書ききったら
			slave=0;	// IIC0上のスレーブアドレス0のEEPROMの最初に戻る
			ClearFlags(ena_dteleme);
			debug_puts("\n Stop Dteleme\n");
		}else;
	}else;
	
	return 1;
}

/*************************************************
		テレメトリ書き込み関数(API配信)
*************************************************/
/* APIテレメトリアドレスマップ */
// OBC時刻
#define API_OBCTIME_0	0
#define API_OBCTIME_1	1
#define API_OBCTIME_2	2
#define API_OBCTIME_3	3
// 電流
#define API_AMPARE_PX	4
#define API_AMPARE_MX	5
#define API_AMPARE_PY	6
#define API_AMPARE_MY	7
#define API_AMPARE_PZ	8
#define API_AMPARE_MZ	9
#define API_AMPARE_BAT	10
// 温度
#define API_TEMP_PX		11
#define API_TEMP_MX		12
#define API_TEMP_PY		13
#define API_TEMP_MY		14
#define API_TEMP_PZ		15
#define API_TEMP_MZ		16
#define API_TEMP_BAT0	17
#define API_TEMP_BAT1	18
#define API_TEMP_BAT2	19
// ジャイロ
#define API_GYRO_X		20
#define API_GYRO_Y		21
#define API_GYRO_Z		22
// 磁気
#define API_MAGNET_X	23
#define API_MAGNET_Y	24
#define API_MAGNET_Z	25

unsigned char SetTelemetry_API(void)
{
//	static unsigned long int api_eep_page;		// EEPROMのページ番号
	static unsigned char data_set[MODE32];	// EEPROMに書き込むデータ
	
	/***** OBC時刻 *****/
	data_set[API_OBCTIME_0] = status.obctime[0];
	data_set[API_OBCTIME_1] = status.obctime[1];
	data_set[API_OBCTIME_2] = status.obctime[2];
	data_set[API_OBCTIME_3] = status.obctime[3];	
	
	/***** 電流 *****/
	data_set[API_AMPARE_BAT] = status.ampare[2];
	data_set[API_AMPARE_PX] = status.ampare[11];
	data_set[API_AMPARE_MX] = status.ampare[10];
	data_set[API_AMPARE_PZ] = status.ampare[7];
	data_set[API_AMPARE_MZ] = status.ampare[6];	
	
	// +Y面に関しては，2領域の電流の平均値を記録する
	unsigned int ampare_pY;
	ampare_pY = (status.ampare[5] + status.ampare[9]) >> 1;	// 足して2で割る（溢れないようにint型）
	data_set[API_AMPARE_PY] = (unsigned char)ampare_pY;
	
	// -Y面に関しては，2領域の電流の平均値を記録する
	unsigned int ampare_mY;
	ampare_mY = (status.ampare[4] + status.ampare[8]) >> 1;	// 足して2で割る（溢れないようにint型）
	data_set[API_AMPARE_MY] = (unsigned char)ampare_mY;
	
	/***** 温度 *****/
	data_set[API_TEMP_BAT0] = status.temperature[0];
	data_set[API_TEMP_BAT1] = status.temperature[1];
	data_set[API_TEMP_BAT2] = status.temperature[2];
	data_set[API_TEMP_PX] = status.temperature[3];
	data_set[API_TEMP_MX] = status.temperature[4];
	
	// +Y面に関しては，2領域の温度の平均値を記録する
	unsigned int temp_pY;
	temp_pY = (status.temperature[5] + status.temperature[6]) >> 1;	// 足して2で割る（溢れないようにint型）
	data_set[API_TEMP_PY] = (unsigned char)temp_pY;

	// -Y面に関しては，2領域の温度の平均値を記録する
	unsigned int temp_mY;
	temp_mY = (status.temperature[7] + status.temperature[8]) >> 1;	// 足して2で割る（溢れないようにint型）
	data_set[API_TEMP_MY] = (unsigned char)temp_mY;	
	
	// +Z面に関しては，2領域の温度の平均値を記録する
	unsigned int temp_pZ;
	temp_pZ = (status.temperature[9] + status.temperature[10]) >> 1;// 足して2で割る（溢れないようにint型）
	data_set[API_TEMP_PZ] = (unsigned char)temp_pZ;
	
	// -Z面に関しては，2領域の温度の平均値を記録する
	unsigned int temp_mZ;
	temp_mZ = (status.temperature[11] + status.temperature[12]) >> 1;// 足して2で割る（溢れないようにint型）
	data_set[API_TEMP_MZ] = (unsigned char)temp_mZ;	
	
	/***** ジャイロ *****/
	data_set[API_GYRO_X] = status.gyro[0];
	data_set[API_GYRO_Y] = status.gyro[1];
	data_set[API_GYRO_Y] = status.gyro[2];
	
	/***** 磁気 *****/
	data_set[API_MAGNET_X] = status.magnet[0];
	data_set[API_MAGNET_Y] = status.magnet[1];
	data_set[API_MAGNET_Z] = status.magnet[2];
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPWRITE(api_eep_page, data_set, API_SLAVE, MODE32); // IIC1上のスレーブアドレス3のEEPROMに書く
	
	/*
	// codes for debug
	debug_puts("I've written API tlm at.\r\n");
	debug_puts("IIC1, ");
	debug_puts("slave ");
	debug_putdec(3);
	debug_puts(", ");
	debug_puts("page ");
	debug_putdec(eep_page);
	debug_puts("\r\n\r\n");
	
	for(i=0;i<24;i++){
		debug_puts("tlm[");
		debug_putdec(i);
		debug_puts("]: ");
		debug_putdec(data_set[i]);
		debug_puts("\r\n");
	}
		*/
	debug_puts("\n Write API\t");
	debug_putdec((api_eep_page+API_SLAVE*4096)*32);
	debug_puts("\ttime : ");
	debug_putdec((data_set[3] & 0x00ff)+((data_set[2] << 8) & 0xff00)+((data_set[1] << 16) & 0x00ff0000)+((data_set[0] << 24) & 0xff000000));
	debug_puts("\n");
	// end of debug codes
	
	
	api_eep_page++;
	if(api_eep_page > API_LASTPAGE){	// IIC1,スレーブアドレスAPI_SLAVEのEEPROMのAPI_LASTPAGEまで書ききったら
		api_eep_page = 0;				// IIC1,スレーブアドレス3のEEPROMの最初のページに戻る
	}else;
	
	return 1;
}

/*************************************************
		テレメトリ書き込み関数(初期化情報)
*************************************************/
unsigned char SetTelemetry_Initial(void)
{
	unsigned char i,n;
	unsigned char j = 0;
	static unsigned char data_obc[MODE32];
	static unsigned char data_error[MODE32];
	
	static unsigned char data_rough_slave[MODE32];
	static unsigned char data_rough_page[MODE32];
	static unsigned char data_api_page[MODE32];
	
	for(n=0; n<3; n++) {
		for(i=0; i<SIZE_OBCTIME; i++) {
			data_obc[j++] = status.obctime[i];
		}
	}
		
	j=0;
	for(i=0; i<SIZE_ERROR; i++) {
		for(n=0; n<3; n++) {
			data_error[j++] = status.error_counter[i];
		}
	}
	j=0;
	for(n=0; n<3; n++) {
		for(i=0; i<SIZE_OBCTIME; i++) {
			data_rough_slave[j++] = (rough_slave >> 24)	& 0x00ff;
			data_rough_slave[j++] = (rough_slave >> 16)	& 0x00ff;
			data_rough_slave[j++] = (rough_slave >>  8)	& 0x00ff;
			data_rough_slave[j++] =  rough_slave		& 0x00ff;

		}
	}
	j=0;
	for(n=0; n<3; n++) {
		for(i=0; i<SIZE_OBCTIME; i++) {
			data_rough_page[j++] = (rough_eep_page >> 24)	& 0x00ff;
			data_rough_page[j++] = (rough_eep_page >> 16)	& 0x00ff;
			data_rough_page[j++] = (rough_eep_page >>  8)	& 0x00ff;
			data_rough_page[j++] =  rough_eep_page			& 0x00ff;

		}
	}
	j=0;
	for(n=0; n<3; n++) {
		for(i=0; i<SIZE_OBCTIME; i++) {
			data_api_page[j++] = (api_eep_page >> 24)	& 0x00ff;
			data_api_page[j++] = (api_eep_page >> 16)	& 0x00ff;
			data_api_page[j++] = (api_eep_page >>  8)	& 0x00ff;
			data_api_page[j++] =  api_eep_page			& 0x00ff;

		}
	}
	debug_puts("writing..................\n");		
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPWRITE(OBCTIME_PAGE, data_obc, INITIAL_SLAVE, MODE32);
	
	debug_puts("          ");		
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPWRITE(ERRORCOUNT_PAGE, data_error, INITIAL_SLAVE, MODE32);
	
	debug_puts("          ");		
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPWRITE(ROUGH_SLAVE_PAGE, data_rough_slave, INITIAL_SLAVE, MODE32);

	debug_puts("          ");		
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPWRITE(ROUGH_PAGE, data_rough_page, INITIAL_SLAVE, MODE32);

	debug_puts("          \n");		
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPWRITE(API_PAGE, data_api_page, INITIAL_SLAVE, MODE32);

	
	debug_puts("\n Write Initial\t");
	debug_puts("\ttime:");
	debug_putdec((data_obc[3] & 0x00ff)+((data_obc[2] << 8) & 0xff00)+((data_obc[1] << 16) & 0x00ff0000)+((data_obc[0] << 24) & 0xff000000));
	debug_puts("\n\t\tRough Address:");
	debug_putdec((rough_eep_page+rough_slave*1024)*128);
	debug_puts("\tAPI Address:");
	debug_putdec((api_eep_page+API_SLAVE*4096)*32);
	debug_puts("\n");	
	debug_putdec((data_rough_slave[3] & 0x00ff)+((data_rough_slave[2] << 8) & 0xff00)+((data_rough_slave[1] << 16) & 0x00ff0000)+((data_rough_slave[0] << 24) & 0xff000000));
	debug_puts("\t");
	debug_putdec((data_rough_page[3] & 0x00ff)+((data_rough_page[2] << 8) & 0xff00)+((data_rough_page[1] << 16) & 0x00ff0000)+((data_rough_page[0] << 24) & 0xff000000));
	debug_puts("\t");
	debug_putdec((data_api_page[3] & 0x00ff)+((data_api_page[2] << 8) & 0xff00)+((data_api_page[1] << 16) & 0x00ff0000)+((data_api_page[0] << 24) & 0xff000000));
	debug_puts("\n");



	/*
	//codes for debug
	debug_puts("\n");
	debug_puts("status.obctime[0]: "); debug_puthex(status.obctime[0]); debug_puts("\n");
	debug_puts("status.obctime[1]: "); debug_puthex(status.obctime[1]); debug_puts("\n");
	debug_puts("status.obctime[2]: "); debug_puthex(status.obctime[2]); debug_puts("\n");
	debug_puts("status.obctime[3]: "); debug_puthex(status.obctime[3]); debug_puts("\n");
	debug_puts("\n");
	
	static unsigned char buf_obc[MODE32];
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	IIC1_EEPREAD(OBCTIME_PAGE, buf_obc, INITIAL_SLAVE, MODE32);
	debug_puts("written_obctime[0]: "); debug_puthex(buf_obc[0]); debug_puts("\n");
	debug_puts("written_obctime[1]: "); debug_puthex(buf_obc[1]); debug_puts("\n");
	debug_puts("written_obctime[2]: "); debug_puthex(buf_obc[2]); debug_puts("\n");
	debug_puts("written_obctime[3]: "); debug_puthex(buf_obc[3]); debug_puts("\n");
	debug_puts("\n");
	//end of debug
	*/
	
	return 1;
}


/*************************************************
		初期化情報読み込み関数
*************************************************/
unsigned char Read_Initial(void)
{
	unsigned char i,n;
	unsigned int j = 0;
	static unsigned char data_obc[32];
	static unsigned char data_error[32];
	
	static unsigned char data_rough_slave[MODE32];
	static unsigned char data_rough_page[MODE32];
	static unsigned char data_api_page[MODE32];
	
	debug_puts(" ");	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts(" ");
	IIC1_EEPREAD(OBCTIME_PAGE,data_obc,INITIAL_SLAVE,MODE32);
	
	for(i=0; i<SIZE_OBCTIME; i++) {
		status.obctime[j++] = OBC_Majority(data_obc, i);			
	}
	
	itu1_cnt = OBC_Majority(data_obc, 3) & 0x00ff;
	itu1_cnt |=	(OBC_Majority(data_obc, 2) << 8) & 0xff00;
	itu1_cnt |=	(OBC_Majority(data_obc, 1) << 16) & 0x00ff0000;
	itu1_cnt |=	(OBC_Majority(data_obc, 0) << 24) & 0xff000000;
	
	debug_puts(" ");	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts(" ");
	IIC1_EEPREAD(ERRORCOUNT_PAGE,data_error,INITIAL_SLAVE,MODE32);
	
	j=0;
	for(i=0; i<SIZE_ERROR*3; i=i+3) {
		status.error_counter[j++] = Error_Majority(data_error, i);
	}
	
	
	
	debug_puts(" ");	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts(" ");
	IIC1_EEPREAD(ROUGH_SLAVE_PAGE,data_rough_slave,INITIAL_SLAVE,MODE32);		
	rough_slave = OBC_Majority(data_rough_slave, 3) & 0x00ff;
	rough_slave |=	(OBC_Majority(data_rough_slave, 2) << 8) & 0xff00;
	rough_slave |=	(OBC_Majority(data_rough_slave, 1) << 16) & 0x00ff0000;
	rough_slave |=	(OBC_Majority(data_rough_slave, 0) << 24) & 0xff000000;
	if(rough_slave>2){
		debug_puts("\nROUGH SLAVE ERROR\t");
		debug_putdec(OBC_Majority(data_rough_slave, 0));
		debug_putdec(OBC_Majority(data_rough_slave, 1));
		debug_putdec(OBC_Majority(data_rough_slave, 2));
		debug_putdec(OBC_Majority(data_rough_slave, 3));
		rough_slave=0;
	}


	debug_puts(" ");
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts(" ");
	IIC1_EEPREAD(ROUGH_PAGE,data_rough_page,INITIAL_SLAVE,MODE32);		
	rough_eep_page = OBC_Majority(data_rough_page, 3) & 0x00ff;
	rough_eep_page |=	(OBC_Majority(data_rough_page, 2) << 8) & 0xff00;
	rough_eep_page |=	(OBC_Majority(data_rough_page, 1) << 16) & 0x00ff0000;
	rough_eep_page |=	(OBC_Majority(data_rough_page, 0) << 24) & 0xff000000;
	if(rough_eep_page > PAGE_PER_EEPROM_MODE128){
		debug_puts("\nROUGH PAGE ERROR\t");
		debug_putdec(OBC_Majority(data_rough_page, 0));
		debug_putdec(OBC_Majority(data_rough_page, 1));
		debug_putdec(OBC_Majority(data_rough_page, 2));
		debug_putdec(OBC_Majority(data_rough_page, 3));
		rough_eep_page=0;
	}

	debug_puts(" ");
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts(" ");
	IIC1_EEPREAD(API_PAGE,data_api_page,INITIAL_SLAVE,MODE32);		
	api_eep_page = OBC_Majority(data_api_page, 3) & 0x00ff;
	api_eep_page |=	(OBC_Majority(data_api_page, 2) << 8) & 0xff00;
	api_eep_page |=	(OBC_Majority(data_api_page, 1) << 16) & 0x00ff0000;
	api_eep_page |=	(OBC_Majority(data_api_page, 0) << 24) & 0xff000000;
	if(api_eep_page > API_LASTPAGE){
		debug_puts("\nAPI PAGE ERROR\t");
		debug_putdec(OBC_Majority(data_api_page, 0));
		debug_putdec(OBC_Majority(data_api_page, 1));
		debug_putdec(OBC_Majority(data_api_page, 2));
		debug_putdec(OBC_Majority(data_api_page, 3));
		api_eep_page=0;
	}
	
	//デバッグ出力
	debug_puts("\n");
	debug_puts("**********  status of config  **********");
	debug_puts("\n");
	
	//OBC時刻
	debug_puts("obctime:\t");
	debug_putdec(itu1_cnt);
	debug_puts("\n");
	
	debug_puts("obctime[0-3]:\t"); debug_puthex(status.obctime[0]); debug_puts(" ");
	debug_puthex(status.obctime[1]); debug_puts(" ");
	debug_puthex(status.obctime[2]); debug_puts(" ");
	debug_puthex(status.obctime[3]); debug_puts("\n");
	
	//エラー履歴
	debug_puts("error[0-9]:\t");
	for(i=0;i<SIZE_ERROR;i++){
		debug_putdec(status.error_counter[i]);
		debug_puts(" ");
	}
	
	//粗履歴＆API
	debug_puts("\nRough_slave:\t");
	debug_putdec(rough_slave);
	debug_puts("\nRough_page:\t");
	debug_putdec(rough_eep_page);
	debug_puts("\nAPI_page:\t");
	debug_putdec(api_eep_page);
	debug_puts("\n\n");
	
	return 1;
}

/*************************************************
		多数決関数（OBC時刻情報）
*************************************************/
unsigned char OBC_Majority(unsigned char data_set[], unsigned char i)
{
	if(data_set[i] == data_set[i+SIZE_OBCTIME] ||
	   data_set[i] == data_set[i+SIZE_OBCTIME*2])
	{
		return data_set[i];
	}
	else if(data_set[i+SIZE_OBCTIME] == data_set[i+SIZE_OBCTIME*2])
	{
		return data_set[i+SIZE_OBCTIME];
	}
	else 
	{
		//return data_set[i];
		return 0;
	}
	return 0;
}

/*************************************************
		多数決関数（エラーカウント情報）
*************************************************/
unsigned char Error_Majority(unsigned char data_set[], unsigned char i)
{
	if(data_set[i] == data_set[i+1]||
	   data_set[i] == data_set[i+2]) 
	{
		return data_set[i];
	}
	else if(data_set[i+1] == data_set[i+2])
	{
		return data_set[i+1];
	}
	else
	{
		//return data_set[i];
		return 0;
	}
	return 0;
}

/*************************************************
		共有メモリ特別領域書き込み関数(Boot Mode)
*************************************************/
unsigned char WriteIicBootMode(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	
	if(param_length != BootModeLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3FF, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length;i++){
		dat[5+i]=param[i];
		dat[43+i]=param[i];
		dat[81+i]=param[i];
		
		debug_puts("Write\t");
		debug_puthex(5+i);
		debug_puts("\t");
		debug_puthex(dat[5+i]);
		debug_puts("\n");
		
		debug_puts("Write\t");
		debug_puthex(43+i);
		debug_puts("\t");
		debug_puthex(dat[43+i]);
		debug_puts("\n");

		debug_puts("Write\t");
		debug_puthex(81+i);
		debug_puts("\t");
		debug_puthex(dat[81+i]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3FF, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3FF, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(Boot Flag)
*************************************************/
unsigned char WriteIicBootFlag(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	
	if(param_length != BootFlagLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3FF, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length;i++){
		dat[6+i]=param[i];
		dat[44+i]=param[i];
		dat[82+i]=param[i];
		
		debug_puts("Write\t");
		debug_puthex(6+i);
		debug_puts("\t");
		debug_puthex(dat[6+i]);
		debug_puts("\n");
		
		debug_puts("Write\t");
		debug_puthex(44+i);
		debug_puts("\t");
		debug_puthex(dat[44+i]);
		debug_puts("\n");

		debug_puts("Write\t");
		debug_puthex(82+i);
		debug_puts("\t");
		debug_puthex(dat[82+i]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3FF, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3FF, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(Boot Time & Boot Count)
*************************************************/
unsigned char WriteIicBootTime(volatile unsigned long time){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char param_length=4;
	unsigned char param[4];
	
	param[0]=(time>>24);
	param[1]=(time>>16);
	param[2]=(time>>8);
	param[3]=time;
	
	debug_puts("\n OBC time : ");
	debug_puthex(time);
	debug_puts("\n");
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");		/*時間稼ぎ用*/
	IIC2_EEPREAD(0x3FF, dat, 3, MODE128);	//読み込み
	
	/*timeに関する書き換え*/
	for(i=0;i<param_length;i++){
		dat[i]=param[i];
		dat[38+i]=param[i];
		dat[76+i]=param[i];
		
		/*
		debug_puts("Write time\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
		
		debug_puts("Write time\t");
		debug_puthex(38+i);
		debug_puts("\t");
		debug_puthex(dat[38+i]);
		debug_puts("\n");

		debug_puts("Write time\t");
		debug_puthex(76+i);
		debug_puts("\t");
		debug_puthex(dat[76+i]);
		debug_puts("\n");
		*/

	}
	/*起動回数に関する書き換え*/
	/*
	debug_puts("Read count\t");
	debug_puthex(4);
	debug_puts("\t");
	debug_puthex(dat[4]);
	debug_puts("\n");
	
	debug_puts("Read count\t");
	debug_puthex(42);
	debug_puts("\t");
	debug_puthex(dat[42]);
	debug_puts("\n");
	
	debug_puts("Read count\t");
	debug_puthex(80);
	debug_puts("\t");
	debug_puthex(dat[80]);
	debug_puts("\n");
	*/
	
	dat[4]=dat[4]+1;
	dat[42]=dat[42]+1;
	dat[80]=dat[80]+1;
	/*
	debug_puts("Write count\t");
	debug_puthex(4);
	debug_puts("\t");
	debug_puthex(dat[4]);
	debug_puts("\n");
	
	debug_puts("Write count\t");
	debug_puthex(42);
	debug_puts("\t");
	debug_puthex(dat[42]);
	debug_puts("\n");
	
	debug_puts("Write count\t");
	debug_puthex(80);
	debug_puts("\t");
	debug_puthex(dat[80]);
	debug_puts("\n");
	*/
	/*デバッグ用
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	*/
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");	/*時間稼ぎ用*/
	IIC2_EEPWRITE(0x3FF, dat, 3, MODE128);
	
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");		//時間稼ぎ用
	
	IIC2_EEPREAD(0x3FF, debug_dat, 3, MODE128);	
	
	debug_puts("Read time\t0to3\t");
	debug_puthex(debug_dat[0]);
	debug_puthex(debug_dat[1]);
	debug_puthex(debug_dat[2]);
	debug_puthex(debug_dat[3]);
	debug_puts("\n");
		
	debug_puts("Read time\t38to41\t");
	debug_puthex(debug_dat[38]);
	debug_puthex(debug_dat[39]);
	debug_puthex(debug_dat[40]);
	debug_puthex(debug_dat[41]);
	debug_puts("\n");

	debug_puts("Read time\t76to79\t");
	debug_puthex(debug_dat[76]);
	debug_puthex(debug_dat[77]);
	debug_puthex(debug_dat[78]);
	debug_puthex(debug_dat[79]);
	debug_puts("\n");
	
	
	debug_puts("Read count\t4\t");
	debug_puthex(debug_dat[4]);
	debug_puts("\n");
	
	debug_puts("Read count\t42\t");
	debug_puthex(debug_dat[42]);
	debug_puts("\n");
	
	debug_puts("Read count\t80\t");
	debug_puthex(debug_dat[80]);
	debug_puts("\n");
	/*
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	*/
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(Camera Param)
*************************************************/
unsigned char WriteIicCameraParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != CameraParamLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>3){
		debug_puts("section error!\n");
		return 1;
	}
	
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3F6, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3F6, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3F6, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}


/*************************************************
		共有メモリ特別領域書き込み関数(Morse Param)
*************************************************/
unsigned char WriteIicMorseParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != MorseParamLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>3){
		debug_puts("section error!\n");
		return 1;
	}
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3F6, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+64+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+64+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+64+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3F6, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3F6, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}


/*************************************************
		共有メモリ特別領域書き込み関数(Digitalker Param)
*************************************************/
unsigned char WriteIicDigitalkerParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != DigitalkerParamLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>7){
		debug_puts("section error!\n");
		return 1;
	}

	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3F7, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3F7, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3F7, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(Note Param)
*************************************************/
unsigned char WriteIicNoteParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != NoteParamLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>15){
		debug_puts("section error!\n");
		return 1;
	}
	
	unsigned char page2=0;
	if(section>7){
		page2=1;
	}

	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3F8+page2, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+(section-page2*8)*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+(section-page2*8)*16);
		debug_puts("\t");
		debug_puthex(dat[i+(section-page2*8)*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3F8+page2, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3F8+page2, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(Text Debug)
*************************************************/
unsigned char WriteIicTextDebug(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != NoteParamLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>7){
		debug_puts("section error!\n");
		return 1;
	}

	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3FA, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3FA, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3FA, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(TextZ Param)
*************************************************/
unsigned char WriteIicTextZParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != TextLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>7){
		debug_puts("section error!\n");
		return 1;
	}

	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3FB, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3FB, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3FB, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ特別領域書き込み関数(TextY Param)
*************************************************/
unsigned char WriteIicTextYParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != TextLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>7){
		debug_puts("section error!\n");
		return 1;
	}
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3FC, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3FC, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3FC, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}


/*************************************************
		共有メモリ特別領域書き込み関数(TextX Param)
*************************************************/
unsigned char WriteIicTextXParam(volatile unsigned char* param, unsigned char param_length){
	unsigned char dat[128];
	unsigned char debug_dat[128];
	int i;
	unsigned char section;
	
	if(param[1]!=0x2e){
		debug_puts("param error!\n");
		return 1;
	}
	if(param_length != TextLength){
		debug_puts("param_length error!\n");
		return 1;
	}
	section=(unsigned char)param[0]-48;
	if(section>7){
		debug_puts("section error!\n");
		return 1;
	}
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");	
	IIC2_EEPREAD(0x3FD, dat, 3, MODE128);	//読み込み
	for(i=0;i<param_length-2;i++){
		dat[i+section*16]=param[i+2];
		
		debug_puts("Write\t");
		debug_puthex(i+section*16);
		debug_puts("\t");
		debug_puthex(dat[i+section*16]);
		debug_puts("\n");

	}
	for(i=0;i<128;i++){
		debug_puts("Writing\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(dat[i]);
		debug_puts("\n");
	}
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("\nWriting...\n");
	IIC2_EEPWRITE(0x3FD, dat, 3, MODE128);
	
	IIC2_EEPRESET();
	IIC2_EEPSTOP();
	debug_puts("Reading...\n");
	IIC2_EEPREAD(0x3FD, debug_dat, 3, MODE128);	
	for(i=0;i<128;i++){
		debug_puts("Writed\t");
		debug_puthex(i);
		debug_puts("\t");
		debug_puthex(debug_dat[i]);
		debug_puts("\n");
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;

}

/*************************************************
		共有メモリ読み出しコマンド
*************************************************/
unsigned char ReadSharedROM(volatile unsigned char* param, unsigned char param_length)
{
	
	unsigned char rsr[256];

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
	
	for(page = start_page; page < start_page+range; page++){	
		int k=0;
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		IIC2_EEPREAD(page,tlm,slave,MODE128);
		;;
		debug_puts("\n");
		debug_puts("---------------page:");
		debug_putdec(page);
		debug_puts("---------------\n");
		
		rsr[k++]='r';
		rsr[k++]='-';
		rsr[k++]='g';
		rsr[k++]='-';
		rsr[k++]='c';
		rsr[k++]='-';
		rsr[k++]='r';
		rsr[k++]='s';
		rsr[k++]='r';
		rsr[k++]='-';
		
		
		for(i=0;i<MODE128;i++){
			debug_puts("tlm[");
			debug_putdec(i);
			debug_puts("]: ");
			debug_putdec(tlm[i]);
			debug_puts("\n");
			rsr[k++]=tlm[i];
		}
		
		// tlm[] をASCIIで表示
		debug_puts("text:");
		debug_puts(tlm);
		debug_puts("\n");
		
		rsr[k++]=0x20;
		rsr[k++]=0x0d;
		rsr[k++]=0x0a;
		Ax_Send(rsr,k);//長さは141のはず
		k=0;
	}
	//end of debug
	
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	
	debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		共有メモリ読み出しコマンド
*************************************************/
unsigned char LongReadSharedROM(volatile unsigned char* param, unsigned char param_length)
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
	unsigned long int range0 = (unsigned long int)param[6]-48;
	unsigned long int range1 = (unsigned long int)param[7]-48;
	unsigned long int range2 = (unsigned long int)param[8]-48;
	unsigned long int range = 100*range0+10*range1+range2;

	
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
	debug_puts("Reading.................................................\n");
	
	for(page = start_page; page < start_page+range; page++){	
		int k=0;
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		IIC2_EEPREAD(page,tlm,slave,MODE128);
		;;
		
		for(i=0;i<MODE128;i++){
			debug_putc(tlm[i]);
		}
	}
	//end of debug
	
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	
	debug_puts(".................................................Finish reading.");

	return 1;
}

#define PAGESIZE 128
#define SLAVESIZE 1024
/*************************************************
		共有メモリ書き込み関数(General)
*************************************************/
unsigned char GeneralWriteSharedMomery(volatile unsigned char* param, unsigned char param_length){
	/*書き込むところのアドレスとか長さとか内容とかを取得する*/
	unsigned long int address = 0;
	unsigned long int size = 0;
	int i,j;
	unsigned int pb,pe,mb,me;
	
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		address=address*10+(unsigned long int)(param[i]-48);
		if(address>524288){
			debug_puts("address param error\n");
			return 1;
		}
	}

	for(;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		size=size*10+(unsigned long int)(param[i]-48);
		if(size>524288){
			debug_puts("size param error\n");
			return 1;
		}
	}
	j=0;
	unsigned char dat[size];
	for(;i<param_length;i++){
		dat[j]=param[i];
		j++;
	}
	
	pb=address/PAGESIZE;
	pe=(address+size-1)/PAGESIZE;
	mb=address%PAGESIZE;
	me=PAGESIZE;
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	/*書き込む*/
	unsigned char slave_num;
	unsigned long int page_num;
	unsigned long int k=0;
	unsigned char dat2[PAGESIZE];
	
	for(i=pb;i<=pe;i++){
		
		slave_num=i/SLAVESIZE;
		page_num=i%SLAVESIZE;
		
		if(i>=pe){
			me = (address + size - 1) % PAGESIZE + 1;
		}

		debug_puts("");	// スイッチングの時間稼ぎ
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("Reading...\n");	
		IIC2_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			dat2[j]=dat[k++];
			debug_puts("Write\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat[k-1]);
			debug_puts("\n");
		}
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("\nWriting...\n");
		IIC2_EEPWRITE(page_num, dat2, slave_num, MODE128);
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("Reading...\n");
		IIC2_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			debug_puts("Read\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat2[j]);
			debug_puts("\n");
		}	
		if (i <= pb) {
			mb = 0;
		}
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;

	return 0;
}	

/*************************************************
		共有メモリ書き込み関数(General)
*************************************************/
unsigned char GeneralWriteSharedAscii(volatile unsigned char* param, unsigned char param_length){
	/*書き込むところのアドレスとか長さとか内容とかを取得する*/
	unsigned long int address = 0;
	unsigned long int size = 0;
	int i,j,h;
	unsigned int pb,pe,mb,me;
	
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		address=address*10+(unsigned long int)(param[i]-48);
		if(address>524288){
			debug_puts("address param error\n");
			return 1;
		}
	}

	for(;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		size=size*10+(unsigned long int)(param[i]-48);
		if(size>524288){
			debug_puts("size param error\n");
			return 1;
		}
	}
	j=0;
	h=0;
	unsigned char dat[size];
	for(;i<param_length;i++){
		if(j>=size){
			debug_puts("\nlength too long\n");
			break;
		}
		if(h>0){
			if((0x2f<param[i])&&(param[i]<0x3a)){
				dat[j]=dat[j]*16+param[i]-48;
			}
			else if((0x40<param[i])&&(param[i]<0x47)){
				dat[j]=dat[j]*16+param[i]-55;
			}
			else if((0x60<param[i])&&(param[i]<0x67)){
				dat[j]=dat[j]*16+param[i]-87;
			}
			else{
				debug_puts("\n Error param not correct \n");
				return 0;
			}
				
			h=0;
			j++;
		}
		else{
			if((0x2f<param[i])&&(param[i]<0x3a)){
				dat[j]=param[i]-48;
			}
			else if((0x40<param[i])&&(param[i]<0x47)){
				dat[j]=param[i]-55;
			}
			else if((0x60<param[i])&&(param[i]<0x67)){
				dat[j]=param[i]-87;
			}
			else{
				debug_puts("\n Error param not correct \n");
				return 0;
			}
			h++;
		}		
	}
	if(h%2!=0){
		debug_puts("\n Error param length is odd \n");
		return 0;
	}
	
	pb=address/PAGESIZE;
	pe=(address+size-1)/PAGESIZE;
	mb=address%PAGESIZE;
	me=PAGESIZE;
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	/*書き込む*/
	unsigned char slave_num;
	unsigned long int page_num;
	unsigned long int k=0;
	unsigned char dat2[PAGESIZE];
	
	for(i=pb;i<=pe;i++){
		
		slave_num=i/SLAVESIZE;
		page_num=i%SLAVESIZE;
		
		if(i>=pe){
			me = (address + size - 1) % PAGESIZE + 1;
		}

		debug_puts("");	// スイッチングの時間稼ぎ
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("Reading...\n");	
		IIC2_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			dat2[j]=dat[k++];
			debug_puts("Write\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat[k-1]);
			debug_puts("\n");
		}
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("\nWriting...\n");
		IIC2_EEPWRITE(page_num, dat2, slave_num, MODE128);
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("Reading...\n");
		IIC2_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			debug_puts("Read\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat2[j]);
			debug_puts("\n");
		}
	
		if (i <= pb) {
			mb = 0;
		}
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;
	return 0;
}	

#define PAGESIZE 128
#define SLAVESIZE 1024
/*************************************************
		共有メモリ読み込み関数(General)
*************************************************/
unsigned char GeneralReadSharedMomery(volatile unsigned char* param, unsigned char param_length){
	/*書き込むところのアドレスとか長さとか内容とかを取得する*/
	unsigned long int address = 0;
	unsigned long int size = 0;
	int i,j;
	unsigned int pb,pe,mb,me;
	
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		address=address*10+(unsigned long int)(param[i]-48);
		if(address>524288){
			debug_puts("address param error\n");
			return 1;
		}
	}

	for(;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		size=size*10+(unsigned long int)(param[i]-48);
		if(size>524288){
			debug_puts("size param error\n");
			return 1;
		}
	}
	
	pb=address/PAGESIZE;
	pe=(address+size-1)/PAGESIZE;
	mb=address%PAGESIZE;
	me=PAGESIZE;
	
	// ※共有メモリにアクセスする場合はEN,CHを切り替える必要がある
	unsigned char SharedMemoryEnBuffer;
	unsigned char SharedMemoryChBuffer;
		
	SharedMemoryEnBuffer = SROM_EN;	// 一時退避
	SharedMemoryChBuffer = SROM_CH;	// 一時退避
			
	SROM_EN = 1;	// SharedMemory許可
	SROM_CH = MAIN;	// SharedMemoryをメインCPUに接続
			
	debug_puts("Switch & Enable SharedMemory.\n");	// スイッチングの時間稼ぎ

	
	/*読み込む*/
	unsigned char slave_num;
	unsigned long int page_num;
	unsigned long int k=0;
	unsigned char dat2[PAGESIZE];
	unsigned char message[256];
	
	
	for(i=pb;i<=pe;i++){
		message[k++]='r';
		message[k++]='-';
		message[k++]='g';
		message[k++]='-';
		message[k++]='c';
		message[k++]='-';
		message[k++]='g';
		message[k++]='r';
		message[k++]='s';
		message[k++]='-';
		
		slave_num=i/SLAVESIZE;
		page_num=i%SLAVESIZE;
		
		if(i>=pe){
			me = (address + size - 1) % PAGESIZE + 1;
		}

		debug_puts("");	// スイッチングの時間稼ぎ
	
		IIC2_EEPRESET();
		IIC2_EEPSTOP();
		debug_puts("Reading...\n");	
		IIC2_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			debug_puts("Read\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat2[j]);
			debug_puts("\n");
			message[k++]=dat2[j];
		}
	

		
		if (i <= pb) {
			mb = 0;
		}
		message[k++]=0x20;
		message[k++]=0x0d;
		message[k++]=0x0a;
		Ax_Send(message,k);//長さは141のはず
		k=0;
	}
	// Enable,CHを元に戻す
	SROM_EN = SharedMemoryEnBuffer;
	SROM_CH = SharedMemoryChBuffer;	
	return 0;
}

#define PAGESIZE 128
#define SLAVESIZE 1024
/*************************************************
		iic0メモリ読み込み関数(General)
*************************************************/
unsigned char GeneralReadIIC0(volatile unsigned char* param, unsigned char param_length){
	/*書き込むところのアドレスとか長さとか内容とかを取得する*/
	unsigned long int address = 0;
	unsigned long int size = 0;
	int i,j;
	unsigned int pb,pe,mb,me;
	
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		address=address*10+(unsigned long int)(param[i]-48);
		if(address>524288){
			debug_puts("address param error\n");
			return 1;
		}
	}

	for(;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		size=size*10+(unsigned long int)(param[i]-48);
		if(size>524288){
			debug_puts("size param error\n");
			return 1;
		}
	}
	
	pb=address/PAGESIZE;
	pe=(address+size-1)/PAGESIZE;
	mb=address%PAGESIZE;
	me=PAGESIZE;
	
			
	debug_puts("");	// スイッチングの時間稼ぎ

	
	/*読み込む*/
	unsigned char slave_num;
	unsigned long int page_num;
	unsigned long int k=0;
	unsigned char dat2[PAGESIZE];
	unsigned char message[256];
	
	for(i=pb;i<=pe;i++){
		
		message[k++]='r';
		message[k++]='-';
		message[k++]='g';
		message[k++]='-';
		message[k++]='c';
		message[k++]='-';
		message[k++]='g';
		message[k++]='r';
		message[k++]='0';
		message[k++]='-';
		
		
		slave_num=i/SLAVESIZE;
		page_num=i%SLAVESIZE;
		
		if(i>=pe){
			me = (address + size - 1) % PAGESIZE + 1;
		}

		debug_puts("");	// スイッチングの時間稼ぎ
	
		IIC0_EEPRESET();
		IIC0_EEPSTOP();
		debug_puts("Reading...\n");	
		IIC0_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			debug_puts("Read\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat2[j]);
			debug_puts("\n");
			message[k++]=dat2[j];
		}

		
		if (i <= pb) {
			mb = 0;
		}
		message[k++]=0x20;
		message[k++]=0x0d;
		message[k++]=0x0a;
		Ax_Send(message,k);//長さは141のはず
		k=0;		
	}
	return 0;
}		
	

#define PAGESIZE 128
#define SLAVESIZE 1024
/*************************************************
		iic1メモリ読み込み関数(General)
*************************************************/
unsigned char GeneralReadIIC1(volatile unsigned char* param, unsigned char param_length){
	/*書き込むところのアドレスとか長さとか内容とかを取得する*/
	unsigned long int address = 0;
	unsigned long int size = 0;
	int i,j;
	unsigned int pb,pe,mb,me;
	
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		address=address*10+(unsigned long int)(param[i]-48);
		if(address>524288){
			debug_puts("address param error\n");
			return 1;
		}
	}

	for(;i<param_length;i++){
		if(param[i]==0x2e){
			i++;
			break;
		}
		size=size*10+(unsigned long int)(param[i]-48);
		if(size>524288){
			debug_puts("size param error\n");
			return 1;
		}
	}
	
	pb=address/PAGESIZE;
	pe=(address+size-1)/PAGESIZE;
	mb=address%PAGESIZE;
	me=PAGESIZE;
	
			
	debug_puts("");	// スイッチングの時間稼ぎ

	
	/*読み込む*/
	unsigned char slave_num;
	unsigned long int page_num;
	unsigned long int k=0;
	unsigned char dat2[PAGESIZE];
	unsigned char message[256];
	
	for(i=pb;i<=pe;i++){
		
		message[k++]='r';
		message[k++]='-';
		message[k++]='g';
		message[k++]='-';
		message[k++]='c';
		message[k++]='-';
		message[k++]='g';
		message[k++]='r';
		message[k++]='1';
		message[k++]='-';
		
		
		slave_num=i/SLAVESIZE;
		page_num=i%SLAVESIZE;
		
		if(i>=pe){
			me = (address + size - 1) % PAGESIZE + 1;
		}

		debug_puts("");	// スイッチングの時間稼ぎ
	
		IIC1_EEPRESET();
		IIC1_EEPSTOP();
		debug_puts("Reading...\n");	
		IIC1_EEPREAD(page_num, dat2, slave_num, MODE128);	//読み込み
		for(j=mb;j<me;j++){
			debug_puts("Read\t");
			debug_putdec(j+page_num*PAGESIZE+slave_num*SLAVESIZE*PAGESIZE);
			debug_puts("\t");
			debug_puthex(dat2[j]);
			debug_puts("\n");
			message[k++]=dat2[j];

		}

		
		if (i <= pb) {
			mb = 0;
		}
		message[k++]=0x20;
		message[k++]=0x0d;
		message[k++]=0x0a;
		Ax_Send(message,k);//長さは141のはず
		k=0;		
	}
	return 0;
}		
	