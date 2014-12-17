#include "Powerboard.h"


void controllCW(void){
	restart_wdt();
	if(PLLflag==PLLSETOM){
		PLLflag=PLLUNSET;	
		PLLRcounter=0;
		task_cw=SETPLL_TASKCW;
		return;
	}else if(PLLflag==CWOFF){
		cwptt_on();//KEY=GND
		return;
	}else if(PLLflag==CWWILL){
		cwptt_off();
		return;
	}
	/*else if(PLLRcounter++>CWRESETINTERVAL){
		cwptt_off();
		output_high(CWLINE);
		for(i=0;i<45;i++){
			restart_wdt();
			delay_ms(10);//delay関数によるwdt発動対策
		}
		
		output_low(CWLINE);
		for(i=0;i<45;i++){
			restart_wdt();
			delay_ms(10);//delay関数によるwdt発動対策
		}
		PLLflag=PLLUNSET;
		PLLRcounter=0;
		task_cw=SETPLL_TASKCW;
		return;
	}*/
	
	
	if(bit_test(k1,k0)){
		cwptt_on();
	}else{
		cwptt_off();
	}
	k0++;
	if(k0>k2)
		task_cw=SETCW_TASKCW;
	MWflag=0;
	
	return;
}

void setCW(){
	uint8_t buf;
	char nbuf;
	restart_wdt();
	k1=0;k0=0;
	
	if(willflag==0){
		if(OBCtime1>MAXOBCTIME){
			OBCtime2++;
			OBCtime1=0;
		}
		
		if(OBCtime2>MAXOBCTIME){
			OBCtime3++;
			OBCtime2=0;
		}

		if(OBCtime3>MAXOBCTIME){
			OBCtime1=0;
			OBCtime2=0;
			OBCtime3=0;
		}
		if(s0==as[phase]){
			s0=0;
			phase++;
			if(phase==1 && dpltdflag==1){
				comflag=2;
			}
			if(phase>5){
				s0=0;
				phase=0;
				OBCtime1++;
				output_low(H8KILL);	
				output_low(RXLINE);
				if(dpltdflag==1){
					comflag=1;
				}
			}
			task_cw=INTERVAL_TASKCW;
			return;	
		}

		switch(phase){
			case 0: nbuf=callsign(s0++);
					break;
			case 1: nbuf=assignment1(s0++);
					break;
			case 2: nbuf=assignment2(s0++);
					break;	
			case 3: nbuf=assignment3(s0++);
					break;
			case 4: nbuf=assignment4(s0++);
					break;
			case 5: nbuf=assignment5(s0++);
					break;
			/*case 6: nbuf=assignment6(s0++);
					break;*/
			default:break;
		}	
	}else{
		if(OBCtime1>MAXOBCTIME){
			OBCtime2++;
			OBCtime1=0;
		}
		
		if(OBCtime2>MAXOBCTIME){
			OBCtime1=0;
			OBCtime2=0;
		}
		if(s0>39){
			s0=0;
			OBCtime1++;
			task_cw=INTERVAL_TASKCW;
			return;
		}
		nbuf=lastwill_message(s0++);
	}

	buf=cw_encoder(nbuf);

	int8_t v;
	v = 0;									// cwを右から1ビットずつ調べる
	while(!bit_test(buf, v)&&v<7) {				// "1"があれば、左からその"1"の1つ手前までがモールス符号
		v++;
	}
	restart_wdt();
	j=7-v;
	for(v=0;v<j;v++){
		if(bit_test(buf,7-v)){
			bit_set(k1,k0++);		
			bit_set(k1,k0++);			
			bit_set(k1,k0++);			
			k0++;			
		}else{
			bit_set(k1,k0);
			k0+=2;			
		}
	}
	k0+=2;
	k2=k0;
	k0=0;
	//d=0;
	task_cw=CONTROLLCW_TASKCW;
	return;
}

/*CWとつぎのCWの間に間隔をあける。　コマンドによりその間隔を地上から自由に変えられるようにする。*/
void intervalCW(){
	interval_count++;
	if(interval_count > MAXTIME){
		interval_count=0;
		output_high(CWLINE);		
		task_cw=SETPLL_TASKCW;
	}
}

