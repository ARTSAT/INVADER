#define BATTERYONTEMP (int16_t)185
//�o�b�e���[�q�[�^�[�I����臒l
//29�x(100)
//�q�[�^�[��ɃI�t(255)
#define BATTERYOFFTEMP (int16_t)180
//�o�b�e���[�q�[�^�[�I�t��臒l
//30�x(95)

//�e�X�g�p�̒l�Ȃ̂ŁA�K�X�ς��Ă�������
//12/20�{�Ԏd�l

//12/27����ɕύX

#define HEATER_ON	1
#define HEATER_OFF	0

#define HEATER_ONEF 23
#define HEATER_OFFEF 14
#define HEATER_NTEF	111


int8_t HeaterEFTest;
short HeaterTest;//short����ύX
//�q�[�^�[�̃I���I�t

void HeaterInit(void);
void controlBatteryTemp(void);

void ONHeater(void);
void OFFHeater(void);

void BHerrCheck();