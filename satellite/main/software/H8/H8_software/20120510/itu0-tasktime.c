/*************************************************************************************
* �t�@�C�����@�F�@itu0-tasktime.c
* �^�C�g���@�@�F�@INVADER�^�C�}(CH0)�v���O����
* ����ҁ@�@�@�F�@03-120495 ���R����
* �����@�@�@�@�F�@������w�H�w���d�C�d�q�H�w��
* ������@�@�@�F�@2012.6.4
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
* H8�����^�C�}(ITU-CH0)
* intimia1(): 10msec����(100Hz)�Ŋ��荞�ݏ����𔭐��B
* H8�O���N���b�N�Ƃ��Ă�25MHz��O��B
* 
*************************************************************************************/
#include "itu0-tasktime.h"
#include "iodefine.h"
#include "inthandler.h"


static volatile unsigned long itu0_cnt;    // 10usec�̊��荞�ݖ��ɃC���N�������g
static volatile unsigned long my_wdt; 	          // 10usec�̊��荞�ݖ��ɃC���N�������g mainloop�����I����ƃ��Z�b�g

/*************************************************
            ITU0�������֐�
--------------------------------------------------
* H8�O���N���b�N25MHz
* GRA�Ƃ̃R���y�A�}�b�`�ŃJ�E���g�N���A
*
*************************************************/
unsigned char itu0_init(void)
{

	ITU.TSTR.BIT.STR0 = 0;                // �J�E���g��~
	
	ITU0.TCR.BIT.CCLR = 1;                // GRA�̃R���y�A�}�b�`�ŃJ�E���g�N���A
	ITU0.TCR.BIT.TPSC = 0;                // �^�C�}�v���X�P�[�� 25MHz/1=25MHz T=0.04[usec]
	ITU0.GRA = 50-1;                   // ����2us

	itu0_cnt = 0;
	my_wdt = 0;
	
	ITU.TISRA.BIT.IMFA0 = 0;                // ���荞�݃X�e�[�^�X�t���O�N���A
	ITU.TISRA.BIT.IMIEA0 = 1;              // �^�C�}���荞�݋���
	
	return 1;
}

/*************************************************
            ITU0�X�^�[�g�֐�
*************************************************/
unsigned char itu0_start(void)
{
	
	ITU0.TCNT = 0;                        // �J�E���g�N���A
	ITU.TSTR.BIT.STR0 = 1;                // �J�E���g�J�n
	
	return 1;
}

/*************************************************
            ITU0�X�g�b�v�֐�
*************************************************/
unsigned char itu0_stop(void)
{
	ITU.TSTR.BIT.STR0 = 0;				//�J�E���g�I��
	
	return 1;
}

/*************************************************
            ITU0�������荞�݊֐�
*************************************************/
void INT_IMIA0(void)
{
	itu0_cnt++;
	ITU.TISRA.BIT.IMFA0 = 0;                // ���荞�݃X�e�[�^�X�t���O�N���A
}

/*************************************************
            ITU0���ʕ\���֐�
*************************************************/
unsigned char itu0_display(void)
{
	int time = itu0_cnt*2;
	debug_puts("time = ");
	debug_putdec(time);
	debug_puts("us\n");
	itu0_cnt = 0;					//�J�E���g�N���A
	
	return 1;
}