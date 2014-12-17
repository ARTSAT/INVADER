#include "Powerboard.h"

void chrging(){
	chgflag=0;
	uint8_t bvol=GETBATVOL;
	
	if(dpltdcount==0){
		if(bvol>FULLLINE){
			task_wb=FULLED_TASKWB;
			output_low(BATLINE);
			chgflag=1;
		}else if(bvol<=DPLTDLINE){
			dpltdcount=1;
		}
	}

	if(dpltdcount>0){
		dpltdtimer++;
		if(dpltdtimer>600){
			if(bvol<=DPLTDLINE){
				dpltdcount++;
				#ifdef _DEBUGf_
					printf("r-c-p-dpw-dp%c \r\n",dpltdcount);
				#endif
			}else{
				dpltdcount=0;
			}
			dpltdtimer=0;
		}
	}
	
	if(dpltdcount>2){
		dpltdflag=BATDPLTD;
	}

	if(dpltdflag==BATDPLTD){
		dpltdcount=0;
		dpltdtimer=0;
		chrgcount=0;
		MAXTIME=600;
		write_eeprom(BATDPLTDAD,BATDPLTD);
		output_high(CWLINE);
		task_cw=SETPLL_TASKCW;
		output_low(ARKILL);
		delay_ms(100);
		missionOBCLiveflag=MISSIONOBCDEAD;
		output_high(H8KILL);
		delay_ms(100);
		mainOBCLiveflag=MAINOBCDEAD;
		output_high(RXLINE);
		delay_ms(100);
		RXLiveflag=0;
		output_low(TXLINE);
		delay_ms(100);		
		task_po=SKIP_TASKPO;
		task_ls=SKIP_TASKLS;
		task_wb=DPLTD_TASKWB;
	}

	if(chrgcount++>CHRGCOUNTLINE){
		output_low(BATRST);
		output_low(BATLINE);
		delay_us(10);
		output_high(BATRST);
		output_high(BATLINE);
		chrgcount=0;
	}//íËä˙ìIÇ…è[ìdÇ™é~Ç‹ÇÈÇÃÇ≈ÇªÇÍÇ÷ÇÃëŒçÙ

	#ifdef _DEBUGf_	
		if(chrgcount%600==0){
			printf("r-c-p-dpw-bat%dtemp%d%d%d%d \r\n",bvol,tlm[BAT_TEMP1],tlm[BAT_TEMP2],tlm[BAT_TEMP3],mainOBCLiveflag);
		}
	#endif
}

void dpltd(){
	uint8_t bvol=GETBATVOL;
	uint8_t pnlcur=tlm[PNLALL_CUR];
	
	if(chrgcount==600){
		comflag=2;
	}

	if(comflag==1){
		POInit();
		delay_ms(10);
	}

	if(comflag==2){
		output_low(ARKILL);
		missionOBCLiveflag=MISSIONOBCDEAD;
		output_high(H8KILL);
		mainOBCLiveflag=MAINOBCDEAD;
		output_high(RXLINE);
		RXLiveflag=0;
		output_low(TXLINE);
		comflag=0;
		task_po=SKIP_TASKPO;
		task_ls=SKIP_TASKLS;
	}

	if(bvol>=NOTDPLTDLINE && pnlcur<SUNSHINECUR){
		dpltdcount=1;
	}

	if(dpltdcount>0){
		dpltdtimer++;
		if(dpltdtimer>600){
			if(bvol>=NOTDPLTDLINE && pnlcur<SUNSHINECUR){
				dpltdflag=BATNOTDPLTD;
			}
			dpltdcount=0;
			dpltdtimer=0;				
		}
	}

	if(dpltdflag==BATNOTDPLTD){
		POInit();
		LSInit();
		delay_ms(10);
		MAXTIME=200;
		dpltdcount=0;
		write_eeprom(BATDPLTDAD,BATNOTDPLTD);
		task_ls=GETLS_TASKLS;
		task_wb=CHRGING_TASKWB;
		chrgcount=0;		
	}

	if(chrgcount++>CHRGCOUNTLINE){
		comflag=1;
		output_low(BATRST);
		delay_us(10);
		output_high(BATRST);
		output_high(BATLINE);
		chrgcount=0;
		#ifdef _DEBUGf_
			printf("r-c-p-dpw-dh8 \r\n");
		#endif
	}//íËä˙ìIÇ…è[ìdÇ™é~Ç‹ÇÈÇÃÇ≈ÇªÇÍÇ÷ÇÃëŒçÙ

	return ;
}

void fulled(){
	if(GETBATVOL<NOTFULLLINE){
		output_low(BATRST);
		delay_us(10);
		output_high(BATRST);
		output_high(BATLINE);
		task_wb=CHRGING_TASKWB;
	}
}

void WBInit(){
	output_low(BATRST);
	delay_us(10);
	output_high(BATRST);
	output_high(BATLINE);
	chgflag=1;
	task_wb=CHRGING_TASKWB;	
	chrgcount=0;
	dpltdtimer=0;
	dpltdcount=0;
	dpltdflag=read_eeprom(BATDPLTDAD);
}

