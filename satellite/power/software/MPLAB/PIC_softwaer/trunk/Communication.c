#include "Powerboard.h"

#int_rda
void isr_rcv(){
	rcvBuffer[rcv_wp++]=getc();
	if(rcv_wp>=RCVBUFFERMAX)
		rcv_wp=0;
    return;
}

void sendtl(void){
        restart_wdt();
        write_ResetMarker(RESET_SENDING);
        printf(send);
		int8_t tlmno;
		for(tlmno=0;tlmno<TLMMAX;tlmno++)
			putc((char)tlm[tlmno]);
	/*
		putc((char)tlm[PNLALL_VOL]);
		putc((char)tlm[BAT_VOL]);
		putc((char)tlm[BUS_VOL]);
		putc((char)tlm[PNLXP_CUR]);
		putc((char)tlm[PNLXM_CUR]);
		putc((char)tlm[PNLY1P_CUR]);
		putc((char)tlm[PNLY2P_CUR]);
		putc((char)tlm[PNLY1M_CUR]);
		putc((char)tlm[PNLY2M_CUR]);
		putc((char)tlm[PNLZP_CUR]);
		putc((char)tlm[PNLZM_CUR]);
		putc((char)tlm[BAT_CUR]);
		putc((char)tlm[PIC_CUR]);
		putc((char)tlm[MISSION_CUR]);
		putc((char)tlm[BUS_CUR]);
		putc((char)tlm[CW_CUR]);
		putc((char)tlm[TX_CUR]);
		putc((char)tlm[RX_CUR]);
		putc((char)tlm[MAIN_CUR]);
		putc((char)tlm[HTR_CUR]);
		putc((char)tlm[BAT_TEMP1]);
		putc((char)tlm[BAT_TEMP2]);
		putc((char)tlm[BAT_TEMP3]);
		putc((char)tlm[CHRGA_FLAG]);
		putc((char)tlm[CHRGB_FLAG]);
	*/
        restart_wdt();
        putc(' ');
        putc('\r');
        putc('\n');
        task_hk=WAIT_TASKHK;
		write_ResetMarker(RESET_OTHERS);
        return;
}

void HKInit(void){
        rcv_rp=0;
        rcv_wp=0;
        rcv_length=0;
        rcv_flag=0;
		rcv_count=0;
        cmdIndex=0;
        task_hk=WAIT_TASKHK;
		MISSIONSWSND=NOSWMISSIONLINE;
		MISSIONSWMSG=MISSIONRUNMSG;
		int8_t v;
		for(v=0;v<CMDLENGTH;v++)
			cmdBuffer[v]=0;
}

char rcv_getc(void){
        if(rcv_length==0)
                return 0;
        if(rcv_rp==RCVBUFFERMAX-1){
                rcv_rp=0;
                rcv_length--;
                return rcvBuffer[RCVBUFFERMAX-1];
        }else{
                rcv_length--;
                return rcvBuffer[rcv_rp++];
        }
}

