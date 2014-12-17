#define BATTERYONTEMP (int16_t)185
//バッテリーヒーターオンの閾値
//29度(100)
//ヒーター常にオフ(255)
#define BATTERYOFFTEMP (int16_t)180
//バッテリーヒーターオフの閾値
//30度(95)

//テスト用の値なので、適宜変えてください
//12/20本番仕様

//12/27さらに変更

#define HEATER_ON	1
#define HEATER_OFF	0

#define HEATER_ONEF 23
#define HEATER_OFFEF 14
#define HEATER_NTEF	111


int8_t HeaterEFTest;
short HeaterTest;//shortから変更
//ヒーターのオンオフ

void HeaterInit(void);
void controlBatteryTemp(void);

void ONHeater(void);
void OFFHeater(void);

void BHerrCheck();