/*************************************************************************************
* ファイル名　：　mission.h
* タイトル　　：　INVADERミッションテレメトリ送信リクエスト/受信プログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.3.30
* 言語　　　　：　C
**************************************************************************************
* ※備考※
*
*************************************************************************************/
#ifndef MISSIONT_H_
#define MISSIONT_H_

volatile unsigned char flag_misst[3];
volatile unsigned char ena_misst[3];

unsigned char InitMissionTelemetry(void);
unsigned char SendMissionTelemetry(void);
unsigned char RequestMissionShutdown(void);
unsigned char PronounceMissionAbort(void);
unsigned char RequestMissionReboot(void);
unsigned char ScheduledRequestMissionReboot(volatile unsigned char* , unsigned char );
unsigned char ScheduledRequestMissionSwitch(volatile unsigned char* , unsigned char );
unsigned char CheckNowTime(void);
unsigned char CheckPowerReply(void);
unsigned char DegitalkerON(void);
unsigned char DegitalkerOFF(void);
unsigned char Eco(void);





volatile unsigned long mission_timeout;

unsigned char itu2_init(void);

volatile unsigned char flag_smr[3];
volatile unsigned long int smr_time; 
volatile unsigned long mission_cnt;

volatile unsigned char flag_sms[3];
volatile unsigned long int sms_time;

volatile unsigned char flag_cpr[3];
unsigned char flag_ton;
unsigned char flag_tof;

#endif