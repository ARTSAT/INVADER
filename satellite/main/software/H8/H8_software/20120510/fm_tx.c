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
	// 1:NRZI ON�A0:NRZI OFFd
#define SPAN	3

void init_modem(void){
	P3DDR |=  (1 << 1);	// FX_CLK  : OUT
	P3DDR |=  (1 << 2);	// FX_TXD  : OUT
	P3DDR |=  (1 << 3);	// FX_M1   : OUT
	P3DDR &= ~(1 << 0);	// FX_RDYN : IN 

   //FX614��ZERO POWER Mode�ɐݒ�
    FX_M1 = 1;

}

unsigned char init_fm(void) {
	P3DDR |=  (1 << 4);	// FMSTB : OUT
    P3DDR |=  (1 << 5);	// FMDAT : OUT
    P3DDR |=  (1 << 6);	// FMCLK : OUT
	P3DDR |=  (1 << 7);	// FMPTT : OUT
	
	P1DDR |=  (1 << 0);	//FMCWSW : OUT
	P1DDR |=  (1 << 1);	//FMDTSW : OUT

    init_modem();		// ���f���̏�����

    fmptt_off();
	
	return 1;
}


void delay_modem(void){
	
    while (FX_RDYN != 0);	// FX_RDYN��0�ɂȂ�܂ő҂�
}

/* ------------------------------------------------- */
/*   FX614��1�r�b�g���M                              */
/*                                                   */
/*   �����������Ȃ� or NRZI����                      */
/*     bitdat : ���M����r�b�g                       */
/*         1 : ���C���o�͈ێ��A0 : ���C���o�͔��]    */
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
	
	// NRZI OFF�i���ʂɃr�b�g���M�j
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
/*   FX614��char�^1�o�C�g���M                        */
/*                                                   */
/*   bytedat : ���M����f�[�^                        */
/* ------------------------------------------------- */

void send_byte(unsigned char bytedat)
{
    int k;
    unsigned char bt;
    
    //8�r�b�g���s��
    for(k = 0 ; k < 8 ; k++)
    {
		//���ʃr�b�g���瑗�M���邽�߁Abyte�̈�ԉE����bit���瑗�M�������s���B
    	
        bt = bytedat & 0x01;
        
		if((fcsflag_send_fm == 0)&&(flag_send_fm == 0))
            crcbit(bt);
        if(bt == 0)
            send_bit(0);
        else
        {
            send_bit(1);
            stuff++;
            
            // stuff : �r�b�g�X�^�b�t�B���O�p�̃J�E���^
            // �v���A���u���E�|�X�g�A���u���ȊO��1��5��ȏ㑱�����ꍇ��"0"��1bit�}������B
            // flag:�v���A���u�����M��������B�v���A���u�����M���̓r�b�g�X�^�b�t�B���O���s��Ȃ��B
            if((flag_send_fm == 0) && (stuff == 5))
            {
                delay_modem();
                send_bit(0);
            }
        }
		
        bytedat = bytedat >> 1;		//����bit�Ɉڂ�
		
		//delay_ms(1);
        delay_modem();
		
    }
}

/* -------------------------------------------------- */
/*   FX614�ɃG���R�[�h�����p�P�b�g�f�[�^�𑗐M        */
/*   (RSC�t�����v���A���u���t����NRZI�ϊ�)            */
/*                                                    */
/*     str : ���M����f�[�^��                         */
/*     str_length : ���M����f�[�^����                */
/* -------------------------------------------------- */

