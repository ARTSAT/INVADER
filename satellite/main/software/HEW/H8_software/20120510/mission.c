/*************************************************************************************
* �t�@�C�����@�F�@mission.c
* �^�C�g���@�@�F�@INVADER�~�b�V�����e�����g�����M�v���O����
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.3.29
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#include "mission.h"
#include "global.h"
#include "sci.h"
#include "cdh_pindefine.h"
#include "inthandler.h"
#include "fm_pindef.h"
#include "fm_tx.h"
#include "iic.h"
#include "teleme.h"

#define MAIN 0		//���L�������̐؂�ւ�
#define MISSION 1
/*************************************************
        �~�b�V�����e�����g���������֐�
*************************************************/
unsigned char InitMissionTelemetry(void)
{
	ClearFlags(flag_misst);
	
	return 1;
}

/*************************************************
        �~�b�V����CPU�e�����g�����M
*************************************************/
unsigned char SendMissionTelemetry(void)
{
	unsigned char i;
	
	sci2_puts("c-m-c-smm-");
	for(i=0; i<SIZE_OBCTIME; i++)		sci2_putc(status.obctime[i]);
	for(i=0; i<SIZE_VOLTAGE; i++)		sci2_putc(status.voltage[i]);
	for(i=0; i<SIZE_AMPARE; i++)		sci2_putc(status.ampare[i]);
	for(i=0; i<SIZE_TEMPERATURE; i++)	sci2_putc(status.temperature[i]);
	for(i=0; i<SIZE_GYRO; i++)			sci2_putc(status.gyro[i]);
	for(i=0; i<SIZE_MAGNET; i++)		sci2_putc(status.magnet[i]);
	sci2_puts(" \r\n");
/*	
	debug_puts("c-m-c-smm-");
	for(i=0; i<SIZE_OBCTIME; i++)		debug_putc(status.obctime[i]);
	for(i=0; i<SIZE_VOLTAGE; i++)		debug_putc(status.voltage[i]);
	for(i=0; i<SIZE_AMPARE; i++)		debug_putc(status.ampare[i]);
	for(i=0; i<SIZE_TEMPERATURE; i++)	debug_putc(status.temperature[i]);
	for(i=0; i<SIZE_GYRO; i++)			debug_putc(status.gyro[i]);
	for(i=0; i<SIZE_MAGNET; i++)		debug_putc(status.magnet[i]);
	debug_puts(" \r\n");
*/
	
	return 1;
}

/*************************************************
        �~�b�V����CPU�N��
*************************************************/
unsigned char RequestMissionReboot(void)
{
	Ax_Send("r-g-c-msr \r\n",12);
	WriteIicBootTime(mission_cnt);		//�N�����Ԃ����L�������ɋL�^
	debug_puts("!!!!!!-- Request Mission Reboot --!!!!!!.\n");
//	delay_us(1000);
	sci0_puts("c-p-c-msr \r\n");		// �~�b�V����CPU�N����PIC�ɗv��

	SROM_EN = 1;				//1017�ŕύX�_
	SROM_CH = MISSION;
	
	ITU.TISRA.BIT.IMIEA2 = 0;		// �^�C�}���荞�݋֎~
	ITU.TSTR.BIT.STR2 = 0;			// �J�E���g��~
	ITU2.TCNT = 0;					// �J�E���^�N���A	
	
//	debug_puts("r-g-c-msr \r\n");
	
	return 1;
}

