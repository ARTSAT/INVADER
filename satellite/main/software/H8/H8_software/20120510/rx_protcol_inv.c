/*********************************************************
 * †
 *Created by Skyhorse（岡田 空馬）,2012/06/01
 *Copyright (c) 2012~. All rights reserved.
 *
 * 受信用関数の詰め合わせ
 * void rx_init(void)
 *  受信機の初期化とモデムの設定を行う。電源投入時に一回呼び出すだけでよい。
 *
 * int get_data(void)
 *  データの受信、並びにデコードを行う。返り値はエラーコードである（正常受信の場合は0を返す）。
 *  0   正常受信
 *  1   プリアンプルエラー（プリアンブルを検出できなかった）
 *  2
 *  以下未実装
 *
 *********************************************************/
/*データ長＝147?*/

// Port Definition for MX614  ******************************/
#include"Rx_protcol.h"
#include"sci.h"
#include"intrinsic.h"
#include "timer.h"
#include "inthandler.h"
/*
#include"rx_pin_as.h"      	//MX_M0など、モデムの端子をH8のポートに割り当て 江
//#include"rsc_decode.c"   	//RSCのデコーダ、エンコーダ、テストプログラムが入っている。江			
						 	//中ではRSC付加後１４４文字と書いてあったが、１４７じゃないのかね。　江
#include"comm_buffer.c"    	//com_UPLINK_BUFFER_LENGTHを147に。バッファの長さを指定　江
#include"timer.h"			//delaymsとdelayusを定義。H8のITUとかいうポートを使っている　江
#include"sci.h"			 	//H8のシリアル通信用の関数の定義。debug_putsとか。sci0.cやsci1.cとちょっと違う　江
#include "Ax_to_info.h"


#define ONE_BIT_CYCLE  350
#define TEST_1BIT 1
#define info_length 204
/***********************************************************/
//
/* Number of cycles which equals to one bit time *********/ 
/* Calculation : 121 / 10 * CLOCK ************************/
/* For example : 10MHz -> 121, 4MHz -> 48 ****************/
int i,bit=0;				//bitは何ビット取得したか。デバッグ用。

void init_itu(void){	
	ITU0.TCR.BYTE=0x43;	//Φ/8 GRA 立ち上がり
	ITU0.GRA=107;		//50us 理想的には99
	ITU0.GRB=695;		//333us
	ITU.TSTR.BIT.STR0=1;	//タイマスタート
}
/**
 * bitin モデムからのデータ入力(FX_RXDピンのHigh/Low)を監視する関数
 * データはNRZI形式(受け取るbitの状態が変わらなければ1を、変われば
 * 0を出力する。)である。
 */
static int oldstate; //前のビット 
unsigned int kbit; 
int shiftbuff=0;//
int fcs_error=0;

int bitin(void){
	set_imask_ccr(1);// 割り込み禁止
	TESTPIN = 0;	/*1bitサイクルの計算のためのピン出力*/
	for(kbit = 0 ; kbit < ONE_BIT_CYCLE ; kbit++){ 
		if(FX_RXD != oldstate){			
            oldstate = FX_RXD;
            TESTPIN = 1;
			delay_us_rx(800);
			//delay_us_rx(1800);//430//元800
            return 0; // state changed 
		}	
	}
	TESTPIN = 1;
	//delay_us_rx(345);//元100,82
	delay_us_rx(30);//82
	return 1; // state did not change 
}
/*************
旧bitinは20130803以前を参照
*************/
/************************************************
 *function get_byte()
 *(bitinをまとめて、　江)受信した1byteを返す
 ************************************************/

unsigned char get_byte(int mode){				//プリアンブル受信時に1　僕はこの辺はよく把握していない　江
  	unsigned char bitbuff;
  	unsigned char bytebuff=0;
  	unsigned char bitbuff_5th1;
	//int shiftbuff=0;
  	for(i=0;i<8;i++){							//常に８回行う。江
    	bitbuff=bitin();						//1bit受信、0なら0、1なら1.冗長じゃない？　江
    	if(mode==0){							/*プリアンブルモードでないならビットシフトの処理 //をしてから。江			　*/
    											/*ビットシフトの処理というのは、５回連続で１が来たら次のビットを捨てる処理　江*/
      		if(bitbuff) shiftbuff++;
      		else shiftbuff=0;
			if(shiftbuff==5){					//5回連続1を検出する //5回連続bitinで1を検出する　江
				shiftbuff=0;
				bitbuff_5th1=bitin();			//次の1bitが0だった場合は捨てる。1であるならポストアンブルの可能性
				if(bitbuff_5th1==1){
					bytebuff=bytebuff>>1;		//上位ビットから入れていく。1bit左にシフト    //ここから３行はbit→byteの処理。江
    				bytebuff=bytebuff&0x7f;		//一番下の桁を0にマスク
   					bytebuff+=(bitbuff_5th1)*(0x80);
					bit++;
					i++;
				}
      		}
    	}
    	bytebuff=bytebuff>>1;					//上位ビットから入れていく。1bit左にシフト    //ここから３行はbit→byteの処理。江
    	bytebuff=bytebuff&0x7f;					//一番下の桁を0にマスク
    	bytebuff+=(bitbuff)*(0x80);
		bit++;
  	}											//ここまでを８回繰り返す　江
  	return bytebuff;							//値を返す
}

