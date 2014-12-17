/*************************************************************************************
* �t�@�C�����@�F�@adconv.c
* �^�C�g���@�@�F�@INVADER AD�ϊ��v���O����
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.31
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#include "iodefine.h"
#include "inthandler.h"
#include "adconv.h"
#include "sci.h"
#include "global.h"
#include "cdh_pindefine.h"
#include "inthandler.h"
#include "mission.h"


#define	MPX00   (AD.ADDRA & 0xff00) >> 8	// �}���`�v���N�T1-DA
#define	MPX01   (AD.ADDRB & 0xff00) >> 8	// �}���`�v���N�T1-DB
#define	MPX10   (AD.ADDRC & 0xff00) >> 8	// �}���`�v���N�T2-DA
#define	MPX11   (AD.ADDRD & 0xff00) >> 8	// PIC�d���Z���T

/*************************************************
		AD�ϊ��֐��������֐�
*************************************************/
unsigned char adconv_init(void)
{
	unsigned char i;
	
	// AD�ϊ�: AN0~AN3,SCAN���[�h,266�X�e�[�g
	AD.ADCSR.BYTE = 0x13;
	
	// AD�ϊ��^�C�}���荞�݃t���O
	ClearFlags(flag_ad);
	ClearFlags(flag_overa);
	ClearFlags(flag_dispAD);
	
	
	return 1;
}

/*************************************************
		AMI302(���C�Z���T)�X�C�b�`���O�֐�
*************************************************/
void ami_switch(unsigned char ch)
{
	if(ch == 3){
		// x���ϊ�
		AMICH1 = 1;
		AMICH2 = 1;
	}
	else if(ch == 4){
		// y���ϊ�
		AMICH1 = 0;
		AMICH2 = 0; 
	}
	else if(ch == 5){
		// z���ϊ�
		AMICH1 = 1;
		AMICH2 = 0;
	}
	else;
	/*
	debug_puts("AMI:");
	debug_putdec(AMICH1);
	debug_putdec(AMICH2);
	debug_puts("\n");
	*/
}

/*************************************************
		�}���`�v���N�T�E�X�C�b�`���O�֐�
*************************************************/
void mpx_switch(unsigned char ch)
{
	MPX0_A0 = ch & 0x01;
	MPX0_A1 = (ch & 0x02) >> 1;
	MPX0_A2 = (ch & 0x04) >> 2;
	
	MPX1_A0 = ch & 0x01;
	MPX1_A1 = (ch & 0x02) >> 1;
	MPX1_A2 = (ch & 0x04) >> 2;
	
	/*
	debug_puts("CH:");
	debug_putdec(ch);
	debug_puts(" ,PADR:");
	debug_putdec(PADR.BIT.B0);
	debug_putdec(PADR.BIT.B1);
	debug_putdec(PADR.BIT.B2);
	debug_putdec(PADR.BIT.B4);
	debug_putdec(PADR.BIT.B5);
	debug_putdec(PADR.BIT.B6);
	debug_puts("\n");
	*/
}

/*************************************************
		AD�ϊ����荞�݃Z�b�g�֐�
*************************************************/
/*
unsigned char SetADConvert(void)
{
	
	AD.ADCSR.BIT.ADST = 1;		// AD�ϊ��J�n
	AD.ADCSR.BIT.ADIE = 1;		// AD�ϊ����荞�݋���
	
	return 1;
}
*/