/*************************************************
        �~�b�V����CPU�N��(�^�C�}�\��^)
*************************************************/
unsigned char ScheduledRequestMissionReboot(volatile unsigned char* param, unsigned char param_length){
	int i;
	smr_time=0;
	for(i=0;i<param_length;i++){
		smr_time=smr_time*10+(unsigned long int)(param[i]-48);
	}
	
	
	/* smr_time���傫�����Ȃ������m���߂� */
	/*
	if((smr_time>=4294967296)||(param_length>=10)){
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > the LIMIT. \r\n",50);

		debug_puts("\n INVALID message header with SCI1: smr_time beyonds the limit (4294967296) ");
		debug_puts("\n request is: ");
		debug_putdec(smr_time);
		debug_puts("\n");
		return 1;
	}
	*/
	/* smr_time�����݂̎��Ԃ𒴂��Ă��Ȃ������m���߂� */
	/*
	else if(smr_time<mission_cnt){
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > mission_cnt. \r\n",52);
		
		debug_puts("\n INVALID message header with SCI1: smr_time > mission_cnt ");
		debug_puts("\n request is: ");
		debug_putdec(smr_time);
		debug_puts("\n obc_time is: ");
		debug_putdec(mission_cnt);
		debug_puts("\n");
		return 1;
	}
	*/
	debug_puts("\n param_length: ");
	debug_putdec(param_length);
	debug_puts("\n request is: ");
	debug_putdec(smr_time);
	debug_puts("\n obc_time is: ");
	debug_putdec(mission_cnt);
	debug_puts("\n");
	
	unsigned char message_smr[21];
	message_smr[0]='r';
	message_smr[1]='-';
	message_smr[2]='g';
	message_smr[3]='-';
	message_smr[4]='c';
	message_smr[5]='-';
	message_smr[6]='s';
	message_smr[7]='m';
	message_smr[8]='r';
	message_smr[9]='-';
	message_smr[10]=(smr_time>>24);
	message_smr[11]=(smr_time>>16);
	message_smr[12]=(smr_time>>8);
	message_smr[13]=smr_time;
	message_smr[14]=(mission_cnt>>24);
	message_smr[15]=(mission_cnt>>16);
	message_smr[16]=(mission_cnt>>8);
	message_smr[17]=mission_cnt;
	message_smr[18]=0x20;
	message_smr[19]=0x0d;
	message_smr[20]=0x0a;
	
	Ax_Send(message_smr,21);
	
	debug_puts("\n r-g-c-smr-");
	debug_puthex(message_smr[10]);
	debug_puts(" ");
	debug_puthex(message_smr[11]);
	debug_puts(" ");
	debug_puthex(message_smr[12]);
	debug_puts(" ");
	debug_puthex(message_smr[13]);
	debug_puts("  ");
	debug_puthex(message_smr[14]);
	debug_puts(" ");
	debug_puthex(message_smr[15]);
	debug_puts(" ");
	debug_puthex(message_smr[16]);
	debug_puts(" ");
	debug_puthex(message_smr[17]);
	return 1;
}

/*************************************************
        �~�b�V����CPU�N��(�^�C�}�\��&�쓮���Ԏw��^)
*************************************************/
unsigned char ScheduledRequestMissionSwitch(volatile unsigned char* param, unsigned char param_length){
	int i,sms_length=0;
	sms_time=0;
	smr_time=0;
	
	/* . �̑O�͋N�����Ă��鎞�Ԃ�\�� */
	for(i=0;i<param_length;i++){
		if(param[i]==0x2e){
			sms_length=i;
			break;
		}
		sms_time=sms_time*10+(unsigned long int)(param[i]-48);
	}
	/* . �������o�Ă��Ȃ��ꍇ�̓G���[ */	
	if(sms_length==0){
		Ax_Send("r-g-c-sms-Error1 \r\n",19);
//		debug_puts("\n INVALID message header with SCI1: SMS needs . between two variables \n");
		return 1;
	}
	/* . �̌�͋N�����鎞�Ԃ�\�� */
	for(i=sms_length+1;i<param_length;i++){
		smr_time=smr_time*10+(unsigned long int)(param[i]-48);
	}
	
	/* sms_time���I�����鎞�Ԃɂ��� */
	sms_time=sms_time+smr_time;
	
	/* smr_time���傫�����Ȃ������m���߂� */
	/*
	if(smr_time>=4294967296||(param_length-sms_length>10)){
		int ex_smr=smr_time;
		sms_time=0;
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > the LIMIT. \r\n",50);
		debug_puts("\n INVALID message header with SCI1: smr_time beyonds the limit (4294967296) ");
		debug_puts("\n request is: ");
		debug_putdec(ex_smr);
		debug_puts("\n");
		return 1;
	}
	*/
	
	/* sms_time���傫�����Ȃ������m���߂� 
	if(sms_time>=(4294967296-smr_time)||(sms_length>10)){
		int ex_sms=sms_time;
		sms_time=0;	
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:sms_time > the LIMIT. \r\n",50);
		debug_puts("\n INVALID message header with SCI1: sms_time beyonds the limit (4294967296) ");
		debug_puts("\n request is: ");
		debug_putdec(ex_sms);
		debug_puts("\n");
		return 1;
	}
	*/
	

	/* smr_time�����݂̎��Ԃ𒴂��Ă��邩���m���߂� */
	/*
	else if(smr_time<mission_cnt){
		int ex_smr=smr_time;
		sms_time=0;
		smr_time=0;
		Ax_Send("r-g-c-smr-INVALID message:smr_time > mission_cnt. \r\n",52);
		debug_puts("\n INVALID message header with SCI1: smr_time beyonds mission_cnt ");
		debug_puts("\n request is: ");
		debug_putdec(ex_smr);
		debug_puts("\n obc_time is: ");
		debug_putdec(mission_cnt);
		debug_puts("\n");
		return 1;
	}
	*/
	/*
	debug_puts("\n param_length: ");
	debug_putdec(param_length-1);
	debug_puts("\n sms_length: ");
	debug_putdec(sms_length);
	debug_puts("\n smr_length: ");
	debug_putdec(param_length-sms_length-1);
	
	debug_puts("\n itu1_cnt time: ");
	debug_putdec(mission_cnt);
	debug_puts("\n sms_time time: ");
	debug_putdec(sms_time);	
	debug_puts("\n smr_time time: ");
	debug_putdec(smr_time);
	debug_puts("\n");
	*/
	i=0;
	unsigned char message_sms[32];
	message_sms[i++]='r';
	message_sms[i++]='-';
	message_sms[i++]='g';
	message_sms[i++]='-';
	message_sms[i++]='c';
	message_sms[i++]='-';
	message_sms[i++]='s';
	message_sms[i++]='m';
	message_sms[i++]='s';
	message_sms[i++]='-';
	message_sms[i++]=(smr_time>>24);
	message_sms[i++]=(smr_time>>16);
	message_sms[i++]=(smr_time>>8);
	message_sms[i++]=smr_time;
	message_sms[i++]=(sms_time>>24);
	message_sms[i++]=(sms_time>>16);
	message_sms[i++]=(sms_time>>8);
	message_sms[i++]=sms_time;
	message_sms[i++]=(mission_cnt>>24);
	message_sms[i++]=(mission_cnt>>16);
	message_sms[i++]=(mission_cnt>>8);
	message_sms[i++]=mission_cnt;
	message_sms[i++]=0x20;
	message_sms[i++]=0x0d;
	message_sms[i++]=0x0a;

	Ax_Send(message_sms,i);//���Ԃ񒷂��͂Q�T
	
	debug_puts("\n smr_time time:\t");
	debug_puthex(message_sms[10]);
	debug_puthex(message_sms[11]);
	debug_puthex(message_sms[12]);
	debug_puthex(message_sms[13]);
	debug_puts("\n sms_time time:\t");
	debug_puthex(message_sms[14]);
	debug_puthex(message_sms[15]);
	debug_puthex(message_sms[16]);
	debug_puthex(message_sms[17]);
	debug_puts("\n obc time:\t");
	debug_puthex(message_sms[18]);
	debug_puthex(message_sms[19]);
	debug_puthex(message_sms[20]);
	debug_puthex(message_sms[21]);
	return 1;
}

