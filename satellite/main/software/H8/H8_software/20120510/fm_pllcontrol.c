/* ================================================================================ */
/*                                                                                  */
/*                 FM PLL Control Program for INVADER BBM                           */
/*                                                                                  */
/* 				   Device driver for TB31213FNG (PLL IC)                            */
/* 		        		                                                            */
/* 										Written by Shota Kikuchi	                */
/* 										20012.01.30                                 */
/* 										                                            */
/* 										Based on "pll_control_fmtx.c" for PRISM	    */
/* 						                                                            */
/* ================================================================================ */


/* ---------------------------------------------------------------------- */
/*                                                                        */
/*   西無線のパラメータNprgの下位5ビットをswallow counterの値に、         */
/*   上位12ビットをprogrammable counterの値として指定する。               */
/*     	　Nprg = 87000　→ 1 0101 0011 1101 1000                          */
/*     					→ 下位5ビット ：1 1000                           */
/*     					　 上位12ビット：1010 1001 1110                   */
/*     					→ swallow_counter = 0x18                         */
/*     					 　programmable_counter = 0xA9E                   */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef FM_PLLCONTROL_C
#define FM_PLLCONTROL_C

#include "iodefine.h"
#include "fm_pindef.h"
#include "timer.h"
#include "fm_pllcontrol.h"
#include "sci.h"

//CW:435.000
//FM:437.200

void out_bit_fm(unsigned char dat) {
	if (dat == 0) {
		FMDAT = 0;
	}
	else {
		FMDAT = 1;
	}
//	debug_putc(dat);
	//クロックを進める
	FMCLK = 1;
	delay_us(2);
	FMCLK = 0;
	delay_us(2);
}


void strobe_fm() {
	FMSTB = 1;
	delay_us(2);
	FMSTB = 0;
	delay_us(2);
}


void out_bits_fm(int num_bits, long dat) {
	int i;
	
	for (i = 0; i < num_bits; i++) {
		out_bit_fm(dat & 0x0001);
		dat >>=1;
	}
}


void set_reference_divider_fm(long count) {
	
	// 12bitのreference divider送信
	out_bits_fm(12, count);
	
	// reference dividerのグループコード
	out_bit_fm(1);
	out_bit_fm(1);
	
	strobe_fm();
}


void set_option_register_fm(
	unsigned char charge_pump_tx,
	unsigned char charge_pump_rx,
	unsigned char lock_detector,
	unsigned char standby_control_tx,
	unsigned char standby_control_rx
	) {
	strobe_fm();
	out_bits_fm(3, 0);					// test mode (always 0)
	out_bits_fm(2, charge_pump_tx);
	out_bits_fm(2, charge_pump_rx);
	out_bits_fm(2, lock_detector);
	out_bit_fm(standby_control_tx);
	out_bit_fm(standby_control_rx);
	
	// optional controlのグループコード
	out_bit_fm(0);
	out_bit_fm(0);
	
	strobe_fm();
}


void set_programmable_divider_fm(
	unsigned char swallow_counter,
	long programmable_counter,
	int channel
	) {
	out_bits_fm(5, swallow_counter);
	out_bits_fm(12, programmable_counter);
	
	// programmable dividerのグループコード
	if (channel == 2) {
		out_bit_fm(0);
		out_bit_fm(1);
	}
	else {
		out_bit_fm(1);
		out_bit_fm(0);
	}
	
	strobe_fm();
}


void pll_init_fm(
	long reference_divider, 
	unsigned char swallow_counter,
	long programmable_counter,
	int channel,							//channel 1 : Tx "10"、channel 2 : Rx "01"
	unsigned char charge_pump_tx,
 	unsigned char charge_pump_rx,
 	unsigned char lock_detector,
 	unsigned char standby_control_tx,
 	unsigned char standby_control_rx
 	) {
	set_reference_divider_fm(reference_divider);
	set_option_register_fm(charge_pump_tx, charge_pump_rx, lock_detector,
	standby_control_tx, standby_control_rx);
	set_programmable_divider_fm(swallow_counter, programmable_counter, channel);
}


// PLL設定 ------------------------------------------------------------------
void set_pll_fm(){
	FMCWSW = 1;//0
	delay_ms(500);
	pll_init_fm(
	0xA00,				// refernce driver 		(Nref = 2560）
	0x10,				// swallow counter 		(Nprg = 87000)	before:0x18		after:0x10
	0xAAC,				// programmable_counter (Nprg = 87000)	before:0xA9E	after:0xAAC
	1,					// channel 				(Group code "10")
	3,					// charge_pump_tx 		(CPT1 = 1, CPT2 = 1)
	3,					// charge_pump_rx 		(CPR1 = 1, CPR2 = 1)
	0,					// lock_detector 		(LD1 = 0, LD = 0)
	0,					// standby_control_tx 	(Tx = 0)
	1					// standby_control_rx 	(Rx = 1)
	); 
	delay_ms(900);

}

#endif