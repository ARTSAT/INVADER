/*************************************************************************************
* �t�@�C�����@�F�@pwr.c
* �^�C�g���@�@�F�@INVADER�p���[�e�����g�����M���N�G�X�g/��M�v���O����
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.3.19
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#include "iodefine.h"
#include "pwr.h"
#include "global.h"
#include "sci.h"
#include "cdh_pindefine.h"
#include "teleme.h"

#define SIZE_VOLTAGE_PIC 3
#define SIZE_AMPARE1_PIC 12
#define SIZE_AMPARE2_PIC 7
#define SIZE_TEMPERATURE1_PIC 1
#define SIZE_TEMPERATURE2_PIC 3
#define SIZE_FLAG_PIC 2

/*************************************************
        �p���[�e�����g���������֐�
*************************************************/
unsigned char InitPowerTelemetry(void)
{
	ClearFlags(flag_pwrt);
	ClearFlags(flag_alive);
	
	return 1;
}

/*************************************************
		�p���[�e�����g���E���N�G�X�g���M�֐�
*************************************************/
unsigned char RequestPowerTelemetry(void)
{
	sci0_puts("c-p-c-pwt \r\n");
	//debug_puts("c-p-c-pwt \r\n");		// for debug
	
	return 1;
}

/*************************************************
		�p���[�e�����g����M/�X�V�֐�
*************************************************/
unsigned char UpdatePowerTelemetry(volatile unsigned char *pwrt)
{
	unsigned char i,j=0;
	
	/*
	status.temperature[0] = pwrt[j++];
	for(i=0;i<SIZE_VOLTAGE_PIC;i++)	status.voltage[i] = pwrt[j++]; 
	for(i=0;i<SIZE_AMPARE_PIC;i++)	status.ampare[i] = pwrt[j++];
	for(i=0;i<SIZE_TEMPERATURE_PIC;i++) status.temperature[i] = pwrt[j++];
	for(i=0;i<SIZE_FLAG_PIC;i++) status.chargeflag[i] = pwrt[j++];
	*/
	
	for(i=0;i<SIZE_AMPARE1_PIC;i++) 		status.ampare[i] = pwrt[j++];
	for(i=0;i<SIZE_FLAG_PIC;i++) 			status.chargeflag[i] = pwrt[j++];
	for(i=0;i<SIZE_VOLTAGE_PIC;i++)			status.voltage[i] = pwrt[j++];
	status.temperature[13] = pwrt[j++];
	for(i=SIZE_AMPARE1_PIC;i<SIZE_AMPARE1_PIC+SIZE_AMPARE2_PIC;i++) status.ampare[i] = pwrt[j++];
	for(i=0;i<SIZE_TEMPERATURE2_PIC;i++) status.temperature[i] = pwrt[j++];
	
	// for debug
	//debug_puts("get_pwrt\n");
	/*
	for(i=0;i<28;i++){
		debug_puts("pwr[");
		debug_putdec(i);
		debug_puts("]: ");
		debug_putdec(pwrt[i]);
		debug_puts("\n");
	}
	*/
	// end of debug
	
	
	return 1;
}

/*************************************************
		���Z�b�g ���N�G�X�g���M�֐�
*************************************************/
unsigned char RequestMainShutdown(void)
{
	status.error_counter[2] += 1;
	SetFlags(flag_iteleme);
	sci0_puts("c-p-c-rst \r\n");
	debug_puts("c-p-c-rst \r\n");	// for debug
	
	return 1;
}

/*************************************************
		�q�[�^�[�e�X�g�֐��iHDR���M�j
*************************************************/
unsigned char SendHDRtoPIC(void)
{
	sci0_puts("c-p-c-hdr \r\n");
	return 1;
}

/*************************************************
		�q�[�^�[�e�X�g�֐��iHDS���M�j
*************************************************/
unsigned char SendHDStoPIC(void)
{
	sci0_puts("c-p-c-hds \r\n");
	return 1;
}

/*************************************************
		�d���f�o�b�O�p
*************************************************/
unsigned char DebugPowerMessage(volatile unsigned char* param, unsigned char param_length){
	int i;
	debug_puts("\n --------- message from power --------- \n");
	for(i=0;i<param_length;i++){
		debug_putc(param[i]);
		debug_puts("\t");
		debug_puthex(param[i]);
		debug_puts("\n");
	}
	debug_puts(" ---------message from power end------ \n");
}

		