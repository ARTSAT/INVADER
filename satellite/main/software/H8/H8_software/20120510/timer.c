/* =============================================================================== */
/* 															                       */
/* 				H8 Timer Program for INVADER BBM					               */
/* 															                       */
/* 										Written by Shota Kikuchi			       */
/* 										2012.02.10                                 */
/* 	                                                                               */
/* 										Based on "timer.c" for PRISM			   */
/* 															                       */
/* =============================================================================== */
#ifndef __TIMER_C__
#define __TIMER_C__

#include "iodefine.h"
#include "timer.h"
#include "inthandler.h"
#include "sci.h"
#include "intrinsic.h"

volatile unsigned int time;
volatile unsigned int timer_flag = 0;

/*
void init_timer(void)
{
	ITU0.TCR.BIT.CCLR = 3;
	ITU.TSNC.BIT.SYNC0 = 0;
    ITU0.TCR.BIT.TPSC = 0x03;		//�N���b�N��1/8 (3.125MHz)�ɐݒ�@���@����:0.32usec
	ITU0.TCR.BIT.CCLR = 2;			//�f�t�H���g�ł�GRB��p����
    ITU0.TIOR.BYTE = 0x88;
    ITU.TISRA.BIT.IMIEA0 = 0x00;		//���荞�݋֎~
	ITU.TISRB.BIT.IMIEB0 = 0x00;
    ITU.TSTR.BIT.STR0 = 0;			//ITU0 TCNT�J�E���g�X�g�b�v

}
*/

/* ------------------------------- */
/*   ITU0�ɂ��t[msec]�ҋ@         */
/* ------------------------------- */

/*
void delay_ms(unsigned int t)
{
    unsigned int i = 0;
	
	ITU0.GRA = 2000*t-1;			//t[msec]
    ITU0.TCR.BIT.CCLR = 0x01;		//GRA��p����
	ITU0.TCNT = 0;
//	ITU0.TIER.BIT.IMIEA = 0x01;
    ITU.TSTR.BIT.STR0 = 1;			//ITU0 TCNT�J�E���g�X�^�[�g
    
    while(i < t) {
		//IMFA�t���O���`�F�b�N
        if(ITU0.TSR.BIT.IMFA) {
            ITU0.TSR.BIT.IMFA = 0;		//IMFA�t���O���N���A
            i++;
        }
    }
    ITU.TSTR.BIT.STR0 = 0;			//ITU0 TCNT�J�E���g�X�g�b�v
	ITU0.TIER.BIT.IMIEA = 0x00;
}
*/

void delay_ms(unsigned int t)
{	
//	timer_flag = 0;				//���߂�0�ɂ��Ȃ��ƁA���荞�݊֐�����ɌĂ΂�Ă��܂���while���甲�����Ȃ�
	ITU0.GRA = 3125*t-1;		//t[msec]
	ITU0.TCR.BIT.CCLR = 1;		//GRA��p����
	ITU.TISRA.BIT.IMIEA0 = 1;	//IMFA�t���O�ɂ�銄�荞�݋���
	ITU.TSTR.BIT.STR0 = 1;		//ITU0 TCNT�J�E���g�X�^�[�g
	
	while(!timer_flag);
	
	timer_flag = 0;
	ITU.TSTR.BIT.STR0 = 0;		//ITU0 TCNT�J�E���g�X�g�b�v
	ITU0.TCR.BIT.CCLR = 2;		//�f�t�H���g�ł�GRB��p����
}

/*
void INT_IMIA0(void) {
	ITU.TISRA.BIT.IMFA0 = 0;		//���荞�݃X�e�[�^�X�t���O�N���A
	ITU.TISRA.BIT.IMIEA0 = 0x00;	//IMFA�t���O�ɂ�銄�荞�݋֎~
	timer_flag = 1;					//while�����甲���邽�߂̃t���O�𗧂Ă�
}
*/


/* ------------------------------- */
/*   ITU0�ɂ��t[usec]�ҋ@         */
/* ------------------------------- */


void delay_us(unsigned int t)
{	
	ITU0.GRB = 2*t-1;			//t[usec]
	ITU.TSTR.BIT.STR0 = 1;		//ITU0 TCNT�J�E���g�X�^�[�g
	ITU.TISRB.BIT.IMIEB0 = 1;	//IMFB�t���O�ɂ�銄�荞�݋���
	
	while(!timer_flag);
	 
	timer_flag = 0;
	ITU.TSTR.BIT.STR0 = 0;		//ITU0 TCNT�J�E���g�X�g�b�v
}

void delay_us_rx(unsigned int t)
{	
	timer_flag = 0;
	ITU0.GRB = 2*t-1;			//t[usec]
	ITU.TSTR.BIT.STR0 = 1;		//ITU0 TCNT�J�E���g�X�^�[�g
	ITU.TISRB.BIT.IMIEB0 = 1;	//IMFB�t���O�ɂ�銄�荞�݋���
	set_imask_ccr(0);// ���荞�݋���
	
	while(!timer_flag);
	 
	set_imask_ccr(1);// ���荞�݋֎~
	timer_flag = 0;
	ITU.TSTR.BIT.STR0 = 0;		//ITU0 TCNT�J�E���g�X�g�b�v
}

/*
void INT_IMIB0(void) {
	ITU.TISRB.BIT.IMFB0 = 0;			//���荞�݃X�e�[�^�X�t���O�N���A
	ITU.TISRB.BIT.IMIEB0 = 0x00;		//IMFB�t���O�ɂ�銄�荞�݋֎~
	timer_flag = 1;					//while�����甲���邽�߂̃t���O�𗧂Ă�
}	
*/



#endif
