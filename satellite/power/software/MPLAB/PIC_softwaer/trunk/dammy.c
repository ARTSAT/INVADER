#include "Powerboard.h"

//�������p�֐�
void IOInit(void){return ;}
//void ADCInit(void){return ;}
void TimerInit(void){return ;}
void CWInit(void){return ;}
void checkSelfDevices(void){
return ;
}

//void wait_AntennaInit();//�A���e�i��W�J�ł��鎞�Ԃ܂ł܂�
//void AntennaInit();//�A���e�i�W�J�p

void resetInit(int8_t jobid){return ;}//�O�����Ă����W���u�ɂ����reset���̏�������ς���

//���C�����[�`���p
int8_t checkMainOBC(void){return 0;}//�E������1���������Ă��d����o�^����
void controlCW(void){return ;}
void controlBatteryTemp(void){return ;}
int8_t checkVandC(void){return 0;}//H8���E������P��������
int8_t processJobs(void){return 0;}//�Ԃ�l�͂���Ȃ���������Ȃ�
int8_t callbackOthers(void){return 1;}//Arduino��H8���N�������߂̊֐�


void registerJobs(int8_t id){return ;}//id�œo�^����W���u���w�肷��
/*�W���uid��
�`�d�l���l���܂��傤�`

*/
void memoJobs(int8_t id){return;}//�����n�߂�W���u���L������
int8_t getBeforeJobs(void){return 0;}//�O�����Ă����W���u���������l�����炤�B