/*************************************************
        �~�b�V����CPU�V���b�g�_�E��
*************************************************/
unsigned char RequestMissionShutdown(void)
{	
	sci0_puts("c-p-c-msd \r\n");		// �~�b�V����CPU�V���b�g�_�E����PIC�ɗv��

	debug_puts("!!!!!!-- Request Mission Shutdown to Power --!!!!!!.\n");
	ITU2.TCR.BIT.CCLR = 1;		// GRA�̃R���y�A�}�b�`�ŃJ�E���g�N���A
	ITU.TISRA.BIT.IMIEA2 = 0;		// �^�C�}���荞�݋֎~
	ITU.TSTR.BIT.STR2 = 0;			// �J�E���g��~
	ITU2.TCNT = 0;					// �J�E���^�N���A
	
	SROM_EN = 0;					//1017�ŕύX�_
	SROM_CH = MAIN;
	
	Ax_Send("r-g-c-nsd \r\n",12);	// �n��ւ̃��v���C�̃^�C�~���O�ɂ��ėv����
//	debug_puts("r-g-c-nsd \r\n");
	
	return 1;
}



/*************************************************
        �~�b�V����CPU�����I��
*************************************************/
unsigned char PronounceMissionAbort(void)
{
	debug_puts("!!!!!!-- Pronounce Shutodown to Mission --!!!!!!.\n");
	debug_puts("r-g-c-asd \r\n");
	
	sci2_puts("c-m-c-asd \r\n");
	
	// �^�C���A�E�g�����p�^�C�}(ITU2)�J�E���g�J�n
	ITU.TISRA.BIT.IMFA2 = 0;	// ���荞�݃X�e�[�^�X�t���O�N���A
	ITU.TISRA.BIT.IMIEA2 = 1;	// �^�C�}���荞�݋���
	ITU.TSTR.BIT.STR2 = 1;		// �J�E���g�J�n
	
//	Ax_Send("r-g-c-asd \r\n",12);
	
	return 1;
}

