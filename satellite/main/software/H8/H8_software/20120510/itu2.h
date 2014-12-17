/*************************************************************************************
* �t�@�C�����@�F�@itu2.h
* �^�C�g���@�@�F�@INVADER�^�C�}(CH2)�w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-120495 ���R����
* �����@�@�@�@�F�@������w�H�w���d�C�d�q�H�w��
* ������@�@�@�F�@2012.6.4
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* H8�����^�C�}(ITU-CH2)
* intimia0(): 10msec����(100Hz)�Ŋ��荞�ݏ����𔭐��B
* H8�O���N���b�N�Ƃ��Ă�25MHz��O��B
* 
*************************************************************************************/

#ifndef ITU0_H_
#define ITU0_H_

static volatile unsigned long itu0_cnt;           // 10msec�̊��荞�ݖ��ɃC���N�������g
static volatile unsigned long my_wdt; 	          // 10msec�̊��荞�ݖ��ɃC���N�������g mainloop�����I����ƃ��Z�b�g

unsigned char itu0_init(void);
unsigned char itu0_start(void);
unsigned char itu0_stop(void);
unsigned char itu0_display(void);

#endif
