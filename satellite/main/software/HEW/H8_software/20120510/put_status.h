/*************************************************************************************
* �t�@�C�����@�F�@put_status.h
* �^�C�g���@�@�F�@INVADER�X�e�[�^�X���M�p�w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.3.15
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#ifndef PUTSTATUS_H_
#define PUTSTATUS_H_

unsigned char SendStatusToGS(unsigned char reply_num);
unsigned char SendTelemetryToGS(unsigned char reply_num, volatile unsigned char *param, unsigned char param_length);

#endif