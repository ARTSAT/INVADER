/*#include"iodefine.h"

#define PLL_CLK     P3.DR.BIT.B0
#define PLL_DATA    P3.DR.BIT.B1
#define PLL_STB     P3.DR.BIT.B2
#define MX_M0       P3.DR.BIT.B3
#define DET         P3.DR.BIT.B4
#define FX_RXD      P4.DR.BIT.B7

#define TESTPIN     P4.DR.BIT.B1
*/

/* ==================================== */
/* 	 FM用のH8ピン定義                   */
/* 		by Shota Kikuchi 2012/01/30     */
/* ==================================== */

#ifndef PINDEF_H_
#define PINDEF_H_

#include "iodefine.h"


// Port 4

/* Port 3 */
//#define FX_RDYN		P4DR.BIT.B0	//A3
#define FX_RDYN		P3DR.BIT.B0	//A12
#define FX_CLK		P3DR.BIT.B1	//A13
#define FX_TXD		P3DR.BIT.B2	//A14
#define FX_M1		P3DR.BIT.B3	//A15

#define FMSTB		P3DR.BIT.B7	//A19
#define FMDAT		P3DR.BIT.B6	//A17
#define FMCLK		P3DR.BIT.B5	//A17
#define FMPTT		P3DR.BIT.B4	//A16

#define FMCWSW		P1DR.BIT.B0	//CW switch
#define FMDTSW		P1DR.BIT.B1	//Degitalker switch

#define FX_M0       P4DR.BIT.B6 //A9
#define FX_DET      P4DR.BIT.B5 //A8
#define FX_RXD      P4DR.BIT.B4 //A7
#define FX_CLK_RX   P4DR.BIT.B3 //A6

#define RX_CLK      P4DR.BIT.B0 //A3
#define RX_DAT      P4DR.BIT.B1 //A4
#define RX_STB      P4DR.BIT.B2 //A5

#define TESTPIN     P5DR.BIT.B0 //testpin出力




#endif