/*************************************************
		AD�ϊ��I�����荞�݊֐�
*************************************************/
void get_sensor(void)
{
	static unsigned char mpx_ch;		// �}���`�v���N�TCH:0~7
										// ���C�Z���T(AMI302)CH0~2
	AD.ADCSR.BIT.ADST = 1;				// AD�ϊ��J�n
	while(AD.ADCSR.BIT.ADF == 0);		// AD�ϊ��I���҂�
	
	mpx[0][mpx_ch] = MPX00;
	mpx[1][mpx_ch] = MPX01;
	mpx[2][mpx_ch] = MPX10;
	mpx[3][mpx_ch] = MPX11;
	//MPX11;	// dummy read
	
	//00:A
	//MPX01 
	
	switch(mpx_ch)
	{
		case 0:
			status.temperature[4] =	MPX00;	//-X
			status.temperature[3] =	MPX01;	//+X
			status.gyro[0] =		MPX10;
			break;
		case 1:
			status.temperature[15] =MPX00;	//TX
			status.temperature[5] =	MPX01;	//+Y1
			status.gyro[1] =		MPX10;
			break;
		case 2:
			status.temperature[14] =MPX00;	//Mission
			status.temperature[7] =	MPX01;	//-Y1
			status.gyro[2] =		MPX10;
			break;
		case 3:
			status.temperature[12] =MPX00;	//-Z2
			status.temperature[9] =	MPX01;	//+Z1�i�M�^�󎎌��ɂ����Ă̂�+X�j
			status.magnet[0] =		MPX10;
			break;
		case 4:
			status.temperature[16] =MPX00;	//RX
			status.temperature[17] =MPX01;	//Main
			status.magnet[1] =		MPX10;
			break;
		case 5:
			status.temperature[8] = MPX00;	//-Y2
			status.temperature[11] =MPX01;	//-Z1
			status.magnet[2] =		MPX10;
			break;
		case 6:
			status.temperature[10] = MPX00;	//+Z2
			break;
		case 7:
			status.temperature[6] = MPX00;	//+Y2
			break;
		default:
			break;
		
	}
	
	mpx_ch = mpx_ch + 1;
	if(mpx_ch > 7){
		mpx_ch = 0;
	}else;
	
	mpx_switch(mpx_ch);			// ����̕ϊ��ɔ����A�}���`�v���N�T�̃X�C�b�`���O
								// ADG707:max transition time = 60nsec
	
	ami_switch(mpx_ch);			// ���C�Z���T�̃X�C�b�`���O
								// AMI302:max transition time = 1ms
	
	AD.ADCSR.BIT.ADF = 0;		// �t���O�N���A
	AD.ADCSR.BIT.ADST = 0;		// AD�ϊ���~
}

/*************************************************
		�Q�d�����m�֐�
*************************************************/
void check_overAmpare(void)
{
	AD.ADCSR.BIT.ADST = 1;				// AD�ϊ��J�n
	while(AD.ADCSR.BIT.ADF == 0);		// AD�ϊ��I���҂�
	
	mpx[3][0] = MPX11;
//	status.ampare[11] = MPX11;
	
	// dummy read
	MPX00;
	MPX01;
	MPX10;
	
	debug_puts("<");
//	debug_putdec(status.ampare[0]);
	debug_putdec(mpx[3][0]);
	debug_puts(">");

	
//	if(status.ampare[0] >= 40) {
	if(mpx[3][0] >= 200) {
		num_overAmpare++;
		debug_puts("\n !!!!!!! OverAmpare ");
		debug_putdec(num_overAmpare);
		debug_puts("th !!!!!!! \n");
	}
	else{
		num_overAmpare=0;
	}
	
	if(num_overAmpare>3){
		mission_timeout=0;

		debug_puts("\n !!!!!!!-- Power OFF --!!!!!!!\n");
		num_overAmpare=0;
		
		PIC_KILL1 = 0;
		PIC_KILL2 = 1;
		
		// �^�C���A�E�g�����p�^�C�}(ITU2)�J�E���g�J�n
		ITU2.GRB = 31250-1;			// ����10ms
//		ITU2.TCR.BIT.CCLR = 2;		// GRB�̃R���y�A�}�b�`�ŃJ�E���g�N���A
		ITU.TISRB.BIT.IMFB2 = 0;	// ���荞�݃X�e�[�^�X�t���O�N���A
		ITU.TISRB.BIT.IMIEB2 = 1;	// �^�C�}���荞�݋���
		ITU.TSTR.BIT.STR2 = 1;		// �J�E���g�J�n
		ITU.TISRA.BIT.IMIEA2 = 0;	// �^�C�}���荞�݋֎~
	} 
	
	
	AD.ADCSR.BIT.ADF = 0;		// �t���O�N���A
	AD.ADCSR.BIT.ADST = 0;		// AD�ϊ���~
}
/*************************************************
		�d���nON/OFF
*************************************************/
void PowerReset(void){
	mission_timeout=0;
	debug_puts("\n !!!!!!!-- Power OFF --!!!!!!!\n");
		
	PIC_KILL1 = 0;
	PIC_KILL2 = 1;
		
	// �^�C���A�E�g�����p�^�C�}(ITU2)�J�E���g�J�n
	ITU2.GRB = 31250-1;			// ����10ms
//	ITU2.TCR.BIT.CCLR = 2;		// GRB�̃R���y�A�}�b�`�ŃJ�E���g�N���A
	ITU.TISRB.BIT.IMFB2 = 0;	// ���荞�݃X�e�[�^�X�t���O�N���A
	ITU.TISRB.BIT.IMIEB2 = 1;	// �^�C�}���荞�݋���
	ITU.TSTR.BIT.STR2 = 1;		// �J�E���g�J�n 
	ITU.TISRA.BIT.IMIEA2 = 0;	// �^�C�}���荞�݋֎~
}
/*************************************************
		�d���nON
*************************************************/
void RequestPowerReboot(void){
	debug_puts("\n !!!!!!!-- Power On (5 minutes')--!!!!!!!\n");
		
	PIC_KILL1 = 1;
	PIC_KILL2 = 0;

}
/*************************************************
		�d���nOFF
*************************************************/
void RequestPowerShutdown(void){
	debug_puts("\n !!!!!!!-- Power Off --!!!!!!!\n");
		
	PIC_KILL1 = 0;
	PIC_KILL2 = 1;

}

