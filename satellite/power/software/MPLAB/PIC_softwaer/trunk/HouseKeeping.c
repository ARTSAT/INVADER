#include "Powerboard.h"

#int_rda
void isr_rcv(){
	rcvBuffer[rcv_wp++]=getc();
    return;
}

void sendtl(void){
        restart_wdt();
        write_ResetMarker(RESET_SENDING);
        printf(send);
        for(j=0;j<32;j++)
                putc((char)tlm[j]);
        restart_wdt();
        putc(' ');
        putc('\r');
        putc('\n');
        task_hk=WAIT_TASKHK;
		HKInit();
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
		for(i=0;i<CMDLENGTH;i++)
			cmdBuffer[i]=0;
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
		//do{
		//	cmdBuffer[rcv_rp]=rcvBuffer[rcv_wp];
		//}while(rcv_rp-->0&&rcv_wp-->0);
		
		cmdBuffer[rcv_rp++]=rcvBuffer[rcv_wp-2];
		cmdBuffer[rcv_rp++]=rcvBuffer[rcv_wp-1];
		cmdBuffer[rcv_rp++]=rcvBuffer[rcv_wp];
		cmdBuffer[rcv_rp]='\0';
	}else{
		HKInit();
        task_hk=SENDTLM_TASKHK;
		return;
	}
	if(strcmp(cmdBuffer,rstcmd)==0){
		mainOBCKillflag1=MAINOBCKILLING;
		mainOBCKillflag2=MAINOBCKILLING;
		mainOBCKillflag3=MAINOBCKILLING;
		mainOBCResetReason=MAINOBCKILLTORST;
			
		#ifdef _DEBUG_
		printf("r-c-p-rst \r\n");
		#endif
		HKInit();
	}else if(strcmp(cmdBuffer,pwtcmd)==0){
    	HKInit();
        task_hk=SENDTLM_TASKHK;
	}else if(strcmp(cmdBuffer,msdcmd)==0){
		missionOBCKillflag1=MISSIONOBCKILLING;
		missionOBCKillflag2=MISSIONOBCKILLING;
		missionOBCKillflag3=MISSIONOBCKILLING;
		HKInit();
	}
	#ifdef _DEBUG_
		else if(strcmp(cmdBuffer,errcmd1)==0){
			task_pd=SKIP_TASKPD+1;
			task_bh=SKIP_TASKBH+1;
			task_pw=SKIP_TASKPW+1;
			HKInit();
		}
	#endif
	return;
}

void wait_hk(void){
	if(rcv_wp>RCVGETLINE){
		if(rcvBuffer[rcv_wp-1]=='\n'||rcvBuffer[rcv_wp-1]=='\r'||rcvBuffer[rcv_wp-1]==' '){
			task_hk=RCVCMD_TASKHK;
			rcvBuffer[rcv_wp]='\0';
			rcv_count=0;
		}else if(++rcv_count>RCVCOUNTLIMIT){
			rcv_count=0;
			rcv_wp=0;
			task_hk=SENDTLM_TASKHK;
		}
	}
	if(MISSIONSWSND==SWMISSIONLINE){
		if(MISSIONSWMSG==MISSIONKILLMSG){
			printf("r-c-p-msd \r\n");
			MISSIONSWSND=NOSWMISSIONLINE;
			MISSIONSWMSG=MISSIONRUNMSG;
		}else{
			//T.B.D.
		}
	}
	return;
}

void HKerrCheck(){
	
	if(getfuncOnOff(UARTCUTFLAG)==FUNCON){
		task_hk=WAIT_TASKHK;
	}else{
		task_hk=SKIP_TASKHK;
	}
	return;
	
}