void rcv_cmd(){
	rcv_rp=0;
	
	while(rcv_wp>0&&rcvBuffer[rcv_wp--]!=' ');
	if(rcv_wp>4&&rcvBuffer[rcv_wp-3]=='-'){
		cmdBuffer[rcv_rp++]=rcvBuffer[rcv_wp-2];
		cmdBuffer[rcv_rp++]=rcvBuffer[rcv_wp-1];
		cmdBuffer[rcv_rp++]=rcvBuffer[rcv_wp];
		cmdBuffer[rcv_rp]='\0';
		if(rcv_wp>9)
			rcv_wp=rcv_wp-9;
		else
			rcv_wp=0;
	}else{
		return;
	}
	if(strcmp(cmdBuffer,txscmd)==0){
		#ifdef _DEBUG_
		printf("r-c-p-txn \r\n");
		#endif
		TXKillflag1=TXKILLING;
		TXKillflag2=TXKILLING;
		TXKillflag3=TXKILLING;
	}else if(strcmp(cmdBuffer,txrcmd)==0){
		TXflag=TXONFLAG;
	}else if(strcmp(cmdBuffer,rstcmd)==0){
		mainOBCKillflag1=MAINOBCKILLING;
		mainOBCKillflag2=MAINOBCKILLING;
		mainOBCKillflag3=MAINOBCKILLING;
		mainOBCResetReason=MAINOBCKILLTORST;
			
		#ifdef _DEBUG_
		printf("r-c-p-rst \r\n");
		#endif
	}else if(strcmp(cmdBuffer,mrrcmd)==0){
		mainOBCRTimer=0;
		mainOBCRMinute=0;
		mainOBCRHour=0;
	}else if(strcmp(cmdBuffer,pwtcmd)==0){
        task_hk=SENDTLM_TASKHK;
        return;
	}else if(strcmp(cmdBuffer,msdcmd)==0){
		missionOBCKillflag1=MISSIONOBCKILLING;
		missionOBCKillflag2=MISSIONOBCKILLING;
		missionOBCKillflag3=MISSIONOBCKILLING;
	}else if(strcmp(cmdBuffer,msrcmd)==0){
		missionOBCRunflag=MISSIONOBCRUN;
	}else if(strcmp(cmdBuffer,cwpcmd)==0){
		PLLflag=PLLSETOM;//CWPLL再設定
		printf("r-c-p-cwp \r\n");
	}else if(strcmp(cmdBuffer,aefcmd)==0){
		antennaExpandflag=1;
	}else if(strcmp(cmdBuffer,cwscmd)==0){
		MAXTIME=200;
	}else if(strcmp(cmdBuffer,cwmcmd)==0){
		MAXTIME=400;
	}else if(strcmp(cmdBuffer,cwlcmd)==0){
		MAXTIME=600;
	}else if(strcmp(cmdbuffer,hibcmd)==0){
		dpltdflag=BATDPLTD;
	}else if(strcmp(cmdbuffer,wakcmd)==0){
		dpltdflag=BATNOTDPLTD;
	}else if(strcmp(cmdbuffer,wilcmd)==0){
		willflag=1;
		OBCtime1=0;
		OBCtime2=0;
		OBCtime3=0;
		s0=0;
		task_cw=SETPLL_TASKCW;
	}
	#ifdef _DEBUG_
		else if(strcmp(cmdBuffer,errcmd1)==0){
			task_pd=SKIP_TASKPD+1;
			task_bh=SKIP_TASKBH+1;
			task_pw=SKIP_TASKPW+1;
			//意図的にビット反転を起こすコマンド
		}else if(strcmp(cmdBuffer,htrcmd1)==0){
			HeaterEFTest=HEATER_ONEF;
		}else if(strcmp(cmdBuffer,htrcmd2)==0){
			HeaterEFTest=HEATER_OFFEF;
		}
	#endif
	#ifdef _DEBUGf_
		else if(strcmp(cmdbuffer,raecmd)==0){
			write_eeprom(ANTENNAEXFLAGAD1,0x00);
			write_eeprom(ANTENNAEXFLAGAD2,0x00);
			write_eeprom(ANTENNAEXFLAGAD3,0x00);
		}
	#endif
	else{
		printf("r-c-p-npc \r\n");
        rcv_wp=0;
	}
	task_hk=WAIT_TASKHK;
	return;
}

void wait_hk(void){
	if(rcv_wp>RCVGETLINE){
		task_hk=RCVCMD_TASKHK;
		rcv_count=0;
	}
	if(MISSIONSWSND==SWMISSIONLINE){
		if(MISSIONSWMSG==MISSIONKILLMSG){
			printf("r-c-p-msd \r\n");
			MISSIONSWSND=NOSWMISSIONLINE;
			MISSIONSWMSG=MISSIONRUNMSG;
		}else{
			printf("r-c-p-msr \r\n");
			MISSIONSWSND=NOSWMISSIONLINE;
			MISSIONSWMSG=MISSIONRUNMSG;
		}
	}
	return;
}

void HKerrCheck(){
	HKInit();
	return;
}