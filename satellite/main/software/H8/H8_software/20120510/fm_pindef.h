/* ==================================== */
/* 	 FM—p‚ÌH8ƒsƒ“’è‹`                   */
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
//#define FX_M1		P3DR.BIT.B3	//A15
#define FX_M1		P5DR.BIT.B0

#define FMSTB		P3DR.BIT.B7	//A19
#define FMDAT		P3DR.BIT.B6	//A17
#define FMCLK		P3DR.BIT.B5	//A17
#define FMPTT		P3DR.BIT.B4	//A16

#define FMCWSW		P1DR.BIT.B0	//CW switch
#define FMDTSW		P1DR.BIT.B1	//Degitalker switch


#endif /*PINDEF_H_*/
