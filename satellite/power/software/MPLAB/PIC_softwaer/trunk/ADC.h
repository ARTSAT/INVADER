#define MUL_ANPIN_A 0
//�}���`�v���N�T�f�[�^�[�s��A�̔ԍ�
#define MUL_ANPIN_B 1
//�}���`�v���N�T�f�[�^�[�s��B�̔ԍ�
#define setMUL output_b

#define RXCURRENT		7
#define MAINOBCCURRENT	2
#define MISSIONOBCCURRENT 3
#define BATTERYTEMP1	4
#define BATTERYTEMP2	5
#define BATTERYTEMP3	6
//getCriticalDatas�p��port�ԍ�

uint8_t readADC(uint8_t);
void ADCInit(void);
uint8_t getCriticalDatas(uint8_t);