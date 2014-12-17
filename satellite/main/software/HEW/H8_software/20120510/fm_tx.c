/* =============================================================================== */
/* 															                       */
/* 				FM TX Program for INVADER BBM				          		       */
/* 															                       */
/* 										Written by Shota Kikuchi			       */
/* 										2012.02.10                                 */
/* 	                                                                               */
/* 										Based on "fmtx.c" for PRISM				   */
/* 															                       */
/* =============================================================================== */


/* ------------------------ */
/*	Pin Definition          */
/*                          */
/*		PA-1	FMPTT       */
/*	 	PA-2	FMCLK       */
/*		PA-3	FMDAT       */
/*		PA-4	FMSTB       */
/*		PA-5	FX_RDYN     */
/*		PA-6	FX_CLK      */
/*		PA-7	FX_TXD      */
/*		PB-0	FX_DET      */
/*		PB-2	FX_M0       */
/*		PB-3	FX_M1       */
/* ------------------------ */


#ifndef FM_TX_C
#define FM_TX_C


#include "iodefine.h"
#include "fm_pindef.h"
#include "fm_tx.h"
#include "timer.h"
//#include "rsc_encode.h"

#define NRZI	1
	// 1:NRZI ON、0:NRZI OFFd
#define SPAN	3

void init_modem(void){
	P3DDR |=  (1 << 1);	// FX_CLK  : OUT
	P3DDR |=  (1 << 2);	// FX_TXD  : OUT
	P3DDR |=  (1 << 3);	// FX_M1   : OUT
	P3DDR &= ~(1 << 0);	// FX_RDYN : IN 

   //FX614をZERO POWER Modeに設定
    FX_M1 = 1;

}

unsigned char init_fm(void) {
	P3DDR |=  (1 << 4);	// FMSTB : OUT
    P3DDR |=  (1 << 5);	// FMDAT : OUT
    P3DDR |=  (1 << 6);	// FMCLK : OUT
	P3DDR |=  (1 << 7);	// FMPTT : OUT
	
	P1DDR |=  (1 << 0);	//FMCWSW : OUT
	P1DDR |=  (1 << 1);	//FMDTSW : OUT

    init_modem();		// モデムの初期化

    fmptt_off();
	
	return 1;
}


void delay_modem(void){
	
    while (FX_RDYN != 0);	// FX_RDYNが0になるまで待つ
}

/* ------------------------------------------------- */
/*   FX614に1ビット送信                              */
/*                                                   */
/*   符号化方式なし or NRZI方式                      */
/*     bitdat : 送信するビット                       */
/*         1 : ライン出力維持、0 : ライン出力反転    */
/* ------------------------------------------------- */

void send_bit(unsigned char bitdat) {
	// NRZI ON
	if (NRZI == 1) {
	    if (bitdat == 0) {
	        stuff = 0;
	    }

	    if (bitdat == FX_TXD) {
	        FX_TXD = 1;
	        FX_CLK = 1;
	        delay_us(10);
	        FX_CLK = 0;
	    } 
	    else {
	        FX_TXD = 0;
	        FX_CLK = 1;
	        delay_us(10);
	        FX_CLK = 0;
	    }
	}
	
	// NRZI OFF（普通にビット送信）
	else {
		if (bitdat == 0) {
			FX_TXD = 0;
			FX_CLK = 1;
	        delay_us(10);
	        FX_CLK = 0;
	    }
		else {
			FX_TXD = 1;
	        FX_CLK = 1;
	        delay_us(10);
	        FX_CLK = 0;
	    }
	}
}