char callsign(uint8_t s){
	switch(s){
		case 0:return CALLSIGNA;
		case 1:return CALLSIGNB;
		case 2:return CALLSIGNC;
		case 3:return CALLSIGND;
		case 4:return CALLSIGNE;
		case 5:return CALLSIGNF;
	}
}//コールサインとご挨拶

#separate
char assignment1(uint8_t s){
	switch(s){
		case 0:return 'a';
		case 1:return 's';
		case 2:return '1';
		case 3:return 't';
		case 4:return 'h';
		case 5:return 'e';
		case 6:return '_';
		case 7:return 'f';
		case 8:return 'i';
		case 9:return 'r';
		case 10:return 's';
		case 11:return 't';
		case 12:return '_';
		case 13:return 'a';
		case 14:return 'r';
		case 15:return 't';
		case 16:return '_';
		case 17:return 's';
		case 18:return 'a';
		case 19:return 't';
		case 20:return 'e';
		case 21:return 'l';
		case 22:return 'l';
		case 23:return 'i';
		case 24:return 't';
		case 25:return 'e';
		case 26:return '_';
		case 27:return 'i';
		case 28:return 'n';
		case 29:return '_';
		case 30:return 't';
		case 31:return 'h';
		case 32:return 'e';
		case 33:return '_';
		case 34:return 'w';
		case 35:return 'o';
		case 36:return 'r';
		case 37:return 'l';
		case 38:return 'd';
		case 39:return '_';
		case 40:return '_';
		case 41:return 'a';
		case 42:return 'r';
		case 43:return 't';
		case 44:return 's';
		case 45:return 'a';
		case 46:return 't';
		case 47:return '1';
		case 48:return ':';
		case 49:return 'i';
		case 50:return 'n';
		case 51:return 'v';
		case 52:return 'a';
		case 53:return 'd';
		case 54:return 'e';
		case 55:return 'r';
		case 56:return '_';
		case 57:return 'a';
		case 58:return 'r';
		case 59:return 't';
		case 60:return 's';
		case 61:return 'a';
		case 62:return 't';
		case 63:return '.';
		case 64:return 'j';
		case 65:return 'p';
		

		default:return 0;
	}
}//ご挨拶

#separate
char assignment2(uint8_t s){
	switch(s){
		case 0:return 'a';
		case 1:return 's';
		case 2:return '2';
		case 3:return convert_to_hex1(OBCtime3);//picのカウントしている時刻,オーバーフローしてとまるかチェック
		case 4:return convert_to_hex0(OBCtime3);
		case 5:return convert_to_hex1(OBCtime2);
		case 6:return convert_to_hex0(OBCtime2);
		case 7:return convert_to_hex1(OBCtime1);
		case 8:return convert_to_hex0(OBCtime1);
		case 9:return chgflag+48;//充放電(充電＝0、放電＝１）
		case 10:return convert_to_hex1(mainOBCRHour);
		case 11:return convert_to_hex0(mainOBCRHour);
		case 12:return convert_to_hex1(mainOBCRMinute);
		case 13:return convert_to_hex0(mainOBCRMinute);
		case 14:return convert_to_hex1(TLMCW19);//BAT_VOL
		case 15:return convert_to_hex0(TLMCW19);
		case 16:return dpltdflag+48;

		default:return 0;
	}
}//PICの時刻

#separate
char assignment3(uint8_t s){
	switch(s){
		case 0:return 'a';
		case 1:return 's';
		case 2:return '3';
		case 3:return mainOBCLiveflag+48;//H8
		case 4:return missionOBCLiveflag+48;//ARD
		case 5:return RXLiveflag+48;//RX
		case 6:return HeaterTest+48;//HTR
		case 7:return convert_to_hex1(TLMCW19);//BAT_VOL
		case 8:return convert_to_hex0(TLMCW19);
		case 9:return dpltdflag+48;
	
		default:return 0;
	}
}//スイッチングステータス

