/*********************************************************
 * ��
 *Created by Skyhorse�i���c ��n�j,2012/06/01
 *Copyright (c) 2012~. All rights reserved.
 *
 * ��M�p�֐��̋l�ߍ��킹
 * void rx_init(void)
 *  ��M�@�̏������ƃ��f���̐ݒ���s���B�d���������Ɉ��Ăяo�������ł悢�B
 *
 * int get_data(void)
 *  �f�[�^�̎�M�A���тɃf�R�[�h���s���B�Ԃ�l�̓G���[�R�[�h�ł���i�����M�̏ꍇ��0��Ԃ��j�B
 *  0   �����M
 *  1   �v���A���v���G���[�i�v���A���u�������o�ł��Ȃ������j
 *  2
 *  �ȉ�������
 *
 *********************************************************/
/*�f�[�^����147?*/

// Port Definition for MX614  ******************************/
#include"Rx_protcol.h"
#include"sci.h"
#include"intrinsic.h"
#include "timer.h"
#include "inthandler.h"
/*
#include"rx_pin_as.h"      	//MX_M0�ȂǁA���f���̒[�q��H8�̃|�[�g�Ɋ��蓖�� �]
//#include"rsc_decode.c"   	//RSC�̃f�R�[�_�A�G���R�[�_�A�e�X�g�v���O�����������Ă���B�]			
						 	//���ł�RSC�t����P�S�S�����Ə����Ă��������A�P�S�V����Ȃ��̂��ˁB�@�]
#include"comm_buffer.c"    	//com_UPLINK_BUFFER_LENGTH��147�ɁB�o�b�t�@�̒������w��@�]
#include"timer.h"			//delayms��delayus���`�BH8��ITU�Ƃ������|�[�g���g���Ă���@�]
#include"sci.h"			 	//H8�̃V���A���ʐM�p�̊֐��̒�`�Bdebug_puts�Ƃ��Bsci0.c��sci1.c�Ƃ�����ƈႤ�@�]
#include "Ax_to_info.h"


#define ONE_BIT_CYCLE  350
#define TEST_1BIT 1
#define info_length 204
/***********************************************************/
//
/* Number of cycles which equals to one bit time *********/ 
/* Calculation : 121 / 10 * CLOCK ************************/
/* For example : 10MHz -> 121, 4MHz -> 48 ****************/
int i,bit=0;				//bit�͉��r�b�g�擾�������B�f�o�b�O�p�B

void init_itu(void){	
	ITU0.TCR.BYTE=0x43;	//��/8 GRA �����オ��
	ITU0.GRA=107;		//50us ���z�I�ɂ�99
	ITU0.GRB=695;		//333us
	ITU.TSTR.BIT.STR0=1;	//�^�C�}�X�^�[�g
}
/**
 * bitin ���f������̃f�[�^����(FX_RXD�s����High/Low)���Ď�����֐�
 * �f�[�^��NRZI�`��(�󂯎��bit�̏�Ԃ��ς��Ȃ����1���A�ς���
 * 0���o�͂���B)�ł���B
 */
static int oldstate; //�O�̃r�b�g 
unsigned int kbit; 
int shiftbuff=0;//
int fcs_error=0;

int bitin(void){
	set_imask_ccr(1);// ���荞�݋֎~
	TESTPIN = 0;	/*1bit�T�C�N���̌v�Z�̂��߂̃s���o��*/
	for(kbit = 0 ; kbit < ONE_BIT_CYCLE ; kbit++){ 
		if(FX_RXD != oldstate){			
            oldstate = FX_RXD;
            TESTPIN = 1;
			delay_us_rx(800);
			//delay_us_rx(1800);//430//��800
            return 0; // state changed 
		}	
	}
	TESTPIN = 1;
	//delay_us_rx(345);//��100,82
	delay_us_rx(30);//82
	return 1; // state did not change 
}
/*************
��bitin��20130803�ȑO���Q��
*************/
/************************************************
 *function get_byte()
 *(bitin���܂Ƃ߂āA�@�])��M����1byte��Ԃ�
 ************************************************/