void crcbit(unsigned char tbyte){
    int fcshi0, fcslo0;
    int status0;

    if((fcshi & 0x01) == 0x01)
        fcshi0 = 1;
    else
        fcshi0 = 0;

    if((fcslo & 0x01) == 0x01)
        fcslo0 = 1;
    else
        fcslo0 = 0;

    fcshi = fcshi >> 1;
    fcslo = fcslo >> 1;

    if(fcshi0)
        fcslo |= 0x80;

    if(fcslo0)
        status0 = 1;
    else
        status0 = 0;

    if (((status0 & 0x01)^(tbyte)) == 0x01)
    {
        fcshi = fcshi^0x84;
        fcslo = fcslo^0x08;
    }
}

/* ------------------------------------------------- */
/*   FX614にchar型1バイト送信                        */
/*                                                   */
/*   bytedat : 送信するデータ                        */
/* ------------------------------------------------- */

void send_byte(unsigned char bytedat)
{
    int k;
    unsigned char bt;
    
    //8ビット分行う
    for(k = 0 ; k < 8 ; k++)
    {
		//下位ビットから送信するため、byteの一番右側のbitから送信処理を行う。
    	
        bt = bytedat & 0x01;
        
		if((fcsflag_send_fm == 0)&&(flag_send_fm == 0))
            crcbit(bt);
        if(bt == 0)
            send_bit(0);
        else
        {
            send_bit(1);
            stuff++;
            
            // stuff : ビットスタッフィング用のカウンタ
            // プリアンブル・ポストアンブル以外で1が5回以上続いた場合に"0"を1bit挿入する。
            // flag:プリアンブル送信中か判定。プリアンブル送信時はビットスタッフィングを行わない。
            if((flag_send_fm == 0) && (stuff == 5))
            {
                delay_modem();
                send_bit(0);
            }
        }
		
        bytedat = bytedat >> 1;		//次のbitに移る
		
		//delay_ms(1);
        delay_modem();
		
    }
}

/* -------------------------------------------------- */
/*   FX614にエンコードしたパケットデータを送信        */
/*   (RSC付加→プリアンブル付加→NRZI変換)            */
/*                                                    */
/*     str : 送信するデータ列                         */
/*     str_length : 送信するデータ長さ                */
/* -------------------------------------------------- */

void send_packet(volatile unsigned char *str, int str_length)
{
    int i, plength;
    
    FX_CLK = 0;		// TXのリタイミング
    
    stuff = 0;		// ビットスタッフィング用カウンタ初期化
    
    // 送信前にPTTをON
    fmptt_on();
    
    flag_send_fm = 1;		// プリアンブル送信中
    
    // FX614を送信モードに設定
    FX_M1 = 0;
	delay_ms(20);
    
//     // プリアンブル送信によりクロックの同期をとる
//     for ( i = 0 ; i < FmPreamble ; i++ )
//     {
//         send_byte(0x7e);
//     }
//     flag = 0;
    
    // データ送信
    if(str_length > 0) {
		for(i=0; i<str_length; i++){
			send_byte(str[i]);
			//debug_putc(str[i]);
			
			//sci1_putc(str[i]);
			//P5.DR.BYTE = str[i];	//LEDに出力
			
// 			send_byte(rsc_encode(str, str_length)[i]);
// 			sci1_putc(rsc_encode(str, str_length)[i]);
// 			P5.DR.BYTE = rsc_encode(str, str_length)[i];	//LEDに出力
		}
		//debug_puts("\r\n\n");
	}
	
    flag_send_fm = 1;		// ポストプリアンブル送信
    
    // ポストプリアンブル送信;;
//     send_byte(0x7e);
//     send_byte(0x7e);
//     send_byte(0x7e);
//     send_byte(0x7e);
    
    // 送信完了したらPTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614をZero Power modeに設定
    FX_M1 = 1;

}

