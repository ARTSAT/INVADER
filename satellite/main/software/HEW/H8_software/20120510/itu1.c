/*************************************************************************************
* �t�@�C�����@�F�@itu0.c
* �^�C�g���@�@�F�@INVADER�^�C�}(CH0)�v���O����
* ����ҁ@�@�@�F�@03-110351 �O�D�����@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.5.18
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* H8�����^�C�}(ITU-CH1)
* intimia1(): 10msec����(100Hz)�Ŋ��荞�ݏ����𔭐��B
* H8�O���N���b�N�Ƃ��Ă�25MHz��O��B
* 
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "itu1.h"
#include "cdh_pindefine.h"
#include "sci.h"
#include "teleme.h"
#include "adconv.h"
#include "message.h"
#include "global.h"
#include "pwr.h"
#include "mission.h"
#include "debug.h"
#include "Rx_protcol.h"
#include "fm_tx.h"

//#define DIV_MPX   		  	5                   // 20msec���ɑS�Ẵ}���`�v���N�T�Ń`�����l���؂�ւ�
#define DIV_MPX   		  	10                   // 100msec���ɑS�Ẵ}���`�v���N�T�Ń`�����l���؂�ւ�
//#define DIV_AD				50					// AD�ϊ���500msec�����ōs��
#define DIV_AD				100					// AD�ϊ���1000msec�����ōs��
#define DIV_OVERAMPARE		100					// 1.0sec���ɉQ�d�����m
#define DIV_PTELEME     	100					// 1sec�����Ńp���[CPU�ɓd�����𑗐M�v��
#define DIV_MTELEME			100					// 1sec�����Ń~�b�V����CPU�Ƀe�����g�����M
#define DIV_ROUGHTELEME     36000	//36000				// 6��������1�Z�b�g�̑e�e�����g���ۑ�
#define DIV_DETAILEDTELEME  200	//200      		// 2sec������1�Z�b�g�̏ڍ׃e�����g���ۑ�
#define DIV_APITELEME		60000	//60000				// 10��������API�z�M�̃e�����g���ۑ�
#define DIV_ITELEME			1000				// 10sec�����ŏ��������̃e�����g���ۑ�
#define DIV_MESSAGE 		10					// 0.1sec�����ŃR�}���h�m�F
#define DIV_EXE		 		10					// 0.1sec�����ŃR�}���h���s
#define DIV_RX				10					// 0.1sec������RX����̃R�}���h�m�F
#define DIV_TX				100					// 1.0sec������TX�Ƃ������ϐ��Ɋւ���m�F
#define DIV_CPR				50					// 0.5sec�����Ńp���[����Ԏ����Ԃ��ė��Ă��邩�̊m�F
#define DIV_OUTPUTTELEME	30000				// 5�������Ńe�����g�����o��
#define	DIV_PIC				30000				// 5��������PIC��ON����B

#define ALV_PERIOD	200					// �����M�������i�V�^�j�i2sec�j
#define OVERFLOW	1000				// my_wdt�����̒l�ȏ�ɂȂ����玩�ȃ��Z�b�g�v��������

//static volatile unsigned long itu1_cnt;         // 10msec�̊��荞�ݖ��ɃC���N�������g

/*************************************************
            ITU1�������֐�
--------------------------------------------------
* H8�O���N���b�N25MHz
* GRA�Ƃ̃R���y�A�}�b�`�ŃJ�E���g�N���A
*
*************************************************/
unsigned char itu1_init(void)
{
	ITU.TSTR.BIT.STR1 = 0;                // �J�E���g��~
	ITU1.TCNT = 0;                        // �J�E���g�N���A
	
	ITU1.TCR.BIT.CCLR = 1;                // GRA�̃R���y�A�}�b�`�ŃJ�E���g�N���A
	ITU1.TCR.BIT.TPSC = 3;                // �^�C�}�v���X�P�[�� 25MHz/8=3.125MHz T=0.32[usec]
	ITU1.GRA = 31250-1;                   // ����10ms
	
	itu1_cnt = 1;
	my_wdt = 0;
	
	ITU.TISRA.BIT.IMFA1 = 0;                // ���荞�݃X�e�[�^�X�t���O�N���A
	ITU.TISRA.BIT.IMIEA1 = 1;               // �^�C�}���荞�݋���
	ITU.TSTR.BIT.STR1 = 1;                  // �J�E���g�J�n
	
	return 1;
}

