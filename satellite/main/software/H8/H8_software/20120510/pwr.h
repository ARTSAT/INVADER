/*************************************************************************************
* �t�@�C�����@�F�@pwr.h
* �^�C�g���@�@�F�@INVADER�p���[�e�����g�����M���N�G�X�g/��M�v���O����
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.3.19
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#ifndef PWRT_H_
#define PWRT_H_

volatile unsigned char flag_pwrt[3];
volatile unsigned char ena_pwrt[3];

volatile unsigned char flag_alive[3];
volatile unsigned char ena_alive[3];

unsigned char InitPowerTelemetry(void);
unsigned char RequestPowerTelemetry(void);
unsigned char RequestMainShutdown(void);
unsigned char UpdatePowerTelemetry(volatile unsigned char *pwrt);

#endif