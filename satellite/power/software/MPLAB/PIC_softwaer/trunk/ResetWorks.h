#define RESETMARKERAD	0x10
#define RESET_SENDING 	0x11
#define RESET_ADC		0x12
#define RESET_OTHERS	0x13
//リセットマーカーの値（アドレスも兼ねる）

#define ANTENNAEXFlAGAD1	0x00
#define ANTENNAEXFlAGAD2	0x01
#define ANTENNAEXFlAGAD3	0x02
//アンテナ展開済みフラグアドレス
#define ANTENNAEXPANDED	0x7f
//アンテナが展開済みならこれを書き込む
#define ANTENNATH		0x14
//アンテナ展開判定の閾値

#define ALLRESETCOUNTS	0x05
//PowerOBCリセット回数のアドレス

#define LIMITADC	0x70
#define LIMITSENDING	0x70
//機能切り離しのリミット

void write_ResetMarker(uint8_t);
uint8_t read_ResetMarker(void);
//リセットマーカーの書き出し、読み出し

void setAntennaExpandedFlag(void);
short getAntennaExpandFlag(void);//フラグ判定含む
//アンテナ展開フラグ

void incResetCounts(uint8_t);
uint8_t getResetCounts(uint8_t);
void clrResetCounts(uint8_t);
//各リセットカウントまわり

void RSTInit(void);

int8_t bitcounts(uint8_t);
short getfuncOnOff(uint8_t);
void setfuncOnOff(uint8_t,short);
//機能の切り離し