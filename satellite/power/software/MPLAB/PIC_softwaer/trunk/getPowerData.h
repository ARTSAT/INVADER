#define TLMMAX 28

//�}���`�v���N�T�P�AA�|�[�g
#define	PIC_CUR		0
#define BUS_CUR		1
#define BAT_CUR	2
#define PNLALL_CUR	3
#define PNLY2M_CUR	4
#define PNLY2P_CUR	5
#define PNLZM_CUR	6
#define PNLZP_CUR	7
//�}���`�v���N�T�P�AB�|�[�g
#define PNLY1M_CUR	8
#define PNLY1P_CUR	9
#define PNLXM_CUR	10
#define PNLXP_CUR	11
#define CHRGA_FLAG	12
#define CHRGB_FLAG	13
#define BAT_VOL		14
#define BUS_VOL		15
//�}���`�v���N�T2�AB�|�[�g
#define PNLALL_VOL	16
#define PIC_TEMP	17
#define ATN_CUR		18
#define HTR_CUR		19
#define TX_CUR		20
#define CW_CUR		21

//����ADC�s���Ɉ����Ă���z
#define RX_CUR		22
#define MAIN_CUR	23
#define MISSION_CUR	24
#define BAT_TEMP1	25
#define BAT_TEMP2	26
#define BAT_TEMP3	27

#define MPX2BMAX	6


uint8_t tlm[TLMMAX];
int8_t pdcount;


void collectpdata();
void PDInit();
void PDerrCheck();