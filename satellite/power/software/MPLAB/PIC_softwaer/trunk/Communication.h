//RCVBUFFERの最大値
#define RCVBUFFERMAX 	40
#define CMDBUFFERMAX 	10
#define CMDLENGTH	 	9
#define RCVCOUNTLIMIT	1
#define HKDLENGTH		8
#define RCVGETLINE		5
#define SWMISSIONLINE	1
#define NOSWMISSIONLINE	0
#define MISSIONKILLMSG	1
#define MISSIONRUNMSG	0

char send[11]="r-c-p-pwt";
//以下コマンドリスト
char rstcmd[4]="rst";//メインをリセットする
char mrrcmd[4]="mrr";//メインのリセット周期をリセットする
char pwtcmd[4]="pwt";//テレメトリーを送る
char msdcmd[4]="msd";//ミッションを殺す
char msrcmd[4]="msr";//ミッションをオンにする
char txrcmd[4]="txr";//TXをオンにする
char txscmd[4]="txs";//TXをオフにする
char cwpcmd[4]="cwp";//CWPLL再設定
char aefcmd[4]="aef";//アンテナ展開終了
char cwscmd[4]="cws";//CWの間隔小
char cwmcmd[4]="cwm";//CWの間隔中
char cwlcmd[4]="cwl";//CWの間隔大
char hibcmd[4]="hib";//強制冬眠モード
char wakcmd[4]="wak";//強制冬眠モード解除
char wilcmd[4]="wil";//遺言モード

#ifdef _DEBUG_
char errcmd1[4]="er1";//タスク管理変数のビット反転への対処の健全性確認
char htrcmd1[4]="hdr";//ヒーターオン
char htrcmd2[4]="hds";//ヒーターオフ
#endif

#ifdef _DEBUGf_
char raecmd[4]="rae";//アンテナ展開再開
#endif

char cmdBuffer[CMDBUFFERMAX];
char rcvBuffer[RCVBUFFERMAX];
int8_t rcv_rp;
int8_t rcv_wp;
int8_t rcv_length;
int8_t rcv_flag;
int8_t cmdIndex;
int8_t rcv_count;
short MISSIONSWSND;//shortから変更
short MISSIONSWMSG;//同上

void sendtl(void);
void isr_rcv(void);
void HKInit(void);
char rcv_getc(void);
void rcv_cmd(void);

void wait_hk(void);
void HKerrCheck(void);
