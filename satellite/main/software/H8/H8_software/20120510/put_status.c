	/*************************************************************************************
* ファイル名　：　put_status.c
* タイトル　　：　INVADERステータス送信プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.15
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#include "put_status.h"
#include "global.h"
#include "teleme.h"

/*************************************************
        	ステータスコマンド実行関数
*************************************************/
unsigned char SendStatusToGS(unsigned char reply_num)
{
	unsigned char i,j,cnt;
	unsigned char sta[256];
	j=0;
	sta[j++]='r';
	sta[j++]='-';
	sta[j++]='g';
	sta[j++]='-';
	sta[j++]='c';
	sta[j++]='-';
	sta[j++]='s';
	sta[j++]='t';
	sta[j++]='a';
	sta[j++]='-';
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
			
	sta[j++]=0x20;
	sta[j++]=0x0d;
	sta[j++]=0x0a;
	Ax_Send(sta,j);		//jは140のはず
/*	
	for(cnt=0;cnt<reply_num-0x30;cnt++){
		for(i=0;i<127;i++)	debug_putc(sta[i]);
		debug_puts(" \r\n");
	}
*/	
	return 1;
}



/*************************************************
		    粗履歴読み出しコマンド
*************************************************/
unsigned char PutOut_Rough(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char byte = 128;
	unsigned char slave = 0;
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
	

/*	while(start_page >= 0x3FF) {
		start_page = start_page - 0x3FF;
		slave++;
	}
	if(slave>2){
//		debug_puts("\nrange error\n");
		Ax_Send("r-g-c-pte-RangeError1 \r\n",24);
		return 1;
	}
*/
	if(start_page<1024){
		start_page = start_page;
		slave=0;
	}
	else if(start_page<2048){
		start_page = start_page-1024;
		slave=1;
	}
	else if(start_page<3072){
		start_page = start_page-2048;
		slave=2;
	}
	else{
		Ax_Send("r-g-c-ptr-RangeError1 \r\n",24);
		return 1;
	}

	/*
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
	*/
	
	unsigned long int page;
	unsigned char i;
	unsigned char cnt;
	static unsigned char tlm[128];		// EEPROM読込バッファ 128Byte/page
	unsigned char ptr[256];
	
	//debug_puts("Reading...\n");
	
	for(page = start_page; page < start_page+range; page++)
	{	
		debug_puts("\nRough page = ");
		debug_putdec(page);
		debug_puts("\tslave = ");
		debug_putdec(slave);
		debug_puts("\n");
		
		IIC1_EEPRESET();
		IIC1_EEPSTOP();
		debug_puts("Reading...\n");
		IIC1_EEPREAD(page,tlm,slave,byte);   // device = 0
		;;
		int k=0;
//		debug_puts("r-g-c-ptr-");
//		debug_putdec(start_page);
		
		ptr[k++]='r';
		ptr[k++]='-';
		ptr[k++]='g';
		ptr[k++]='-';
		ptr[k++]='c';
		ptr[k++]='-';
		ptr[k++]='p';
		ptr[k++]='t';
		ptr[k++]='r';
		ptr[k++]='-';
		
		for(i=0;i<SIZE_STATUS;i++){
			ptr[k++]=tlm[i];
//			debug_putc(tlm[i]);
		}
		ptr[k++]=0x20;
		ptr[k++]=0x0d;
		ptr[k++]=0x0a;
		Ax_Send(ptr,k);//長さは140のはず
//		debug_puts(" \r\n");
		
	}
	//end of debug
	//debug_puts("Finish reading.");

	return 1;
}

/*************************************************
		デバッグ用詳細履歴読み出しコマンド
*************************************************/
unsigned char PutOut_Detail(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char byte = 128;
	unsigned char slave = 0;
	// param の最初の6ケタは読み出しはじめのページ数
	unsigned long int param0 = (unsigned long int)param[0]-48;
	unsigned long int param1 = (unsigned long int)param[1]-48;
	unsigned long int param2 = (unsigned long int)param[2]-48;
	unsigned long int param3 = (unsigned long int)param[3]-48;
	unsigned long int param4 = (unsigned long int)param[4]-48;
	unsigned long int param5 = (unsigned long int)param[5]-48;
	unsigned long int start_page = 100000*param0 + 10000*param1 + 1000*param2 + 100*param3 + 10*param4 + param5;
/*
	while(start_page >= 0x3FF) {
		start_page = start_page - 0x3FF;
		slave++;
	}
	if(slave>3){
		debug_puts("\nrange error\n");
		Ax_Send("r-g-c-ptd-RangeError1 \r\n",24);
		return 1;
	}
	*/
	// param の7ケタ目が読み出すページ数
	unsigned long int range = (unsigned long int)param[6]-48;
	
	if(start_page<1024){
		start_page = start_page;
		slave=0;
	}
	else if(start_page<2048){
		start_page = start_page-1024;
		slave=1;
	}
	else if(start_page<3072){
		start_page = start_page-2048;
		slave=2;
	}
	else if(start_page<4096){
		start_page = start_page-3072;
		slave=3;
	}
	else{
		Ax_Send("r-g-c-ptd-RangeError1 \r\n",24);
		return 1;
	}
	
	unsigned long int page;
	unsigned char i;
	static unsigned char tlm[128];		// EEPROM読込バッファ 128Byte/page
	unsigned char ptd[256];
	
	for(page = start_page; page < start_page+range; page++)
	{	
		debug_puts("\nDetail page = ");
		debug_putdec(page);
		debug_puts("\tslave = ");
		debug_putdec(slave);
		debug_puts("\n");
		
		IIC0_EEPRESET();
		IIC0_EEPSTOP();
		debug_puts("Reading...\n");
		IIC0_EEPREAD(page,tlm,slave,byte);   // device = 0
		;;
		int k=0;
//		debug_puts("r-g-c-ptd-");	
//		debug_putdec(start_page);
		
		ptd[k++]='r';
		ptd[k++]='-';
		ptd[k++]='g';
		ptd[k++]='-';
		ptd[k++]='c';
		ptd[k++]='-';
		ptd[k++]='p';
		ptd[k++]='t';
		ptd[k++]='d';
		ptd[k++]='-';

		for(i=0;i<SIZE_STATUS;i++){
			ptd[k++]=tlm[i];
//			debug_putc(tlm[i]);
		}
		ptd[k++]=0x20;
		ptd[k++]=0x0d;
		ptd[k++]=0x0a;
		Ax_Send(ptd,k);//長さは140のはず
//		debug_puts(" \r\n");
	}

	return 1;
}