#define TIMEOUT	1000	// Arduino�����V���b�g�_�E���F1000*10msec = 10sec
/*************************************************
		PIC�ċN��
*************************************************/
void INT_IMIB2(void)
{
	static volatile unsigned long cnt=0;
	
	ITU.TISRB.BIT.IMFB2 = 0;		// ���荞�݃X�e�[�^�X�t���O�N���A
	cnt++;
	
	if(cnt == TIMEOUT){
		debug_puts("\n !!!!!!!-- Power ON --!!!!!!!\n");
		PIC_KILL1 = 1;
		PIC_KILL2 = 0;	
		
		cnt = 0;
//		ITU2.GRB = 0;			// ����10ms
		ITU.TISRA.BIT.IMIEA2 = 0;	// �^�C�}���荞�݋֎~
		ITU.TISRB.BIT.IMIEB2 = 0;	// �^�C�}���荞�݋֎~
		ITU.TSTR.BIT.STR2 = 0;		// �J�E���g��~
		ITU2.TCNT = 0;				// �J�E���^�N���A
//		ITU2.TCR.BIT.CCLR = 1;		// GRA�ɖ߂�
	}
}



/*************************************************
		�X�y�[�X�}���֐�
*************************************************/
void insertSpace(unsigned char dat)
{
	if(dat >= 100)	debug_puts(" ");
	else if(dat >= 10)	debug_puts("  ");
	else debug_puts("   ");
	
	return;
}

/*************************************************
		AD�ϊ����ʕ\���֐�
*************************************************/
unsigned char disp_AD(void)
{
	unsigned char i;
	
	debug_puts("\n");
	
	for(i=0;i<8;i++)
	{
		insertSpace(mpx[0][i]);
		debug_putdec(mpx[0][i]);
		//debug_puts(" ");
	}
	
	debug_puts("\t");
	
	for(i=0;i<8;i++)
	{
		insertSpace(mpx[1][i]);
		debug_putdec(mpx[1][i]);
		//debug_puts(" ");
	}
	
	debug_puts("\t");
	for(i=0;i<8;i++)
	{
		insertSpace(mpx[2][i]);
		debug_putdec(mpx[2][i]);
		//debug_puts(" ");
	}
	
	debug_puts("\t");
	for(i=0;i<6;i++)
	{
		insertSpace(mpx[3][i]);
		debug_putdec(mpx[3][i]);
		//debug_puts(" ");
	}
	debug_puts("\n");
	
	return 1;
}