/*********************************
*function get_buff()
*受信データを配列に格納する
**********************************/
//配列とは、uplink_rx_buff[147]。comm_buffer.c内で定義されている。147で１パケット。　江
//はじめにプリアンブルでの同期を行う。プリアンブルは0x7e(01111110)。preambuffが　江
//これに合致するまで、１ビットずつずらし続ける。江
//次にスタートフラグを探す。見つけたら配列にデータを格納。江

int get_buff(void){										//ここで最低プリアンブル５０回分のビットを取得
	unsigned char preambuff;							//先ず1byte受信	//get_byte(１)ならプリアンブル。つまりビットシフト処理はしない。　江
	int j;
	int count_a=0;
	int count_b=0;
	unsigned char starbuf;
	unsigned char preamble_flag=0x0f;
	for(j=0;j<4;j++){
		preambuff= get_byte(1);		
		for(i=0;i<16;i++){									//プリアンブルは８回流す　江
	    	if(preambuff!=0x7e){							//プリアンブルに同期した時点で処理を止める　江
				preambuff=preambuff<<1;						//1bitずつずらして同期するまで見る
				preambuff=preambuff&0xfe;					//1bit目を0でマスク　江
				preambuff+=bitin();							//これまでの３行でpreambuffが1bit後ろにずれる。　江
			}
		 
		}
		if(preambuff!=0x7e){								//8bitずらして上手く行かなかったらエラーを返す
			return 1;										//return 1 はプリアンブル取得エラー　江
		}
	}
	while(preambuff==0x7e) preambuff=get_byte(1);		//プリアンブルが終わるまで受信し続ける
	uplink_rx_buff[0]=preambuff;
	for(j=1;j<com_UPLINK_BUFFER_LENGTH;j++)			//受信データを配列に格納
	uplink_rx_buff[j]=get_byte(0);
/*
  debug_puts("last preamble...");
  debug_puts("\n");
  debug_puthex(uplink_rx_buff[0]);
  debug_puts(" ");
  debug_puthex(uplink_rx_buff[1]);
  debug_puts("\n");
  debug_puts("last preamble end");
  debug_puts("\n");
  */
	return 0;											//正常終了
}

void precrc_bit(unsigned char bytedat){
	int k;
	unsigned char bt;
	for(k = 0 ; k < 8 ; k++){
		bt = bytedat & 0x01;
		crcbit(bt);
		bytedat = bytedat >> 1;
	}
}

int crc_length(unsigned char *textstring){
	int i;
	for(i=0;i<com_UPLINK_BUFFER_LENGTH;i++){
//		if(textstring[i-5]==0x20){
			if(textstring[i-4]=='\r'){
//				if(textstring[i-3]=='\n'){
					if(textstring[i]==0x7e){
						break;
					}
//				}
			}
//		}
	}
	return (i+1);
}

