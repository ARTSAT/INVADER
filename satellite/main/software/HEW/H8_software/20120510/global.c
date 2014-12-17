/******************************************************************************
* �t�@�C�����@�F�@global.c
* �^�C�g���@�@�F�@INVADER�p�O���[�o���ϐ��֘A�v���O����
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.30
* ����@�@�@�@�F�@C
*******************************************************************************
* �����l��
*
******************************************************************************/
#include "iodefine.h"
#include "global.h"

#include "message.h"
#include "pwr.h"
#include "mission.h"
#include "adconv.h"
#include "teleme.h"
#include "itu1.h"

/*************************************************
		�X�e�[�^�X��񏉊����֐�
*************************************************/
unsigned char InitStatus(void)
{
	unsigned char i;
	
	for(i=0;i<SIZE_OBCTIME;i++)		status.obctime[i] = 0x00;
  
	for(i=0;i<SIZE_VOLTAGE;i++)		status.voltage[i] = 0x00;
	
	for(i=0;i<SIZE_AMPARE;i++)		status.ampare[i] = 0x00;
	
	for(i=0;i<SIZE_TEMPERATURE;i++)	status.temperature[i] = 0x00;
	
	for(i=0;i<SIZE_GYRO;i++)		status.gyro[i] = 0x00;
	
	for(i=0;i<SIZE_MAGNET;i++)		status.magnet[i] = 0x00;
	
	for(i=0;i<SIZE_RSSI;i++)		status.rssi[i] = 0x00;
  
	for(i=0;i<SIZE_MISSION;i++)		status.mission[i] = 0x00;
	
	for(i=0;i<SIZE_POWER;i++)		status.power[i] = 0x00;
	
	for(i=0;i<SIZE_INIT;i++)		status.init[i] = 0x00;
	
	for(i=0;i<SIZE_MODE;i++)		status.mode[i] = 0x00;
	
	for(i=0;i<SIZE_CHARGEFLAG;i++)		status.chargeflag[i] = 0x00;
	
	for(i=0;i<SIZE_FLAG;i++)		status.flag[i] = 0x00;
	
	for(i=0;i<SIZE_ERROR;i++)		status.error_counter[i] = 0x00;
	
	for(i=0;i<SIZE_MESSAGE;i++)		status.message_counter[i] = 0x00;
	
	return 1;
}

/*************************************************
		���[�`���������֐�
*************************************************/
unsigned char InitRoutine(void)
{
	/*
	1.Power Telemetry Request	: EN
	2.Mission Telemetry Send	: EN
	3.AD Convert				: EN
	4.Rough Telemetry			: EN
	5.Detail telemetry			: DIS
	6.API Telemetry				: EN
	7.My WDT					: DIS
	8.Over Ampare				: EN
	9.Initial Telemetry			: EN
	10.Alive					: EN
	11.DispAD					: DIS
	12.Output Telemetry			: EN
	*/

	SetFlags(ena_pwrt);
	SetFlags(ena_misst);
	SetFlags(ena_ad);
	SetFlags(ena_rteleme);
	ClearFlags(ena_dteleme);
	SetFlags(ena_ateleme);
	ClearFlags(ena_mywdt);
	SetFlags(ena_overa);
	SetFlags(ena_iteleme);
	SetFlags(ena_alive);
	ClearFlags(ena_dispAD);
	SetFlags(ena_oteleme);
	
	return 1;
}

/*************************************************
		�t���O�������֐�
*************************************************/
unsigned char CheckFlags(volatile unsigned char *f)
{
	unsigned char flag = 0;
	unsigned char flag_sum = 0;
	unsigned char i;
	
	for(i=0; i<3; i++){
		f[i] &= 0x01;		// �}�X�N
		flag_sum += f[i];
	}
	
	if(flag_sum >= 2)	flag = 1;
	
	return flag;
}

/**************************************************
		�t���O�N���A�֐�
**************************************************/
void ClearFlags(volatile unsigned char *f)
{
	unsigned char i;
	
	for(i=0;i<3;i++){
		f[i] = 0x00;	
	}
}

/**************************************************
		�t���O�Z�b�g�֐�
**************************************************/
void SetFlags(volatile unsigned char *f)
{
	unsigned char i;
	
	for(i=0;i<3;i++){
		f[i] = 0x01;
	}
}
