/*************************************************************************************
* �t�@�C�����@�F�@message.h
* �^�C�g���@�@�F�@INVADER�R�}���h�F���E�ԓ��w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.2.3
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/


#ifndef MESSAGE_H_
#define MESSAGE_H_

volatile unsigned char flag_scirx[3];
volatile unsigned char flag_exe[3];

unsigned char InitMessage(void);
unsigned char FindCommand(void);
unsigned char TransmitMessage(void);
unsigned char InitPLL(void);
unsigned char SendMessage(void);

#endif