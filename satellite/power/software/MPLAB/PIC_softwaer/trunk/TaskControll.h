int8_t task_cw;//CW����^�X�N
int8_t task_hk;//�n�E�X�L�[�s���O�^�X�N
int8_t task_pw;//�d���d���`�F�b�N�^�X�N
int8_t task_po;//�d������^�X�N
int8_t task_bh;//�o�b�e���[�q�[�^�[����
int8_t task_ls;//�����M���^�X�N
int8_t task_ae;//�A���e�i�W�J�^�X�N
int8_t task_pd;//�d�̓f�[�^�[���W�^�X�N
int8_t task_wb;//�o�b�e���[�d���Ď��^�X�N



#define CONTROLLCW_TASKCW 0
#define SETCW_TASKCW 1
#define SETPLL_TASKCW 2
#define INTERVAL_TASKCW 5
#define SKIP_TASKCW 10
//TASK_CW

#define COLLECTHKD_TASKHK	0
#define RCVCMD_TASKHK		1
#define SENDTLM_TASKHK		2
#define SKIP_TASKHK			10
#define WAIT_TASKHK			0


#define WATCHCURRENT_TASKPW	0
#define SKIP_TASKPW			1

#define WAIT_TASKPO			0
#define OBCKILL_TASKPO		1
#define MAINOBCSTANDUP_TASKPO		2
#define MISSIONOBCSTANDUP_TASKPO	3
#define RXUNITKILL_TASKPO		13
#define TXUNITKILL_TASKPO		33
#define COMUNITSTANDUP_TASKPO	64
#define SKIP_TASKPO			10

#define CONTROLLBH_TASKBH	0
#define SKIP_TASKBH			1

#define GETLS_TASKLS		0
#define WAIT_TASKLS			1
#define SKIP_TASKLS			10

#define ANTENNAEXPAND_TASKAE	0
#define WAIT_TASKAE				11
#define INTERVAL_TASKAE			33
#define CHECK_TASKAE			63
#define SKIP_TASKAE				25	

#define COLLECTPDATA_TASKPD 	0
#define SKIP_TASKPD 			27

#define CHRGING_TASKWB			0
#define FULLED_TASKWB			11
#define DPLTD_TASKWB			63
//depleted battery