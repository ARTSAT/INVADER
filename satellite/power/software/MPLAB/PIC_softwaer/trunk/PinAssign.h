#define H8KILL 		PIN_D0
#define ARKILL 		PIN_D1
#define RXLINE		PIN_C4
#define TXLINE		PIN_C5
#define CWLINE		PIN_D5
//電源ライン制御ピン

#define	CWKEY		PIN_C0
#define CWCLK		PIN_C1
#define CWDAT		PIN_C2
#define CWSTB		PIN_C3
//CW制御ピン

#define BATTERYHEATER PIN_D2
//BATTERYHEATER制御ピン



#define LSRECIEVE	PIN_D4
//生存信号受信ピン

#define ANTENNAEXPAND PIN_D3
//アンテナ展開ピン

#define BATLINE	PIN_D6
#define BATRST	PIN_D7
//充電回路リセット

//EMではアンテナ展開がPIND2に、ヒーターがPIND3にそれぞれついています。
//FMと回路図ではアンテナ展開がPIND3に、ヒーターがPIND2にそれぞれついています。