/*************************************************
		デバッグ用API配信データ読み出しコマンド
*************************************************/
unsigned char PutOut_API(volatile unsigned char* param, unsigned char param_length)
{
	unsigned char byte = 32;
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
	
	unsigned long int page;
	unsigned char i,j;
	static unsigned char tlm[32];		// EEPROM読込バッファ 128Byte/page
	unsigned char pta[256];
	
	for(page = start_page; page < start_page+range; page++)
	{	
		debug_puts("\nAPI page = ");
		debug_putdec(page);
		debug_puts("\n");
		
		IIC1_EEPRESET();
		IIC1_EEPSTOP();
		debug_puts("Reading...\n");
		IIC1_EEPREAD(page,tlm,3,byte);   // device = 3
		;;
		int k=0;
//		debug_puts("r-g-c-pta-");
//		debug_putdec(start_page);
		
		pta[k++]='r';
		pta[k++]='-';
		pta[k++]='g';
		pta[k++]='-';
		pta[k++]='c';
		pta[k++]='-';
		pta[k++]='p';
		pta[k++]='t';
		pta[k++]='a';
		pta[k++]='-';
		
		for(i=0;i<byte;i++){
			pta[k++]=tlm[i];
//			debug_putc(tlm[i]);
		}
		
		pta[k++]=0x20;
		pta[k++]=0x0d;
		pta[k++]=0x0a;
		Ax_Send(pta,k);//長さは45のはず
//		debug_puts(" \r\n");
	}

	return 1;
}

/*************************************************
		デバッグ用初期化情報読み出しコマンド
*************************************************/
unsigned char PutOut_Initial(void)
{	
	unsigned char byte = 32;
	unsigned long int page;
	unsigned char i;
	static unsigned char tlm[32];		// EEPROM読込バッファ 128Byte/page
	unsigned char pti[256];
	
	int j=0;
	pti[j++]='r';
	pti[j++]='-';
	pti[j++]='g';
	pti[j++]='-';
	pti[j++]='c';
	pti[j++]='-';
	pti[j++]='p';
	pti[j++]='t';
	pti[j++]='i';
	pti[j++]='-';
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts("Reading(obc time)...");
	IIC1_EEPREAD(OBCTIME_PAGE,tlm,INITIAL_SLAVE,MODE32);   // device = 3
	;;	
	for(i=0;i<byte;i++){
		pti[j++]=tlm[i];
//		debug_putc(tlm[i]);
	}
	debug_puts("done\t");
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts("Reading(error count)...");
	IIC1_EEPREAD(ERRORCOUNT_PAGE,tlm,INITIAL_SLAVE,MODE32);   // device = 3
	;;	
	for(i=0;i<byte;i++){
		pti[j++]=tlm[i];
//		debug_putc(tlm[i]);
	}	
	debug_puts("done\t");
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts("Reading(obc time)...");
	IIC1_EEPREAD(ROUGH_SLAVE_PAGE,tlm,INITIAL_SLAVE,MODE32);   // device = 3
	;;	
	for(i=0;i<byte;i++){
		pti[j++]=tlm[i];
//		debug_putc(tlm[i]);
	}
	debug_puts("done\t");
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts("Reading(obc time)...");
	IIC1_EEPREAD(ROUGH_PAGE,tlm,INITIAL_SLAVE,MODE32);   // device = 3
	;;	
	for(i=0;i<byte;i++){
		pti[j++]=tlm[i];
//		debug_putc(tlm[i]);
	}
	debug_puts("done\t");	
	
	IIC1_EEPRESET();
	IIC1_EEPSTOP();
	debug_puts("Reading(obc time)...");
	IIC1_EEPREAD(API_PAGE,tlm,INITIAL_SLAVE,MODE32);   // device = 3
	;;	
	for(i=0;i<byte;i++){
		pti[j++]=tlm[i];
//		debug_putc(tlm[i]);
	}
	debug_puts("done\t");	

	pti[j++]=0x20;
	pti[j++]=0x0d;
	pti[j++]=0x0a;
	Ax_Send(pti,j);//長さは77のはず

	return 1;
}