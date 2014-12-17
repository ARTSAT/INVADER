#ifndef FMTX_H_
#define FMTX_H_


// FM送信関連の関数 --------------------------------------------------
#define TXPTT_ON TXPTT=0;
#define TXPTT_OFF TXPTT=1;
#define FM_BUFFER_MAX 20
#define FM_LENGTH_MAX 256
#define MAX_DT_time 30 //デジトーカーは 1.0 * (MAX_DT_time) [sec] 連続で動く
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
unsigned char FM_buffer[FM_BUFFER_MAX][FM_LENGTH_MAX];//最大256の長さの文字列を最大16つ保存し、順々に送信する。
unsigned char FM_buffer_length[FM_BUFFER_MAX];	//文字列の長さを記録する。
unsigned char FM_buffer_num;		//たまっている文字列の番号をメモする。
unsigned char FM_sender_num;		//おくっている文字列の番号をメモする。
unsigned char FM_mode;				//0:何もない 1:tonする 2:送る 3:tofする 4:送っている最中
unsigned char DT_mode;				//0:何もない 1:donする前 2:donした 3:送っている最中 4:dofする
unsigned int DT_time;
volatile unsigned char flag_tx[3];

void check_rx_working(void);


// プロトコル関係の変数 -------------------------------------------
unsigned char  fcslo, fcshi;	/* 誤り訂正符号計算 */
unsigned char  stuff;		//ビットスタッフィング用カウンタ(1が連続5回以上続いたときに0を挿入)
unsigned char  flag_send_fm;        //プリアンブル送信中か判定
unsigned char  fcsflag_send_fm;        //FCS計算中か判定
long int FmPreamble;	// プリアンブル（デフォルト）

// // PLL設定関連の関数と変数---------------------------------------------
// extern unsigned char FMPLL;		// 周波数を変えて用いる場合用
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