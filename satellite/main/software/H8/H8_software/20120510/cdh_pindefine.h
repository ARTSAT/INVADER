/******************************************************************************
* ファイル名　：　cdh_pindefine.h
* タイトル　　：　C&DHピンアサインヘッダファイル
* 製作者　　　：　03-103002　木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.5.10
* 言語　　　　：　C
*******************************************************************************
* ※備考※
* Alive PulseがMPX_CHとポートを共有していることに注意
*
* TX1,RX1				:Serial Port for Debug
* TX0,RX0				:Serial Port for PowerCPU
* TX2,RX2				:Serial Port for MissionCPU
* 
* PA.BIT3				:AlivePulse to PIC(general IO)
*
* P2.BIT2				:EEPROM SCL(Telemetry ROM)
* P2.BIT3				:EEPROM SDA(Telemetry ROM)
* P2.BIT4				:EEPROM SCL(Reprogram ROM)
* P2.BIT5				:EEPROM SDA(Reprogram ROM)
* P2.BIT6				:EEPROM SCL(Shared ROM)
* P2.BIT7				:EEPROM SDA(Shared ROM)
* 
* P5.BIT0~BIT1			:AMI302-CH1,CH2
*
* P5.BIT2				:Shared ROM Switcing(0:Main,1:Mission)
* P5.BIT3				:Shared ROM Enable
* 
* PA.BIT0~BIT2			:MultiPlexer1 A0~A2
* PA.BIT4~BIT6			:MultiPlexer2 A0~A2
* P7.BIT0				:MultiPlexer1 DA
* P7.BIT1				:MultiPlexer1 DB
* P7.BIT2				:MultiPlexer2 DA
* P7.BIT3				:MultiPlexer2 DB
*
* P4.BIT7				:PIC-KILL SWITCH1
* PA.BIT7				:PIC-KILL SWITCH2
*
* P9.BIT4				:Mission Reset Switch
*
* P1					:COM
* P3					:COM
* P4					:COM
* IRQ4,IRQ5				:COM?
*
******************************************************************************/
#ifndef	CDH_PINDEF_H_
#define	CDH_PINDEF_H_

#include "iodefine.h"

#define APLS			PADR.BIT.B3			// 生存信号
#define MPX0_A0			PADR.BIT.B0			// マルチプレクサのスイッチング
#define MPX0_A1			PADR.BIT.B1
#define MPX0_A2			PADR.BIT.B2
#define MPX1_A0			PADR.BIT.B4
#define MPX1_A1			PADR.BIT.B5
#define MPX1_A2			PADR.BIT.B6

#define MPX_OUT			PADDR |= 0x77
#define APLS_OUT		PADDR |= 0x08 

#define AMICH1			P5DR.BIT.B0			// 磁気センサ測定軸のスイッチング
#define AMICH2			P5DR.BIT.B1
#define SROM_CH			P5DR.BIT.B2			// 0:H8, 1:Arduino
#define SROM_EN			P5DR.BIT.B3			// Shared ROM EN

#define AMI_OUT			P5DDR |= 0x03
#define SROM_OUT		P5DDR |= 0x0c

#define PIC_KILL1		P4DR.BIT.B7			//こちらが0かつ
#define PIC_KILL2		PADR.BIT.B7			//こちらが1でPICをシャットダウン

#define MI_RST			P9DR.BIT.B4			//ミッションCPUリセットスイッチ

//#define MI_SHSW			P5DR.BIT.B2			//Mission MEMS1（ミッションONでHigh, ミッションOFFでLow）

#endif