#define DPLTDLINE	(unsigned int)110
//電源枯渇のライン
#define NOTDPLTDLINE	(unsigned int)117
//枯渇フラグ取り下げのライン
#define FULLLINE	(unsigned int)212
//満タンのライン
#define NOTFULLLINE	(unsigned int)200
//満タン取り下げのライン
#define CHRGCOUNTLINE	6000
//カウンターによるリセットのライン

#define BATDPLTDAD		0x2120

#define BATDPLTD	1
#define BATNOTDPLTD	0
#define GETBATVOL		readADC(0b01001110)
#define GETCHRGRSTFLAG	readADC(0b01001101)


short dpltdflag;//バッテリー枯渇のサイン
short chgflag;//充電中なら1、そうでないなら0
uint8_t comflag;//冬眠時に通信可能にする

int8_t dpltdcount;

uint16_t dpltdtimer;
int16_t chrgcount;


void chrging();
//チャージ中
void dpltd();
//バッテリー枯渇
void fulled();
//満タン
void WBInit();