unsigned char get_byte(int mode){				//�v���A���u����M����1�@�l�͂��̕ӂ͂悭�c�����Ă��Ȃ��@�]
  	unsigned char bitbuff;
  	unsigned char bytebuff=0;
  	unsigned char bitbuff_5th1;
	//int shiftbuff=0;
  	for(i=0;i<8;i++){							//��ɂW��s���B�]
    	bitbuff=bitin();						//1bit��M�A0�Ȃ�0�A1�Ȃ�1.�璷����Ȃ��H�@�]
    	if(mode==0){							/*�v���A���u�����[�h�łȂ��Ȃ�r�b�g�V�t�g�̏��� //�����Ă���B�]			�@*/
    											/*�r�b�g�V�t�g�̏����Ƃ����̂́A�T��A���łP�������玟�̃r�b�g���̂Ă鏈���@�]*/
      		if(bitbuff) shiftbuff++;
      		else shiftbuff=0;
			if(shiftbuff==5){					//5��A��1�����o���� //5��A��bitin��1�����o����@�]
				shiftbuff=0;
				bitbuff_5th1=bitin();			//����1bit��0�������ꍇ�͎̂Ă�B1�ł���Ȃ�|�X�g�A���u���̉\��
				if(bitbuff_5th1==1){
					bytebuff=bytebuff>>1;		//��ʃr�b�g�������Ă����B1bit���ɃV�t�g    //��������R�s��bit��byte�̏����B�]
    				bytebuff=bytebuff&0x7f;		//��ԉ��̌���0�Ƀ}�X�N
   					bytebuff+=(bitbuff_5th1)*(0x80);
					bit++;
					i++;
				}
      		}
    	}
    	bytebuff=bytebuff>>1;					//��ʃr�b�g�������Ă����B1bit���ɃV�t�g    //��������R�s��bit��byte�̏����B�]
    	bytebuff=bytebuff&0x7f;					//��ԉ��̌���0�Ƀ}�X�N
    	bytebuff+=(bitbuff)*(0x80);
		bit++;
  	}											//�����܂ł��W��J��Ԃ��@�]
  	return bytebuff;							//�l��Ԃ�
}

/*********************************
*function get_buff()
*��M�f�[�^��z��Ɋi�[����
**********************************/
//�z��Ƃ́Auplink_rx_buff[147]�Bcomm_buffer.c���Œ�`����Ă���B147�łP�p�P�b�g�B�@�]
//�͂��߂Ƀv���A���u���ł̓������s���B�v���A���u����0x7e(01111110)�Bpreambuff���@�]
//����ɍ��v����܂ŁA�P�r�b�g�����炵������B�]
//���ɃX�^�[�g�t���O��T���B��������z��Ƀf�[�^���i�[�B�]

