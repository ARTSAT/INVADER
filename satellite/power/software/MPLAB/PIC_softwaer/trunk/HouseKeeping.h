//RCVBUFFER�̍ő�l
#define RCVBUFFERMAX 	25
#define CMDBUFFERMAX 	25
#define CMDLENGTH	 	9
#define RCVCOUNTLIMIT	1
#define HKDLENGTH		8
#define RCVGETLINE		5
#define SWMISSIONLINE	1
#define NOSWMISSIONLINE	0
#define MISSIONKILLMSG	1
#define MISSIONRUNMSG	0

char send[11]="r-c-p-pwt";
//�ȉ��R�}���h���X�g
char rstcmd[4]="rst";//���C�������Z�b�g����
char pwtcmd[4]="pwt";//�e�����g���[�𑗂�
char msdcmd[4]="msd";//�~�b�V�������E��

#ifdef _DEBUG_
char errcmd1[4]="er1";//�^�X�N�Ǘ��ϐ��̃r�b�g���]�ւ̑Ώ��̌��S���m�F
#endif

char cmdBuffer[CMDBUFFERMAX];
char rcvBuffer[RCVBUFFERMAX];
int8_t rcv_rp;
int8_t rcv_wp;
int8_t rcv_length;
int8_t rcv_flag;
int8_t cmdIndex;
int8_t rcv_count;
short MISSIONSWSND;
short MISSIONSWMSG;

void sendtl(void);
void isr_rcv(void);
void HKInit(void);
char rcv_getc(void);
void rcv_cmd(void);

void wait_hk(void);
void HKerrCheck(void);