unsigned char AX_protocol( volatile unsigned char *message_global, int str_length) {
	if(FX_RDYN != 0){
		debug_puts("\n -*-*-*-*-*-*- TX's RDYN error!!! -*-*-*-*-*-*- \n");
		FM_mode=0;
		FinishSendMessage();
		return 0;
	}
	FM_mode=4;
	stuff=0;
	int i;
	fcslo = fcshi = 0xff;
/* 初期設定									*/
	FMCWSW 	= 1;//0
	FX_CLK = 0;						// TXのリタイミング
	fmptt_on();						// 送信前にPTTをON
	flag_send_fm = 1;			// プリアンブル送信中
	delay_ms(20);
/* 初期設定おわり						*/

/* Start Sending Flag				*/
	flag_send_fm = 1;
	fcsflag_send_fm = 0;
	for(i=0;i<30;i++){
		send_byte(0x7e);
	}
	flag_send_fm = 0;
/* Finish Sending Flag			*/

/* Start Sending Address		*/

    char dest1 = 0x94;
    char dest2 = 0xa2;
    char dest3 = 0x62;
    char dest4 = 0xb4;
    char dest5 = 0x96;
    char dest6 = 0x98;
    char dest7 = 0x60;

/*       0x94 = 10010100 >> 01001010 = 0x4a 'J'           */
/*       0xa2 = 10100010 >> 01010001 = 0x51 'Q'           */
/*       0x62 = 01100010 >> 00110001 = 0x31 '1'           */
/*       0xb4 = 10110100 >> 01011010 = 0x5a 'Z'           */
/*       0x96 = 10010110 >> 01001011 = 0x4b 'K'           */
/*       0x98 = 10011000 >> 01001100 = 0x4c 'L'           */
/*       0x60 SSID                                        */
    char src1 = 0x94;//0xaa;
    char src2 = 0xa2;//0xa8;
    char src3 = 0x62;//0xa6;
    char src4 = 0xb4;//0x86;
    char src5 = 0x96;//0x82;
    char src6 = 0x96;//0x9c;
    char src7 = 0x61;

/*       0x94 = 10010100 >> 01001010 = 0x4a 'J'           */
/*       0xa2 = 10100010 >> 01010001 = 0x51 'Q'           */
/*       0x62 = 01100010 >> 00110001 = 0x31 '1'           */
/*       0xb4 = 10110100 >> 01011010 = 0x5a 'Z'           */
/*       0x96 = 10010110 >> 01001011 = 0x4b 'K'           */
/*       0x96 = 10010110 >> 01001011 = 0x4b 'K'           */


/************↓PRISMのもの↓************/
//    char dest1 = 0x94;
//    char dest2 = 0xa2;//0x9e;
//    char dest3 = 0x62;//0x62;
//    char dest4 = 0xb2;//0xb4;
//    char dest5 = 0x86;//0xa0;
//    char dest6 = 0xb0;//0x8e;
//    char dest7 = 0x60;

/*       0x94 = 10010100 >> 01001010 = 0x4a 'J'           */
/*       0xa2 = 10100010 >> 01010001 = 0x51 'Q'           */
/*       0x62 = 01100010 >> 00110001 = 0x31 '1'           */
/*       0xb2 = 10110010 >> 01011001 = 0x59 'Y'           */
/*       0x86 = 10000110 >> 01000011 = 0x43 'C'           */
/*       0xb0 = 10110000 >> 01011000 = 0x58 'X'           */
/*       0x60 SSID                                        */
//    char src1 = 0x94;//0xaa;
//    char src2 = 0xa2;//0xa8;
//    char src3 = 0x62;//0xa6;
//    char src4 = 0xb2;//0x86;
//    char src5 = 0xb4;//0x82;
//    char src6 = 0xae;//0x9c;
//    char src7 = 0x61;

/*       0x94 = 10010100 >> 01001010 = 0x4a 'J'           */
/*       0xa2 = 10100010 >> 01010001 = 0x51 'Q'           */
/*       0x62 = 01100010 >> 00110001 = 0x31 '1'           */
/*       0xb2 = 10110010 >> 01011001 = 0x59 'Y'           */
/*       0xb4 = 10110100 >> 01011010 = 0x5a 'Z'           */
/*       0xae = 10101110 >> 01010111 = 0x57 'W'           */

/************↑PRISMのもの↑************/

	send_byte(dest1);
	send_byte(dest2);
	send_byte(dest3);
	send_byte(dest4);
	send_byte(dest5);
	send_byte(dest6);
	send_byte(dest7);

	send_byte(src1);
	send_byte(src2);
	send_byte(src3);
	send_byte(src4);
	send_byte(src5);
	send_byte(src6);
	send_byte(src7);

/* Finish Sending Address		*/

/* Start Sending Control		*/
	char control = 0x03;//Control Field : 0000 0011
	send_byte(control);


/* Finish Sending Control		*/

/* Start Sending PID				*/
	char PID		= 0xF0;//PID FIeld      : 1111 0000
	send_byte(PID);

/* Finish Sending PID				*/

/* Start Sending Data				*/
	if(str_length>0){
		for(i=0; i<str_length; i++){
			send_byte(message_global[i]);
		}
	}

/* Finish Sending Data 			*/

/* Start Sending FCS				*/
	fcsflag_send_fm = 1;
    fcslo = fcslo^0xff; 
    fcshi = fcshi^0xff;
	send_byte(fcslo);
	send_byte(fcshi);
/* Finish Sending FCS				*/

/* Start Sending Flag				*/
	flag_send_fm = 1;
	fcsflag_send_fm = 0;
	send_byte(0x7e);
	send_byte(0x7e);
	send_byte(0x7e);
	send_byte(0x7e);
	send_byte(0x7e);
	send_byte(0x7e);
	send_byte(0x7e);
	send_byte(0x7e);
	flag_send_fm = 0;
/* Finish Sending Flag			*/

/************終了設定**************/
// 送信完了したらPTT OFF
	fmptt_off();

	FX_TXD = 0;
	
//	FMCWSW 	= 0;//1

	delay_ms(200);
	FM_sender_num++;
	if(FM_sender_num>FM_BUFFER_MAX-1){
		FM_sender_num=0;
	}


	if(FM_sender_num>=FM_buffer_num){
		if(DT_mode==1){
			FM_mode=0;
			FM_sender_num=0;
			FM_buffer_num=0;
			DT_mode=2;
			TransmitDigitalker();
			DT_time=0;
		}
		else{
			FM_mode=3;
			FM_sender_num=0;
			FM_buffer_num=0;
		}		
	}
	else{
		FM_mode=2;
	}
	debug_puts("\n FM_sender_num : ");
	debug_putdec(FM_sender_num);
	debug_puts("\n FM_buffer_num : ");
	debug_putdec(FM_buffer_num);

	debug_puts("\n 32FM_mode : ");
	debug_putdec(FM_mode);
/**********終了設定おわり**********/
	return 1;
}