void send_packet(volatile unsigned char *str, int str_length)
{
    int i, plength;
    
    FX_CLK = 0;		// TX�̃��^�C�~���O
    
    stuff = 0;		// �r�b�g�X�^�b�t�B���O�p�J�E���^������
    
    // ���M�O��PTT��ON
    fmptt_on();
    
    flag_send_fm = 1;		// �v���A���u�����M��
    
    // FX614�𑗐M���[�h�ɐݒ�
    FX_M1 = 0;
	delay_ms(20);
    
//     // �v���A���u�����M�ɂ��N���b�N�̓������Ƃ�
//     for ( i = 0 ; i < FmPreamble ; i++ )
//     {
//         send_byte(0x7e);
//     }
//     flag = 0;
    
    // �f�[�^���M
    if(str_length > 0) {
		for(i=0; i<str_length; i++){
			send_byte(str[i]);
			//debug_putc(str[i]);
			
			//sci1_putc(str[i]);
			//P5.DR.BYTE = str[i];	//LED�ɏo��
			
// 			send_byte(rsc_encode(str, str_length)[i]);
// 			sci1_putc(rsc_encode(str, str_length)[i]);
// 			P5.DR.BYTE = rsc_encode(str, str_length)[i];	//LED�ɏo��
		}
		//debug_puts("\r\n\n");
	}
	
    flag_send_fm = 1;		// �|�X�g�v���A���u�����M
    
    // �|�X�g�v���A���u�����M;;
//     send_byte(0x7e);
//     send_byte(0x7e);
//     send_byte(0x7e);
//     send_byte(0x7e);
    
    // ���M����������PTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614��Zero Power mode�ɐݒ�
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
/* �����ݒ�									*/
	FMCWSW 	= 1;//0
	FX_CLK = 0;						// TX�̃��^�C�~���O
	fmptt_on();						// ���M�O��PTT��ON
	flag_send_fm = 1;			// �v���A���u�����M��
	delay_ms(20);
/* �����ݒ肨���						*/

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


/************��PRISM�̂��́�************/
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

/************��PRISM�̂��́�************/

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

/************�I���ݒ�**************/
// ���M����������PTT OFF
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
/**********�I���ݒ肨���**********/
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
		FM_mode=1;	//���[�h0�Ȃ�ton������
	}
	return 1;
}
unsigned char TX(void){
	/*�f�W�g�[�J�[�ɑ΂��鑀��*/	
	if(FM_mode==0){
		debug_puts("*");
		if(DT_mode==0){
			/*�X���[*/
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
	
	/*FM�ɑ΂��鑀��*/
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
	
	FX_CLK = 0;		// TX�̃��^�C�~���O
    
    stuff = 0;		// �r�b�g�X�^�b�t�B���O�p�J�E���^������
    
    // ���M�O��PTT��ON
    fmptt_on();
    
    flag_send_fm = 1;		// �v���A���u�����M��
    
    // FX614�𑗐M���[�h�ɐݒ�
    FX_M1 = 0;
	delay_ms(20);
		
	FX_TXD = 1;
	delay_ms(span_ms);
	FX_TXD = 0;
	delay_ms(span_ms);
	
	// ���M����������PTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614��Zero Power mode�ɐݒ�
    FX_M1 = 1;

	
	
}

void on_off_sec(int sec) {
	
	int i;
	
	FX_CLK = 0;		// TX�̃��^�C�~���O
    
    stuff = 0;		// �r�b�g�X�^�b�t�B���O�p�J�E���^������
    
    // ���M�O��PTT��ON
    fmptt_on();
    
    flag_send_fm = 1;		// �v���A���u�����M��
    
    // FX614�𑗐M���[�h�ɐݒ�
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
	
	// ���M����������PTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614��Zero Power mode�ɐݒ�
    FX_M1 = 1;
}

void on_off_us(int span_us) {
	
	FX_CLK = 0;		// TX�̃��^�C�~���O
    
    stuff = 0;		// �r�b�g�X�^�b�t�B���O�p�J�E���^������
    
    // ���M�O��PTT��ON
    fmptt_on();
    
    flag_send_fm = 1;		// �v���A���u�����M��
    
    // FX614�𑗐M���[�h�ɐݒ�
    FX_M1 = 0;
	delay_ms(20);
	
	
	FX_TXD = 1;
	delay_us(span_us);
	FX_TXD = 0;
	delay_us(span_us);
	
	// ���M����������PTT OFF
    fmptt_off();
    
    FX_TXD = 0;
    
   // FX614��Zero Power mode�ɐݒ�
    FX_M1 = 1;
}

#endif