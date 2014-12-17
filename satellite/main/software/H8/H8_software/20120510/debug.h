/*************************************************************************************
* �t�@�C�����@�F�@debug.h
* �^�C�g���@�@�F�@INVADER�f�o�b�O�R�}���h�w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.3.19
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/

#ifndef DEBUG_H_
#define DEBUG_H_

unsigned char DebugPut_Status(void);
unsigned char DebugMultiplexer(void);
unsigned char DebugEEPROM(volatile unsigned char* param,unsigned char param_length);
unsigned char DebugPut_Rough(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_Detail(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_API(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_Initial();
unsigned char Debug_SharedROM(unsigned long int page, unsigned char *dat, unsigned char s_ad, unsigned char byte);
unsigned char DebugWrite_SharedROM(volatile unsigned char* param, unsigned char param_length);
unsigned char DebugPut_SharedROM(volatile unsigned char* param, unsigned char param_length);
unsigned char DisplayRoutine(void);
unsigned char SetRoutine(volatile unsigned char* param);
unsigned char StopAlivePeriod(void);

#endif