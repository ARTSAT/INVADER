//RCVBUFFER�̍ő�l
#define RCVBUFFERMAX 	40
#define CMDBUFFERMAX 	10
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
char mrrcmd[4]="mrr";//���C���̃��Z�b�g���������Z�b�g����
char pwtcmd[4]="pwt";//�e�����g���[�𑗂�
char msdcmd[4]="msd";//�~�b�V�������E��
char msrcmd[4]="msr";//�~�b�V�������I���ɂ���
char txrcmd[4]="txr";//TX���I���ɂ���
char txscmd[4]="txs";//TX���I�t�ɂ���
char cwpcmd[4]="cwp";//CWPLL�Đݒ�
char aefcmd[4]="aef";//�A���e�i�W�J�I��
char cwscmd[4]="cws";//CW�̊Ԋu��
char cwmcmd[4]="cwm";//CW�̊Ԋu��
char cwlcmd[4]="cwl";//CW�̊Ԋu��
char hibcmd[4]="hib";//�����~�����[�h
char wakcmd[4]="wak";//�����~�����[�h����
char wilcmd[4]="wil";//�⌾���[�h

#ifdef _DEBUG_
char errcmd1[4]="er1";//�^�X�N�Ǘ��ϐ��̃r�b�g���]�ւ̑Ώ��̌��S���m�F
char htrcmd1[4]="hdr";//�q�[�^�[�I��
char htrcmd2[4]="hds";//�q�[�^�[�I�t
#endif

#ifdef _DEBUGf_
char raecmd[4]="rae";//�A���e�i�W�J�ĊJ
#endif

char cmdBuffer[CMDBUFFERMAX];
char rcvBuffer[RCVBUFFERMAX];
int8_t rcv_rp;
int8_t rcv_wp;
int8_t rcv_length;
int8_t rcv_flag;
int8_t cmdIndex;
int8_t rcv_count;
short MISSIONSWSND;//short����ύX
short MISSIONSWMSG;//����

void sendtl(void);
void isr_rcv(void);
void HKInit(void);
char rcv_getc(void);
void rcv_cmd(void);

void wait_hk(void);
void HKerrCheck(void);