#separate
char assignment4(uint8_t s){
	switch(s){
		case 0:return 'a';
		case 1:return 's';
		case 2:return '4';
		case 3:return convert_to_hex1(TLMCW1);//H8電流値
		case 4:return convert_to_hex0(TLMCW1);
		case 5:return convert_to_hex1(TLMCW2);//ARD電流値
		case 6:return convert_to_hex0(TLMCW2);
		case 7:return convert_to_hex1(TLMCW3);//PIC電流値
		case 8:return convert_to_hex0(TLMCW3);
		case 9:return convert_to_hex1(TLMCW4);//RX電流値
		case 10:return convert_to_hex0(TLMCW4);
		case 11:return convert_to_hex1(TLMCW5);//CW電流値
		case 12:return convert_to_hex0(TLMCW5);
		case 13:return convert_to_hex1(TLMCW6);//TX電流値
		case 14:return convert_to_hex0(TLMCW6);
		case 15:return convert_to_hex1(TLMCW7);//HTR電流値
		case 16:return convert_to_hex0(TLMCW7);
		case 17:return convert_to_hex1(TLMCW8);//BUS電流値
		case 18:return convert_to_hex0(TLMCW8);
		case 19:return convert_to_hex1(TLMCW9);//BAT電流値
		case 20:return convert_to_hex0(TLMCW9);
		case 21:return convert_to_hex1(TLMCW18);//PNLALL電流値
		case 22:return convert_to_hex0(TLMCW18);
		case 23:return convert_to_hex1(TLMCW19);//BAT_VOL
		case 24:return convert_to_hex0(TLMCW19);
		case 25:return dpltdflag+48;

		default:return 0;
	}
}//AD変換データ１

#separate
char assignment5(uint8_t s){
	switch(s){
		case 0:return 'a';
		case 1:return 's';
		case 2:return '5';
		case 3:return convert_to_hex1(TLMCW19);//BAT電圧値
		case 4:return convert_to_hex0(TLMCW19);
		case 5:return convert_to_hex1(TLMCW20);//BUS電圧値
		case 6:return convert_to_hex0(TLMCW20);
		case 7:return convert_to_hex1(TLMCW21);//BAT温度１
		case 8:return convert_to_hex0(TLMCW21);
		case 9:return convert_to_hex1(TLMCW22);//BAT温度２
		case 10:return convert_to_hex0(TLMCW22);
		case 11:return convert_to_hex1(TLMCW23);//BAT温度３
		case 12:return convert_to_hex0(TLMCW23);
		case 13:return convert_to_hex1(TLMCW24);//CHRGA_FLAG
		case 14:return convert_to_hex0(TLMCW24);
		case 15:return convert_to_hex1(TLMCW25);//CHRGB_FLAG
		case 16:return convert_to_hex0(TLMCW25);
		case 17:return dpltdflag+48;

		default:return 0;
	}
}//AD変換データ3

/*
#separate
char assignment6(uint8_t s){
	switch(s){
		case 0:return 'a';
		case 1:return 's';
		case 2:return '6';
		case 3:return 'l';//メッセージ
		case 4:return 'o';
		case 5:return 'v';
		case 6:return 'e';
		case 7:return '_';
		case 8:return 'a';
		case 9:return 'n';
		case 10:return 'd';
		case 11:return '_';
		case 12:return 'p';
		case 13:return 'e';
		case 14:return 'a';
		case 15:return 'c';
		case 16:return 'e';
		case 17:return '_';
		
		default:return 0;
	}
}//メッセージ
*/

#separate
char lastwill_message(uint8_t s){
	switch(s){
		case 0: return 'w';
		case 1: return 'i';
		case 2: return 'l';
		case 3: return 'l';
		case 4: return convert_to_hex1(OBCtime2);
		case 5: return convert_to_hex0(OBCtime2);
		case 6: return convert_to_hex1(OBCtime1);
		case 7: return convert_to_hex0(OBCtime1);
		case 8: return convert_to_hex1(TLMCW21);
		case 9: return convert_to_hex0(TLMCW21);
		case 10: return 'm';
		case 11: return 'a';
		case 12: return 'y';
		case 13: return '_';
		case 14: return 't';
		case 15: return 'h';
		case 16: return 'e';
		case 17: return '_';
		case 18: return 'i';
		case 19: return 'n';
		case 20: return 'v';
		case 21: return 'a';
		case 22: return 'd';
		case 23: return 'e';
		case 24: return 'r';
		case 25: return '_';
		case 26: return 'b';
		case 27: return 'e';
		case 28: return '_';
		case 29: return 'w';
		case 30: return 'i';
		case 31: return 't';
		case 32: return 'h';
		case 33: return '_';
		case 34: return 'y';
		case 35: return 'o';
		case 36: return 'u';
		case 37: return '_';
		case 38: return '_';
	}
}


char convert_to_hex1(uint8_t data){
	int hexdata=data/16;
	return hex(hexdata);
}

