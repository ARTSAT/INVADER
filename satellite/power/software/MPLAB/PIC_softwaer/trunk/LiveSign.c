#include "Powerboard.h"

void LSInit(){
	task_ls=GETLS_TASKLS;
	LScount=0;
	preLS=input(LSRECIEVE);
	return;
}

void getLiveSign(void){
		if(preLS!=input(LSRECIEVE)){
			preLS=input(LSRECIEVE);
			LScount=0;
			return;
		}else if(LScount++>LSLIMIT){
			POInit();
			mainOBCKillflag1=MAINOBCKILLING;
			mainOBCKillflag2=MAINOBCKILLING;
			mainOBCKillflag3=MAINOBCKILLING;
			mainOBCResetReason=MAINOBCKILLTORST;
			TXKillflag1=TXKILLING;
			TXKillflag2=TXKILLING;
			TXKillflag3=TXKILLING;
			LScount=0;
			task_ls=WAIT_TASKLS;
		}
		if(mainOBCLiveflag==MAINOBCDEAD){
			task_ls=WAIT_TASKLS;
			LScount=0;
		}
		return;
}

void wait_LS(void){
	if(mainOBCLiveflag==MAINOBCLIVING){
		task_ls=GETLS_TASKLS;
		preLS=input(LSRECIEVE);
			LScount=0;
	}
	return;
}