unsigned char Ax_Send( volatile unsigned char *message_global, int str_length) {
	int i;
	debug_puts("\n message length... \t");
	debug_putdec(str_length);
	debug_puts("\n/////////\n");
	for(i=0;i<str_length;i++){
		FM_buffer[FM_buffer_num][i]=message_global[i];
		debug_puthex(message_global[i]);
		debug_puts(",");
	}
	debug_puts("\n---------\n");
	for(i=0;i<str_length;i++){
		debug_putc(message_global[i]);
	}
	debug_puts("\n/////////\n");
	FM_buffer_length[FM_buffer_num]=str_length;
	FM_buffer_num++;
	if(FM_buffer_num>FM_BUFFER_MAX-1){
		FM_buffer_num=0;
	}
	if(FM_mode==0||FM_mode==3){
		FM_mode=1;	//モード0ならtonさせる
	}
	return 1;
}
unsigned char TX(void){
	/*デジトーカーに対する操作*/	
	if(FM_mode==0){
		debug_puts("*");
		if(DT_mode==0){
			/*スルー*/
			return 1;
		}
		else if(DT_mode==1){
				DT_mode=2;
				return 1;
		}
		else if(DT_mode==2){
			TransmitDigitalker();
			DT_time=0;
			return 1;
		}
		else if(DT_mode==3){
			debug_puts("(");
			debug_putdec(DT_time);
			debug_puts(")");
			FMCWSW=1;//0
			FMDTSW=0;
			fmptt_on();
			DT_time++;
			if(DT_time>MAX_DT_time){
				DegitalkerOFF();
				PronounceMissionAbort();
			}
			return 1;
		}
	}
	else{
		debug_puts("#");
		if((DT_mode==2)||(DT_mode==3)){
			DT_mode=1;
			return 1;
		}
	}
	if(DT_mode==4){
		DT_mode=0;
		Ax_Send("r-g-c-dof \r\n",12);
//		FinishSendMessage();		
	}
	
	/*FMに対する操作*/
	if(FM_mode==1){
		debug_puts("\n 1FM_mode : ");
		debug_putdec(FM_mode);
		debug_puts("\n");
		
		TransmitMessage();
		
		return 1;
	}
	else if(FM_mode==2){
						
		debug_puts("\n 2FM_mode : ");
		debug_putdec(FM_mode);
		
		FM_mode=4;
		AX_protocol(FM_buffer[FM_sender_num],FM_buffer_length[FM_sender_num]);
		/*
		FM_sender_num++;
		if(FM_sender_num>FM_BUFFER_MAX){
			FM_sender_num=0;
		}
		*/
		/*
		if(FM_sender_num==FM_buffer_num){
			FM_mode=3;
		}
		else{
			FM_mode=1;
		}
		*/
		
		return 1;
	}
	else if(FM_mode==3){
							
		debug_puts("\n 3FM_mode : ");
		debug_putdec(FM_mode);
		debug_puts("\n");
		
		FinishSendMessage();
		return 1;
	}
	return 1;
}

