/*************************************************************************************
* �t�@�C�����@�F�@command.h
* �^�C�g���@�@�F�@�O���[�o���ϐ��������w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.2.3
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* 
*************************************************************************************/

#ifndef COMMAND_H_
#define COMMAND_H_


unsigned char RegistCommand	(unsigned char CommandOrReply,
							 unsigned char *cmd,
                             unsigned char receiver_ID,
                             unsigned char sender_ID,
                             volatile unsigned char *param,
                             unsigned char param_length,
                             unsigned char reply_num);
unsigned char ExecuteCommand(void);

#endif