char convert_to_hex0(uint8_t data){
	int hexdata=data%16;
	return hex(hexdata);
}

char hex(int dec){
	switch(dec) {
		case 0:	return '0';
		case 1:	return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		case 10: return 'a';
		case 11: return 'b';
		case 12: return 'c';
		case 13: return 'd';
		case 14: return 'e';
		case 15: return 'f';
	}
}

#separate
uint8_t cw_encoder(char morse) {
	switch(morse) {				/* 2進数表示*/
		case '0': return 252;	/* 11111100*/
		case '1': return 124;	/* 01111100*/
		case '2': return 60; 	/* 00111100*/
		case '3': return 28; 	/* 00011100*/
		case '4': return 12; 	/* 00001100*/
		case '5': return 4;  	/* 00000100*/
		case '6': return 132;	/* 10000100*/
		case '7': return 196;	/* 11000100*/
		case '8': return 228;	/* 11100100*/
		case '9': return 244;	/* 11110100*/
		
		case 'a': return 96; 	/* 01100000*/
		case 'b': return 136;	/* 10001000*/
		case 'c': return 168;	/* 10101000*/
		case 'd': return 144;	/* 10010000*/
		case 'e': return 64; 	/* 01000000*/
		case 'f': return 40; 	/* 00101000*/
		case 'g': return 208;	/* 11010000*/
		case 'h': return 8;  	/* 00001000*/
		case 'i': return 32; 	/* 00100000*/
		case 'j': return 120;	/* 01111000*/
		case 'k': return 176;	/* 10110000*/
		case 'l': return 72; 	/* 01001000*/
		case 'm': return 224;	/* 11100000*/
		case 'n': return 160;	/* 10100000*/
		case 'o': return 240;	/* 11110000*/
		case 'p': return 104;	/* 01101000*/
		case 'q': return 216;	/* 11011000*/
		case 'r': return 80; 	/* 01010000*/
		case 's': return 16; 	/* 00010000*/
		case 't': return 192;	/* 11000000*/
		case 'u': return 48; 	/* 00110000*/
		case 'v': return 24; 	/* 00011000*/
		case 'w': return 112;	/* 01110000*/
		case 'x': return 152;	/* 10011000*/
		case 'y': return 184;	/* 10111000*/
		case 'z': return 200;	/* 11001000*/
		
		case '.': return 86; 	/* 01010110*/
		case '-': return 134;	/* 10000110*/

		case '_': return 0;	/* 100011100*/
		
		default:  return 0;
	}
}

void cwptt_on(void) {
    output_low(CWKEY);
}

void cwptt_off(void) {
    output_high(CWKEY);
}

void CWInit(){
	t0=0;s0=0;s1=CWINDEXMAX;k0=0;k1=0;k2=0;PLLflag=PLLUNSET;
	output_low(CWDAT);
	output_low(CWSTB);
	output_low(CWCLK);
	PLLRcounter=0;
	setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
	setup_counters(RTCC_INTERNAL,WDT_2304MS);
	set_timer1(0);
	set_timer0(0);
	cwptt_off();
	interval_count=0;

	MAXTIME=200;

	willflag=0;

	as[0]=6;
	as[1]=66;
	as[2]=17;
	as[3]=10;
	as[4]=26;
	as[5]=18;
	as[6]=18;

	phase=0;

	OBCtime1=0;
	OBCtime2=0;
	OBCtime3=0;
	
	task_cw=SETPLL_TASKCW;
}


/**/
void out_bit_cw(short dat) {
	if (dat == 0) {
		output_low(CWDAT);
	}
	else {
		output_high(CWDAT);
	}
	
	delay_us(1);		// PLL ICのデータシートより0.1us以上
	
	//クロックを進める
	output_high(CWCLK);
	delay_us(1);		// PLL ICのデータシートより0.2us以上
	output_low(CWCLK);
}

/*strobe信号・・・終わりましたよーって言う合図*/
void strobe_cw() {
	output_high(CWSTB);
	delay_us(1);		// PLL ICのデータシートより0.2us以上
	output_low(CWSTB);
}


void out_bits_cw(int8_t num_bits, uint16_t dat) {
	int v;
	
	for (v = 0; v < num_bits; v++) {
		out_bit_cw(dat & 0x0001);
		dat >>=1;
	}
}