void on_off_ms(int span_ms) {
	
	FX_CLK = 0;		// TXのリタイミング
    
    stuff = 0;		// ビットスタッフィング用カウンタ初期化
    
    // 送信前にPTTをON
    fmptt_on();
    
    flag_send_fm = 1;		// プリアンブル送信中
    
    // FX614を送信モードに設定
    FX_M1 = 0;
	delay_ms(20);
		
	FX_TXD = 1;
	delay_ms(span_ms);
	FX_TXD = 0;
	delay_ms(span_ms);
	
	// 送信完了したらPTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614をZero Power modeに設定
    FX_M1 = 1;

	
	
}

void on_off_sec(int sec) {
	
	int i;
	
	FX_CLK = 0;		// TXのリタイミング
    
    stuff = 0;		// ビットスタッフィング用カウンタ初期化
    
    // 送信前にPTTをON
    fmptt_on();
    
    flag_send_fm = 1;		// プリアンブル送信中
    
    // FX614を送信モードに設定
    FX_M1 = 0;
	delay_ms(20);
		
	
	FX_TXD = 1;
	for(i=0; i<sec*100; i++) {
		delay_ms(10);
	}
	FX_TXD = 0;
	for(i=0; i<sec*100; i++) {
		delay_ms(10);
	}
	
	// 送信完了したらPTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614をZero Power modeに設定
    FX_M1 = 1;
}

void on_off_us(int span_us) {
	
	FX_CLK = 0;		// TXのリタイミング
    
    stuff = 0;		// ビットスタッフィング用カウンタ初期化
    
    // 送信前にPTTをON
    fmptt_on();
    
    flag_send_fm = 1;		// プリアンブル送信中
    
    // FX614を送信モードに設定
    FX_M1 = 0;
	delay_ms(20);
	
	
	FX_TXD = 1;
	delay_us(span_us);
	FX_TXD = 0;
	delay_us(span_us);
	
	// 送信完了したらPTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614をZero Power modeに設定
    FX_M1 = 1;
}

#endif