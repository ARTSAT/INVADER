#include "Powerboard.h"

//初期化用関数
void IOInit(void){return ;}
//void ADCInit(void){return ;}
void TimerInit(void){return ;}
void CWInit(void){return ;}
void checkSelfDevices(void){
return ;
}

//void wait_AntennaInit();//アンテナを展開できる時間までまつ
//void AntennaInit();//アンテナ展開用

void resetInit(int8_t jobid){return ;}//前回やっていたジョブによってreset時の初期化を変える

//メインルーチン用
int8_t checkMainOBC(void){return 0;}//殺したら1をかえしてお仕事を登録する
void controlCW(void){return ;}
void controlBatteryTemp(void){return ;}
int8_t checkVandC(void){return 0;}//H8を殺したら１をかえす
int8_t processJobs(void){return 0;}//返り値はいらないかもしれない
int8_t callbackOthers(void){return 1;}//ArduinoやH8を起こすための関数


void registerJobs(int8_t id){return ;}//idで登録するジョブを指定する
/*ジョブidは
～仕様を考えましょう～

*/
void memoJobs(int8_t id){return;}//今やり始めるジョブを記憶する
int8_t getBeforeJobs(void){return 0;}//前回やっていたジョブを示す数値をもらう。