/*************************************************
            ITU0�������荞�݊֐�
--------------------------------------------------
* H8�O���N���b�N25MHz
* 100Hz�Ŋ��荞�ݔ���
*
**************************************************/
void INT_IMIA1(void)
{
	unsigned char i;
	
	//�����M��
	if(itu1_cnt % ALV_PERIOD == 0)	SetFlags(flag_alive);
	
	// AD�ϊ�,�}���`�v���N�T�X�C�b�`���O
//	if((itu1_cnt % DIV_AD) % DIV_MPX == 0 && (itu1_cnt % DIV_AD) <=35){	// DIV_AD/10 * 7

	if((itu1_cnt % DIV_AD) % DIV_MPX == 0 && (itu1_cnt % DIV_AD) <=70){	// DIV_AD/10 * 7
		SetFlags(flag_ad);
	}
	
	// AD�ϊ����ʕ\��
	if(itu1_cnt % DIV_AD == 0)	SetFlags(flag_dispAD);
	
	//�R�}���h���s
	if(itu1_cnt % DIV_EXE == 0)	SetFlags(flag_exe);
	
	//�Q�d�����m�e�����g��
	if(itu1_cnt % DIV_OVERAMPARE == 0)	SetFlags(flag_overa);
	
	// �e�e�����g��
	if(itu1_cnt % DIV_ROUGHTELEME == 0)	SetFlags(flag_rteleme);
	
	// �ڍ׃e�����g��
	if(itu1_cnt % DIV_DETAILEDTELEME == 0)	SetFlags(flag_dteleme);
	
	// API�e�����g��
	if(itu1_cnt % DIV_APITELEME == 0)	SetFlags(flag_ateleme);
	
	// ���������e�����g��
	if(itu1_cnt % DIV_ITELEME == 0)	SetFlags(flag_iteleme);
	
	// �p���[�e�����g��
	if(itu1_cnt % DIV_PTELEME == 0)	SetFlags(flag_pwrt);
	
	// �~�b�V�����e�����g��
	if(itu1_cnt % DIV_MTELEME == 0) SetFlags(flag_misst);
	
	// �V���A���ʐM���b�Z�[�W�`�F�b�N
	if(itu1_cnt % DIV_MESSAGE == 0)	SetFlags(flag_scirx);
	
	// RX�`�F�b�N
	if(itu1_cnt % DIV_RX == 0)	SetFlags(flag_rx);
	
	// TX�`�F�b�N
	if(itu1_cnt % DIV_TX == 0)	SetFlags(flag_tx);
	
	//�p���[����Ԏ����Ԃ��ė��Ă��邩�̊m�F
	if(itu1_cnt % DIV_CPR == 0)  SetFlags(flag_cpr);
	
	// �����p�e�����g���o��
	if(itu1_cnt % DIV_OUTPUTTELEME == 0) SetFlags(flag_oteleme);
	
	//�~�b�V����CPU�N��(�^�C�}�\��^�A�^�C�}�\��&�쓮���Ԏw��^)
	if((smr_time>0)&&(itu1_cnt > smr_time)){
		SetFlags(flag_smr);
	}
	//�~�b�V����CPU�V���b�g�_�E��(�^�C�}�\��&�쓮���Ԏw��^)
	if((sms_time>0)&&(itu1_cnt > sms_time)){
		SetFlags(flag_sms);
	}
	//PIC�I���v��
	if(itu1_cnt % DIV_PIC == 0) SetFlags(flag_pic);
	
	/*�f�o�b�O�p*/
	mission_cnt=itu1_cnt;
	
	itu1_cnt++;
	
	if(itu1_cnt >= 0xFFFFFFFE)	// 0.001*4294967296(sec) = 500(day)�ŃN���A
	{
		itu1_cnt = 0;
	}
	
	if(CheckFlags(ena_mywdt)){
		my_wdt++;		// main loop�ŃN���A
		if(my_wdt >= OVERFLOW)	RequestMainShutdown();	// ���ȃ��Z�b�g�v��
	}
	
	status.obctime[3] =  itu1_cnt			& 0x00ff;
	status.obctime[2] = (itu1_cnt >>  8)	& 0x00ff;
	status.obctime[1] = (itu1_cnt >> 16)	& 0x00ff;
	status.obctime[0] = (itu1_cnt >> 24)	& 0x00ff;
	
	ITU.TISRA.BIT.IMFA1 = 0;				// ���荞�݃X�e�[�^�X�t���O�N���A
}