int get_buff(void){										//�����ōŒ�v���A���u���T�O�񕪂̃r�b�g���擾
	unsigned char preambuff;							//�悸1byte��M	//get_byte(�P)�Ȃ�v���A���u���B�܂�r�b�g�V�t�g�����͂��Ȃ��B�@�]
	int j;
	int count_a=0;
	int count_b=0;
	unsigned char starbuf;
	unsigned char preamble_flag=0x0f;
	for(j=0;j<4;j++){
		preambuff= get_byte(1);		
		for(i=0;i<16;i++){									//�v���A���u���͂W�񗬂��@�]
	    	if(preambuff!=0x7e){							//�v���A���u���ɓ����������_�ŏ������~�߂�@�]
				preambuff=preambuff<<1;						//1bit�����炵�ē�������܂Ō���
				preambuff=preambuff&0xfe;					//1bit�ڂ�0�Ń}�X�N�@�]
				preambuff+=bitin();							//����܂ł̂R�s��preambuff��1bit���ɂ����B�@�]
			}
		 
		}
		if(preambuff!=0x7e){								//8bit���炵�ď�肭�s���Ȃ�������G���[��Ԃ�
			return 1;										//return 1 �̓v���A���u���擾�G���[�@�]
		}
	}
	while(preambuff==0x7e) preambuff=get_byte(1);		//�v���A���u�����I���܂Ŏ�M��������
	uplink_rx_buff[0]=preambuff;
	for(j=1;j<com_UPLINK_BUFFER_LENGTH;j++)			//��M�f�[�^��z��Ɋi�[
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
	return 0;											//����I��
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

/*****�t���O�̐��𐔂��Ă���*****/
/*	for(i=0;i<StrLength;i++){
		if(textstring[i]==0x7e){
			count_flags++;
		}
	}		
	i=0;
*/
/*****�t���O�����*****//*
	int start_flag=i;
//////	debug_puts("\r\n preamble...");
//////	debug_puthex(textstring[i]);
	if(textstring[i]!=0x7e){
		debug_puts("\r\n preamble 0x7e is not correct...");
	}
	i++;*/
/*****�A�h���X�����*****/
	int start_address=i;
//////	debug_puts("\r\n address...");
	for(i=start_address;i<start_address+14;i++){
//////		debug_puthex(textstring[i]);
		precrc_bit(textstring[i]);
	}
/*****�R���g���[�������*****/
	int start_control=i;
//////	debug_puts("\r\n control...");
	for(i=start_control;i<start_control+1;i++){
//////		debug_puthex(textstring[i]);
		precrc_bit(textstring[i]);		
	}
/*****PID�����*****/
	int start_PID=i;
//////	debug_puts("\r\n PID...");
	for(i=start_PID;i<start_PID+1;i++){
//////		debug_puthex(textstring[i]);
		precrc_bit(textstring[i]);	
	}
/*****Info�����*****/
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

/*****�����Ă���info�܂ł̕����������FCS�̌v�Z*****/
	fcslo = fcslo^0xff; 
	fcshi = fcshi^0xff;

/*****�����Ă���FCS������BFCS�`�F�b�N�B����Ȃ��Ȃ�-1��Ԃ�*****/

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
/**FCS�������Ă��Ȃ����̑Ώ��@�������Ēu���� **/
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

int get_data(void){								//���̊֐�����̂�������̊֐��̂܂Ƃ߁B
	static int a=0;
  	int errnumb=0;	
  	if(!a){										//0:�G���[�Ȃ��B1:�v���A���u���G���[�B2:�X�^�[�g�t���O�G���[�@�]
  		comm_buffer_init();						//comm_buffer.c���Œ�`����Ă���B�]
  		init_itu();
	}											//�^�C�}�������A�^�C�}�J�n�@�]
	SCI0.SSR.BYTE &= (unsigned char)(~0x80);	// ���M���荞�݋֎~
//	SCI1.SSR.BYTE &= (unsigned char)(~0x80);	// ���M���荞�݋֎~
//	SCI2.SSR.BYTE &= (unsigned char)(~0x80);	// ���M���荞�݋֎~
	ITU.TISRA.BIT.IMIEA2 = 0;		// �^�C�}���荞�݋֎~
	ITU.TISRA.BIT.IMIEA1 = 0;		// �^�C�}���荞�݋֎~
	set_imask_ccr(1);// ���荞�݋֎~

  	errnumb=get_buff();							//�P�p�P�b�g�擾&&�G���[�i���o�[�擾�@�]

	set_imask_ccr(0);// ���荞�݋���
	SCI0.SCR.BYTE = 0x70;		// ����M�A��M���荞�݂̂݋���
//	SCI1.SCR.BYTE = 0x70;		// ����M�A��M���荞�݂̂݋���
//	SCI2.SCR.BYTE = 0x70;		// ����M�A��M���荞�݂̂݋���
	ITU.TISRA.BIT.IMIEA2 = 1;	// �^�C�}���荞�݋���
	ITU.TISRA.BIT.IMIEA1 = 1;   // �^�C�}���荞�݋���
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
		��M�����������ۂ̃o�b�t�@�ւ̏�������
*************************************************/
  if(fcs_error==0){
	sci1_puts_rx(info,length-20);  
	return 0;
  }
  return 1;
}

void rx_init(void){							//�������֐�
	P4DDR = 0xCF; 							//FX_DET�AFX_RXD����͂ɐݒ�
	// init_timer();						//�^�C�}�[���g�p�\�ɂ���@���g��Ȃ��悤�ɂ����@�]
	pll_init_rx(
		0xA00,		//refernce divider
		0x0E,			//swallow_counter
		0x30A,		//programable_counter
		3,			//charge_pump_tx
		3,			//charge_pump_rx
		0,			//lock_detector
		0,			//standby_control_tx
		1			//standby_control_rx
	);		//PLL�ݒ����������
	FX_M0 = 0;								//FSK�̃��[�h
	FX_CLK_RX = 1;							//FSK Operation without Rx Data Retiming
}
