#ifndef FMTX_H_
#define FMTX_H_


// FM���M�֘A�̊֐� --------------------------------------------------
#define TXPTT_ON TXPTT=0;
#define TXPTT_OFF TXPTT=1;
#define FM_BUFFER_MAX 20
#define FM_LENGTH_MAX 256
#define MAX_DT_time 30 //�f�W�g�[�J�[�� 1.0 * (MAX_DT_time) [sec] �A���œ���
void init_modem(void);
unsigned char init_fm(void);
void delay_modem(void);
void send_bit(unsigned char);
void crcbit (unsigned char);
void send_byte(unsigned char);
void send_packet(volatile unsigned char *, int);
unsigned char TX(void);
unsigned char AX_protocol( volatile unsigned char *, int);
unsigned char Ax_Send( volatile unsigned char *, int);
unsigned char FM_buffer[FM_BUFFER_MAX][FM_LENGTH_MAX];//�ő�256�̒����̕�������ő�16�ۑ����A���X�ɑ��M����B
unsigned char FM_buffer_length[FM_BUFFER_MAX];	//������̒������L�^����B
unsigned char FM_buffer_num;		//���܂��Ă��镶����̔ԍ�����������B
unsigned char FM_sender_num;		//�������Ă��镶����̔ԍ�����������B
unsigned char FM_mode;				//0:�����Ȃ� 1:ton���� 2:���� 3:tof���� 4:�����Ă���Œ�
unsigned char DT_mode;				//0:�����Ȃ� 1:don����O 2:don���� 3:�����Ă���Œ� 4:dof����
unsigned int DT_time;
volatile unsigned char flag_tx[3];

void check_rx_working(void);


// �v���g�R���֌W�̕ϐ� -------------------------------------------
unsigned char  fcslo, fcshi;	/* �����������v�Z */
unsigned char  stuff;		//�r�b�g�X�^�b�t�B���O�p�J�E���^(1���A��5��ȏ㑱�����Ƃ���0��}��)
unsigned char  flag_send_fm;        //�v���A���u�����M��������
unsigned char  fcsflag_send_fm;        //FCS�v�Z��������
long int FmPreamble;	// �v���A���u���i�f�t�H���g�j

// // PLL�ݒ�֘A�̊֐��ƕϐ�---------------------------------------------
// extern unsigned char FMPLL;		// ���g����ς��ėp����ꍇ�p
// #ifndef byte
// #define byte unsigned char
// #endif
// void out_bit_tx(byte);
// void strobe_tx(void);
// void out_bits_tx(int, long);
// void set_reference_divider_tx(long);
// void set_option_register_tx(byte, byte, byte, byte, byte);
// void set_programmable_divider_tx(byte, long, int);
// void pll_init_tx(long, byte, long, int, byte, byte, byte, byte, byte);
// void set_pll_tx(void);

#endif 
/*FMTX_H_*/