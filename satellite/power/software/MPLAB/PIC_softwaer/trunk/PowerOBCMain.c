#include "Powerboard.h"
#include "ADC.c"
#include "LiveSign.c"
#include "controlCW.c"
#include "ResetWorks.c"
#include "watchCurrent.c"
#include "controlBatteryHeater.c"
#include "PowerOperating.c"
#include "expandAntenna.c"
#include "Communication.c"
#include "getPowerData.c"
#include "watchBatteryVOL.c"

void main(void){
	POInit();
	delay_ms(10);
	
	#ifdef _DEBUG_
		printf("c-c-p-oso-%u \r\n",restart_cause());
	#endif
	
	//incResetCounts(read_ResetMarker());
	//write_ResetMarker(RESET_OTHERS);
	ADCInit();
	LSInit();
	WCInit();
	CWInit();
	HKInit();
	HeaterInit();
	PDInit();
	RSTInit();
	WBInit();

	enable_interrupts(INT_RDA);//��M���荞�݋���
	enable_interrupts(GLOBAL);//���荞�݋��A����Ŋ��荞�݂����߂ĉ\�ɂȂ�

	//�}���`�^�X�L���O������
	while(1){
		restart_wdt();//wdt�͂��������Ŏg���܂��傤�B
		
		while(get_timer1()<CWINTERVAL);//�|�[�����O�ɂ��܂����B
		set_timer1(0);
		//CW�^�X�N
		switch(task_cw){
			case CONTROLLCW_TASKCW:	controllCW();
									break;
			case SETCW_TASKCW:		setCW();
									break;
			case SETPLL_TASKCW:		set_pll_cw();
									break;
			case INTERVAL_TASKCW:	intervalCW();
									break;
			case SKIP_TASKCW: 		//�������Ȃ�
									break;
			default:				CWInit();
									task_cw=SETPLL_TASKCW;//���W�X�^���]�p
									break;			
		}//CW�I
		
		//�ʐM�^�X�N
		switch(task_hk){
			case WAIT_TASKHK:	wait_hk();
									break;
			case RCVCMD_TASKHK:		rcv_cmd();
									break;
			case SENDTLM_TASKHK:	sendtl();
									break;
			case SKIP_TASKHK:		//�������Ȃ�
									break;
			default:				HKerrCheck();;//EEPROM�Ȃǂ̃t���O���`�F�b�N���ׂ�
									break;
		}//�ʐM�I
		
		//�d���`�F�b�N�^�X�N
		switch(task_pw){
			case WATCHCURRENT_TASKPW: 	watchCurrent();
										break;
			case SKIP_TASKPW:			//�Ȃɂ����Ȃ�
										break;
			default:					WCerrCheck();//���W�X�^���]�p
										break;
		}//�d���Ď��I
		
		//�d������^�X�N
		switch(task_po){
			case WAIT_TASKPO: wait_PO();
					break;
			case OBCKILL_TASKPO: OBCKill();
					break;
			case MAINOBCSTANDUP_TASKPO: mainOBCStandup();
					break;
			case MISSIONOBCSTANDUP_TASKPO: missionOBCStandup();
					break;
			case TXUNITKILL_TASKPO		:TXUNITKILL();
					break;
			case RXUNITKILL_TASKPO		:RXUNITKILL();
					break;
			case COMUNITSTANDUP_TASKPO	:comUnitsStandup();
					break;
			case SKIP_TASKPO://�������Ȃ�
					break;
			default:task_po=WAIT_TASKPO;//���W�X�^���]�p
					break;
		}//�d������I
	
		//�o�b�e���[�q�[�^�[����
		switch(task_bh){
			case CONTROLLBH_TASKBH: controlBatteryTemp();
									break;
			case SKIP_TASKBH:		//�������Ȃ�
									break;
			default:				BHerrCheck();//EEPROM�̃t���O���`�F�b�N���ׂ�
									break;
		}//�o�b�e���q�[�^�[�I
		//�vADC�A�m�}���Ή�
		
		
		//�����M���`�F�b�N
		switch(task_ls){
			case GETLS_TASKLS: 	getLiveSign();
								break;
			case WAIT_TASKLS:	wait_LS();
								break;
			case SKIP_TASKLS:	//�������Ȃ�
								break;
			default:			task_ls=WAIT_TASKLS;//���W�X�^���]�p
								break;
		}//�����M���I

		//�A���e�i�W�J
		switch(task_ae){
			case ANTENNAEXPAND_TASKAE:antennaExpander();
									break;
			case WAIT_TASKAE		:wait_AntennaExpand();
									break;
			case INTERVAL_TASKAE	:interval_AntennaExpand();//�A���e�i�W�J�������I�ɍs�����߂̋x�~����
									break;
			case SKIP_TASKAE		://�������Ȃ�
									break;
			default:				AEerrCheck();//EEPROM�̃t���O���`�F�b�N
									break;
		}//�A���e�i�I
		
		//�e��d���f�[�^���W		
		switch(task_pd){
			case COLLECTPDATA_TASKPD:collectpdata();
									break;
			case SKIP_TASKPD		://�������Ȃ�
									break;
			default:				PDerrCheck();//EEPROM���̃t���O���`�F�b�N���ׂ�
									break;	
		}//pd�I
		
		//�d�r�[�d����
		switch(task_wb){
			case CHRGING_TASKWB		:chrging();
									break;
			case FULLED_TASKWB		:fulled();
									break;
			case DPLTD_TASKWB		:dpltd();
									break;
			default					:wbInit();
									task_wb=CHRGING_TASKWB;				
									break;
		}//�[�d�I���
		
		

		/*
		//hogehoge
		switch(task_hoge){
			case 	:

		}//hoge�I


		*/

	}//while�I

}//main�I