#define DPLTDLINE	(unsigned int)110
//�d���͊��̃��C��
#define NOTDPLTDLINE	(unsigned int)117
//�͊��t���O��艺���̃��C��
#define FULLLINE	(unsigned int)212
//���^���̃��C��
#define NOTFULLLINE	(unsigned int)200
//���^����艺���̃��C��
#define CHRGCOUNTLINE	6000
//�J�E���^�[�ɂ�郊�Z�b�g�̃��C��

#define BATDPLTDAD		0x2120

#define BATDPLTD	1
#define BATNOTDPLTD	0
#define GETBATVOL		readADC(0b01001110)
#define GETCHRGRSTFLAG	readADC(0b01001101)


short dpltdflag;//�o�b�e���[�͊��̃T�C��
short chgflag;//�[�d���Ȃ�1�A�����łȂ��Ȃ�0
uint8_t comflag;//�~�����ɒʐM�\�ɂ���

int8_t dpltdcount;

uint16_t dpltdtimer;
int16_t chrgcount;


void chrging();
//�`���[�W��
void dpltd();
//�o�b�e���[�͊�
void fulled();
//���^��
void WBInit();