/*************************************************
	�~�b�V����CPU�����V���b�g�_�E���p�^�C�}�ݒ�
*************************************************/
unsigned char itu2_init(void)
{
	ITU.TSTR.BIT.STR2 = 0;		// �J�E���g��~
	ITU.TSTR.BIT.STR2 = 0;		// �J�E���g��~
	ITU2.TCNT = 0;				// �J�E���g�N���A
	
	ITU2.TCR.BIT.CCLR = 1;		// GRA�̃R���y�A�}�b�`�ŃJ�E���g�N���A
	ITU2.TCR.BIT.TPSC = 3;		// �^�C�}�v���X�P�[�� 25MHz/8=3.125MMHz T=0.32[usec]
	ITU2.GRA = 31250-1;			// ����10ms
	ITU2.GRB = 31250-1;			// ����10ms

	
	ITU.TISRA.BIT.IMIEA2 = 0;	// �^�C�}���荞�݋֎~
	ITU.TISRB.BIT.IMIEB2 = 0;	// �^�C�}���荞�݋֎~

	
	return 1;
}

#define TIMEOUT	1000	// Arduino�����V���b�g�_�E���F1000*10msec = 10sec
/*************************************************
		�~�b�V����CPU�����I���^�C���A�E�g
*************************************************/
void INT_IMIA2(void){		
	ITU.TISRA.BIT.IMFA2 = 0;		// ���荞�݃X�e�[�^�X�t���O�N���A
	mission_timeout++;
	
	if(mission_timeout == TIMEOUT){
		debug_puts("\n !!!!!!!-- Mission Time Out --!!!!!!!\n");		
		sci0_puts("c-p-c-msd \r\n");// PIC�ɃV���b�g�_�E�����˗�
		
		mission_timeout = 0;
		ITU.TISRA.BIT.IMIEA2 = 0;	// �^�C�}���荞�݋֎~
		ITU.TSTR.BIT.STR2 = 0;		// �J�E���g��~
		ITU2.TCNT = 0;				// �J�E���^�N���A
		Ax_Send("r-g-p-msd \r\n",12);
//		debug_puts("r-g-p-msd \r\n");
		
		SROM_EN = 0;		//1017�ŕύX�_
		SROM_CH = MAIN;		//1017�ŕύX�_
		
		
	}
}

/*************************************************
		OBC���Ԃ̊m�F
*************************************************/
unsigned char CheckNowTime(void){
	unsigned char message_cnt[17];
	message_cnt[0]='r';
	message_cnt[1]='-';
	message_cnt[2]='g';
	message_cnt[3]='-';
	message_cnt[4]='c';
	message_cnt[5]='-';
	message_cnt[6]='c';
	message_cnt[7]='n';
	message_cnt[8]='t';
	message_cnt[9]='-';
	message_cnt[10]=(mission_cnt>>24);
	message_cnt[11]=(mission_cnt>>16);
	message_cnt[12]=(mission_cnt>>8);
	message_cnt[13]=mission_cnt;
	message_cnt[14]=0x20;
	message_cnt[15]=0x0d;
	message_cnt[16]=0x0a;
	
	Ax_Send(message_cnt,17);
/*	
	debug_puts("\n r-g-c-cnt-");
	debug_puthex(message_cnt[10]);
	debug_puts(" ");
	debug_puthex(message_cnt[11]);
	debug_puts(" ");
	debug_puthex(message_cnt[12]);
	debug_puts(" ");
	debug_puthex(message_cnt[13]);	
*/	
	debug_puts("\n itu1_cnt time: ");
	debug_putdec(mission_cnt);
	debug_puts("\n sms_time time: ");
	debug_putdec(sms_time);	
	debug_puts("\n smr_time time: ");
	debug_putdec(smr_time);
	debug_puts("\n");
}

/*************************************************
		�f�W�g�[�J�[ON
*************************************************/
unsigned char DegitalkerON(void){
	DT_mode=1;
	fmptt_off();
	sci0_puts("c-p-c-don \r\n");
	debug_puts("DegitalkerON.\n");
	Ax_Send("r-g-c-don \r\n",12);	
	return 1;
}
/*************************************************
		�f�W�g�[�J�[OFF
*************************************************/
unsigned char DegitalkerOFF(void){
	DT_mode=0;
	fmptt_off();	
	sci0_puts("c-p-c-dof \r\n");
	debug_puts("DegitalkerOFF.\n");	
	Ax_Send("r-g-c-dof \r\n",12);	//FinishCommunication()�Ɉړ�
	return 1;
}

/*************************************************
		�~�b�V��������̕Ԏ�
*************************************************/
unsigned char Eco(void){
	debug_puts("\n Got ECO message \n");
	return 1;
}

