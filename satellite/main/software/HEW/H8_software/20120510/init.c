/*************************************************************************************
* �t�@�C�����@�F�@init.c
* �^�C�g���@�@�F�@INVADER�������v���O����
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.5.10
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#include "iodefine.h"
#include "cdh_pindefine.h"
#include "sci.h"
#include "itu1.h"
#include "adconv.h"
#include "teleme.h"
#include "pwr.h"
#include "mission.h"
#include "message.h"
#include "iic.h"
#include "global.h"
#include "fm_tx.h"
#include "fm_pindef.h"

/*************************************************
			�����֐��������֐�
--------------------------------------------------
* H8�̓����֐��̏��������s��
*
*************************************************/
unsigned char Init_MyFunction(void)
{
	unsigned char flag = 0;		// �����֐��������t���O
	
	flag += sci0_init(19200,8,0,1);		// �V���A���|�[�g(CH0)1������ br=19200,bit=8,parity=none,stopbit=1
	flag += sci1_init(19200,8,0,1);		// �V���A���|�[�g(CH1)1������ br=19200,bit=8,parity=none,stopbit=1
	flag += sci2_init(4800,8,0,1);
	
	flag += InitMessage();
		
	flag += itu0_init();
	flag += itu1_init();
	flag += itu2_init();
	
	flag += adconv_init();
	
	flag += InitStatus();
	flag += InitTelemetry();
	flag += InitPowerTelemetry();
	flag += InitMissionTelemetry();
	
	flag += InitRoutine();	// �e���[�`���̋���/�s����������
	
	flag += init_fm();
	
	return flag;
}

/*************************************************
			�������W�X�^�������֐�
--------------------------------------------------
* H8��CPU�o�X�@�\�y�уs���̐ݒ���s��
* 
*************************************************/
unsigned char Init_Bus(void)
{
	// ���o�͐ݒ�
	APLS_OUT;
	MPX_OUT;
	AMI_OUT;
	SROM_OUT;
	
	// �f�[�^���W�X�^������
	APLS = 0;
	
	MPX0_A0 = 0;
	MPX0_A1 = 0;
	MPX0_A2 = 0;
	MPX1_A0 = 0;
	MPX1_A1 = 0;
	MPX1_A2	= 0;
	
	SROM_CH = 0;
	SROM_EN = 0;
	
	AMICH1 = 0;
	AMICH2 = 0;
	
	FX_RDYN	= 0;
	FX_CLK	= 0;
	FX_TXD	= 0;
	FX_M1	= 0;

	FMSTB	= 0;
	FMDAT	= 0;
	FMCLK	= 0;
	FMPTT	= 0;
	
	FMCWSW 	= 0;//1
//	FMDTSW 	= 1;
	FMDTSW 	= 0;
	
	PIC_KILL1 = 1;
	PIC_KILL2 = 0;
	
	return 1;					// �������W�X�^�������t���O ~0x01
}

/*************************************************
			�f�o�C�X�������֐�
--------------------------------------------------
* H8�ɐڑ�����Ă���e��f�q�̐ݒ���s��
*
*************************************************/
unsigned char Init_Peripherals(void)
{
	unsigned char flag = 0;		// �f�o�C�X�������t���O ~0x01
	
	flag += IIC0_EEPINIT();			// EEPROM������
	flag += IIC1_EEPINIT();
	flag += IIC2_EEPINIT();
	
	return flag;
}