// Reference_dividerの設定 (Group code:"11")
#separate
void set_reference_divider_cw(uint16_t count) {
	
	// Refernce divider (12bit)
	out_bits_cw(12, count);
	
	// Refernce dividerのGroup code
	out_bit_cw(1);
	out_bit_cw(1);
	
	// ストローブ信号送信
	strobe_cw();
}


// Optional controlの設定 (Group code:"00")
#separate
void set_option_register_cw(
	int8_t charge_pump_tx,
	int8_t charge_pump_rx,
	int8_t lock_detector,
	int8_t standby_control_tx,
	int8_t standby_control_rx
	) {
	strobe_cw();
	out_bits_cw(3, 0);					// Test mode (Usually set up T1=T2=T3=0) 
	out_bits_cw(2, (uint16_t)charge_pump_tx);		// CPT1, CPT2
	out_bits_cw(2, (uint16_t)charge_pump_rx);		// CPR1, CPR2
	out_bits_cw(2, (uint16_t)lock_detector);		// LD1, LD2
	out_bit_cw(standby_control_tx);		// TX
	out_bit_cw(standby_control_rx);		// RX
	
	// Optional controlのGroup code
	out_bit_cw(0);
	out_bit_cw(0);
	
	// ストローブ信号送信
	strobe_cw();
}


// Programmable dividerの設定 (Group code: Tx="10", Rx="01")
#separate
void set_programmable_divider_cw(
	uint8_t swallow_counter,
	uint16_t programmable_counter,
	int8_t channel
	) {
	out_bits_cw(5,(uint16_t) swallow_counter);
	out_bits_cw(12, programmable_counter);
	
	
	if (channel == 1) {			// chanel=1: Tx
		out_bit_cw(1);
		out_bit_cw(0);
	}
	else if (channel == 2) {						// chanel=2: Rx
		out_bit_cw(0);
		out_bit_cw(1);
	}
	
	// ストローブ信号送信
	strobe_cw();
}

#separate
void pll_init_cw(
	uint16_t reference_divider, 
	uint8_t swallow_counter,
	uint16_t programmable_counter,
	int8_t channel,							//channel 1 : Tx "10"、channel 2 : Rx "01"
	int8_t charge_pump_tx,
 	int8_t charge_pump_rx,
 	int8_t lock_detector,
 	int8_t standby_control_tx,
 	int8_t standby_control_rx
 	) {
	set_reference_divider_cw(reference_divider);
	set_option_register_cw(charge_pump_tx, charge_pump_rx, lock_detector,
	standby_control_tx, standby_control_rx);
	set_programmable_divider_cw(swallow_counter, programmable_counter, channel);
}


// PLL設定 ------------------------------------------------------------------
void set_pll_cw(){
	t0=0;s0=0;s1=CWINDEXMAX;k0=0;k1=0;k2=0;PLLflag=PLLUNSET;
	PLLRcounter=0;
	cwptt_off();
	output_low(CWLINE);
	#ifdef _DEBUG_
	printf("c-p-c-plw \r\n");
	printf("r-c-p-dpw-pll \r\n");
	#endif
	
	int8_t v;
	for(v=0;v<20;v++){
		restart_wdt();
		delay_ms(10);//delay関数によるwdt発動対策
	}
	disable_interrupts(GLOBAL);//割り込み禁止
	pll_init_cw(
	0x0C2,				// refernce driver 		(Nref = 194）
	0x009,				// swallow counter 		(Nprg = 2121)
	0x042,				// programmable_counter (Nprg = 2121)
	1,					// channel 				(Group code "10")
	3,					// charge_pump_tx 		(CPT1 = 1, CPT2 = 1)
	3,					// charge_pump_rx 		(CPR1 = 1, CPR2 = 1)
	0,					// lock_detector 		(LD1 = 0, LD = 0)
	0,					// standby_control_tx 	(Tx = 0)
	1					// standby_control_rx 	(Rx = 1)
	); 
	enable_interrupts(GLOBAL);//割り込み許可
	for(v=0;v<90;v++){
		restart_wdt();
		delay_ms(10);//delay関数によるwdt発動対策
	}
	
	#ifdef _DEBUG_
	printf("c-p-c-pld \r\n");
	printf("r-c-p-pld \r\n");
	#endif
	
	task_cw=SETCW_TASKCW;
}