void Ax_to_info(unsigned char *textstring,int length){
	int i=0;
	int count_flags=0;
	fcslo = fcshi = 0xff;

//	int StrLength=StrLen(textstring);

/*****フラグの数を数えておく*****/
/*	for(i=0;i<StrLength;i++){
		if(textstring[i]==0x7e){
			count_flags++;
		}
	}		
	i=0;
*/
/*****フラグを回収*****//*
	int start_flag=i;
//////	debug_puts("\r\n preamble...");
//////	debug_puthex(textstring[i]);
	if(textstring[i]!=0x7e){
		debug_puts("\r\n preamble 0x7e is not correct...");
	}
	i++;*/
/*****アドレスを回収*****/
	int start_address=i;
//////	debug_puts("\r\n address...");
	for(i=start_address;i<start_address+14;i++){
//////		debug_puthex(textstring[i]);
		precrc_bit(textstring[i]);
	}
/*****コントロールを回収*****/
	int start_control=i;
//////	debug_puts("\r\n control...");
	for(i=start_control;i<start_control+1;i++){
//////		debug_puthex(textstring[i]);
		precrc_bit(textstring[i]);		
	}
/*****PIDを回収*****/
	int start_PID=i;
//////	debug_puts("\r\n PID...");
	for(i=start_PID;i<start_PID+1;i++){
//////		debug_puthex(textstring[i]);
		precrc_bit(textstring[i]);	
	}
/*****Infoを回収*****/
	int start_info=i;
//	info_length=StrLength-2-2-start_info;
	int j;
	info=(char *)malloc(sizeof(char) * (length-20));
	j=0;
//////	debug_puts("\r\n info...");
	for(i=start_info,j=0;i<start_info+(length-19);i++,j++){
//////		debug_puthex(textstring[i]);
		info[j]=textstring[i];
		precrc_bit(textstring[i]);	
//		j++;
	}

/*****送られてきたinfoまでの文字列を元にFCSの計算*****/
	fcslo = fcslo^0xff; 
	fcshi = fcshi^0xff;

/*****送られてきたFCSを回収。FCSチェック。合わないなら-1を返す*****/

	int hoge=0;
	int start_FCS=i;	
//	debug_puts("\r\n FCS...");
//	debug_puthex(textstring[i]);
	if(textstring[i]!=fcslo){
		hoge++;
	}
	i++;
//	debug_puthex(textstring[i]);
	if(textstring[i]!=fcshi){
		hoge++;
	}
	i++;	
//	debug_puts("\r\n fcs check...");
 //	debug_puthex(fcslo);
//	debug_puts("\t");
//	debug_puthex(fcshi);
//  	debug_puts("\n");

	if(hoge!=0){
/**FCSが合っていない時の対処法を書いて置く事 **/
  		debug_puts("FCS error \n");
		fcs_error=1;

	}
	else{
		debug_puts("FCS OK! \n");
		fcs_error=0;
	}
//	debug_puts("                                                 \n");
    for(i=0;i<length;i++)debug_putc(textstring[i]);
}

int get_data(void){								//この関数が上のたくさんの関数のまとめ。
	static int a=0;
  	int errnumb=0;	
  	if(!a){										//0:エラーなし。1:プリアンブルエラー。2:スタートフラグエラー　江
  		comm_buffer_init();						//comm_buffer.c内で定義されている。江
  		init_itu();
	}											//タイマ初期化、タイマ開始　江
	SCI0.SSR.BYTE &= (unsigned char)(~0x80);	// 送信割り込み禁止
//	SCI1.SSR.BYTE &= (unsigned char)(~0x80);	// 送信割り込み禁止
//	SCI2.SSR.BYTE &= (unsigned char)(~0x80);	// 送信割り込み禁止
	ITU.TISRA.BIT.IMIEA2 = 0;		// タイマ割り込み禁止
	ITU.TISRA.BIT.IMIEA1 = 0;		// タイマ割り込み禁止
	set_imask_ccr(1);// 割り込み禁止

  	errnumb=get_buff();							//１パケット取得&&エラーナンバー取得　江

	set_imask_ccr(0);// 割り込み許可
	SCI0.SCR.BYTE = 0x70;		// 送受信、受信割り込みのみ許可
//	SCI1.SCR.BYTE = 0x70;		// 送受信、受信割り込みのみ許可
//	SCI2.SCR.BYTE = 0x70;		// 送受信、受信割り込みのみ許可
	ITU.TISRA.BIT.IMIEA2 = 1;	// タイマ割り込み許可
	ITU.TISRA.BIT.IMIEA1 = 1;   // タイマ割り込み許可
	if(errnumb==1){
	  	a=1;
    	return 1;
	}
	
	int length=crc_length(uplink_rx_buff);
/*	
	debug_puts("\r\n");
	debug_puts("cathed...");
	debug_puts("\n");
	int i;
	for(i=0;i<length;i++){
		debug_puthex(uplink_rx_buff[i]);
		debug_puts(" ");
	}
	debug_puts("\n");
	debug_puts("end...");
	debug_puts("\n");
	*/
	Ax_to_info(uplink_rx_buff,length);
/*************************************************
		受信が成功した際のバッファへの書き込み
*************************************************/
  if(fcs_error==0){
	sci1_puts_rx(info,length-20);  
	return 0;
  }
  return 1;
}

void rx_init(void){							//初期化関数
	P4DDR = 0xCF; 							//FX_DET、FX_RXDを入力に設定
	// init_timer();						//タイマーを使用可能にする　←使わないようにした　江
	pll_init_rx(
		0xA00,		//refernce divider
		0x0E,			//swallow_counter
		0x30A,		//programable_counter
		3,			//charge_pump_tx
		3,			//charge_pump_rx
		0,			//lock_detector
		0,			//standby_control_tx
		1			//standby_control_rx
	);		//PLL設定を書き込む
	FX_M0 = 0;								//FSKのモード
	FX_CLK_RX = 1;							//FSK Operation without Rx Data Retiming
}
