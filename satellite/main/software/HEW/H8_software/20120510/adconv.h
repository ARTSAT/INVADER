/******************************************************************************
* �t�@�C�����@�F�@adconv.h
* �^�C�g���@�@�F�@INVADER AD�ϊ��w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-103317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.31
* ����@�@�@�@�F�@C
*******************************************************************************
* �����l��
*
******************************************************************************/
#ifndef ADCONV_H_
#define ADCONV_H_

volatile unsigned char flag_ad[3];		// AD�ϊ��^�C�}���荞�݃t���O
volatile unsigned char flag_overa[3];	// �Q�d�����m�^�C�}���荞�݃t���O
volatile unsigned char flag_dispAD[3];	// AD�ϊ����ʕ\�����荞�݃t���O
volatile unsigned char ena_ad[3];
volatile unsigned char ena_overa[3];
volatile unsigned char ena_dispAD[3];
volatile unsigned char flag_pic[3];


unsigned char adconv_init(void);
//unsigned char SetADConvert(void);
void get_sensor(void);
unsigned char disp_AD(void);

void PowerReset(void);
void RequestPowerReboot(void);
void RequestPowerShutdown(void);


#endif