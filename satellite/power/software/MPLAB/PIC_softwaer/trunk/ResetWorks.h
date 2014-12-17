#define RESETMARKERAD	0x10
#define RESET_SENDING 	0x11
#define RESET_ADC		0x12
#define RESET_OTHERS	0x13
//���Z�b�g�}�[�J�[�̒l�i�A�h���X�����˂�j

#define ANTENNAEXFlAGAD1	0x00
#define ANTENNAEXFlAGAD2	0x01
#define ANTENNAEXFlAGAD3	0x02
//�A���e�i�W�J�ς݃t���O�A�h���X
#define ANTENNAEXPANDED	0x7f
//�A���e�i���W�J�ς݂Ȃ炱�����������
#define ANTENNATH		0x14
//�A���e�i�W�J�����臒l

#define ALLRESETCOUNTS	0x05
//PowerOBC���Z�b�g�񐔂̃A�h���X

#define LIMITADC	0x70
#define LIMITSENDING	0x70
//�@�\�؂藣���̃��~�b�g

void write_ResetMarker(uint8_t);
uint8_t read_ResetMarker(void);
//���Z�b�g�}�[�J�[�̏����o���A�ǂݏo��

void setAntennaExpandedFlag(void);
short getAntennaExpandFlag(void);//�t���O����܂�
//�A���e�i�W�J�t���O

void incResetCounts(uint8_t);
uint8_t getResetCounts(uint8_t);
void clrResetCounts(uint8_t);
//�e���Z�b�g�J�E���g�܂��

void RSTInit(void);

int8_t bitcounts(uint8_t);
short getfuncOnOff(uint8_t);
void setfuncOnOff(uint8_t,short);
//�@�\�̐؂藣��