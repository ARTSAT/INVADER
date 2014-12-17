#define MAINOBCOVERCURRENT	20
#define MAINOBCRESET		5
//Main立ち上げにかける時間

#define ONEMINUTE			1100 //約一分
#define ONEHOUR				60 //約20分
#define MAINOBCRESETCYCLE	6
//メインをオフにする周期

#define MAINOBCKILLING		1
#define MAINOBCNOTKILLING	0
#define MAINOBCLIVING		1
#define MAINOBCDEAD			0
#define MAINOBCKILLTORST	0
#define MAINOBCKILLTOOVC	1
#define MISSIONOBCKILLING	1
#define MISSIONOBCNOTKILLING	0
#define MISSIONOBCRUN		1
#define MISSIONOBCNOTRUN	0
#define MISSIONOBCLIVING	1
#define MISSIONOBCDEAD		0

//OBCライン制御周り

#define RXSTANDUPTIME		20
#define TXONFLAG			0x71
#define TXLIVING			1
#define TXDEAD				0
#define TXKILLING			1
#define TXNOTKILLING		0
#define RXKILLING			1
#define RXNOTKILLING		0
//COMUNITSライン制御周り

int8_t mainOBCTimer;
int8_t mainOBCRMinute;
int8_t mainOBCRHour;
int16_t mainOBCRTimer;
int8_t mainStandupTime;
int8_t RXTimer;
uint8_t TXflag;

short mainOBCResetReason;//shortから変更(以下の全部)
short mainOBCKillflag1;
short mainOBCKillflag2;
short mainOBCKillflag3;
short mainOBCLiveflag;
short mainOBCRTimerflag;

short missionOBCLiveflag;
short missionOBCKillflag1;
short missionOBCKillflag2;
short missionOBCKillflag3;
short missionOBCRunflag;

short TXLiveflag;
short TXKillflag1;
short TXKillflag2;
short TXKillflag3;

short RXLiveflag;
short RXKillflag1;
short RXKillflag2;
short RXKillflag3;

void wait_PO(void);
void POInit(void);
//ベース
void missionOBCKill(void);
void missionOBCStandup(void);
void mainOBCKill(void);
void mainOBCStandup(void);
void OBCKill(void);
//OBC周り
void RXUNITKill(void);
void TXUNITKill(void);
void comUnitsStandup(void);