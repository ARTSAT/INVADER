#define CALLSIGNA 'j'
#define CALLSIGNB 'q'
#define CALLSIGNC '1'
#define CALLSIGND 'z'
#define CALLSIGNE 'k'
#define CALLSIGNF 'k'
//コールサインです
//注：打ち上げ時には正式なコールサインに書き換えてください！！！！！！！！！！！
//これはPRISMのコールサインです。
//いいえ、これはINVADERのコールサインです。(10.28)

#define TLMCW1	tlm[MAIN_CUR]
#define TLMCW2	tlm[MISSION_CUR]
#define TLMCW3	tlm[PIC_CUR]
#define TLMCW4	tlm[RX_CUR]
#define TLMCW5	tlm[CW_CUR]
#define TLMCW6	tlm[TX_CUR]
#define TLMCW7	tlm[HTR_CUR]
#define TLMCW8	tlm[BUS_CUR]
#define TLMCW9	tlm[BAT_CUR]
#define TLMCW10	tlm[PNLXP_CUR]
#define TLMCW11	tlm[PNLY1P_CUR]
#define TLMCW12	tlm[PNLY2P_CUR]
#define TLMCW13	tlm[PNLZP_CUR]
#define TLMCW14	tlm[PNLXM_CUR]
#define TLMCW15	tlm[PNLY1M_CUR]
#define TLMCW16 tlm[PNLY2M_CUR]
#define TLMCW17	tlm[PNLZM_CUR]
#define TLMCW18	tlm[PNLALL_CUR]
#define TLMCW19	tlm[BAT_VOL]
#define TLMCW20	tlm[BUS_VOL]
#define TLMCW21	tlm[BAT_TEMP1]
#define TLMCW22	tlm[BAT_TEMP2]
#define TLMCW23	tlm[BAT_TEMP3]
#define TLMCW24	tlm[CHRGA_FLAG]
#define TLMCW25	tlm[CHRGB_FLAG]
//テレメトリーの中身です！！ボードの配線結果などによって書き換えます。

//char lastwill_project[118]="ARTSAT_PROJECT_Tama_Art_University_The_University_of_Tokyo_A.Kubota_T.Tanaka_N.Hirakawa_Y.Tazaki_K.Yasaka_K.Noguchi__";
//char lastwill_art[317]="ART_AND_DESIGN_TEAM_J.Horiguchi_R.Hashimoto_K.Furuya_T.Tsushima_K.Yokoyama_N.Ise_W.Kobara_K.Yamaguchi_H.Hirabayashi_S.Yamamori_N.Toyoshima_S.Ichikawa_S.Higa_J.Kojima_T.Inafuku_T.Yamaguchi_K.Mori_Y.Onishi_S.Kanno_K.Inoue_T.Narita_A.Tadokoro_H.Ogata_S.Takahashi_T.Nozawa_M.Aoki_E.Yamamoto_R.Sakamoto_ARTSAT_PBL_CLASS__";
//char lastwill_engineer[325]="ENGINEERING_TEAM_T.Obuchi_T.Arai_K.Sasaki_K.Nakazawa_S.Tonosaki_M.Kimura_N.Ozaki_K.Miyoshi_S.Kikuchi_S.Hamajima_S.Miyatani_K.Okada_H.Ishikawa_K.Namba_N.Usami_Y.Kotsubo_M.Nishiyama_S.Araki_E.Kuroiwa_Y.Tsuchiya_S.Egawa_K.Sawada_K.Shintani_M.Hosoda_Y.Tokitake_M.Kawanobe_T.Shimamoto_T.Enmei_K.Ariu_K.Miura_R.Setoguchi_M.Kaiho__";
//char lastwill_special[85]="Special_Thanks_To_M.Tsuji_Y.Mita_NOGATA_DENKI_NISHI_MUSEN_NAKASUKA-FUNASE_LAB_JAXA__";
//char lastwill_message[30]="MAY_THE_INVADER_BE_WITH_YOU__";

#define CWBUFMAX 10 	
//CWバッファー容量

#define T_UNIT 1
//CW送信間隔

#define CWINDEXMAX 22
//CWの文字数

#define CWOFF	87
#define PLLSETOM 13
#define PLLUNSET 0
#define CWWILL	35

#define CWINTERVAL (uint32_t)32768

#define CWRESETINTERVAL		30000
//CW送信間隔,52.4ms
//今の値
//・・・プリスケーラー3bit
//・・・4cycleで1インクリメント
//なので32768*4*8/20M=52.4ms

#define MAXOBCTIME	254

//CW周りの変数
int16_t t0;//t0用カウンタ
//char Sendbuf[CWBUFMAX]="exexexde";//CW送信バッファー
int16_t s0;//送信用インデックス
int8_t s1;//送信用バッファのMAXindex
int8_t k0;//今送信しているモールス要素のインデックス
int8_t k2;//今送信すべきモールス要素の総数
uint32_t k1;//送信中のデータのモールス要素
short MWflag;//CWセットの後に主な仕事ができるようにするためのフラグ、shortから変更
short willflag;
int8_t PLLflag;//PLL再設定
uint32_t PLLRcounter;//PLLReset用
int16_t interval_count;//CWの間隔カウント

uint8_t OBCtime1;
uint8_t OBCtime2;
uint8_t OBCtime3;

int8_t as[7];
int8_t phase;

uint16_t MAXTIME; //CWの間隔

//CW周り
void controllCW(void);
void CWTimer(void);
#separate uint8_t  cw_encoder(char);
void ptt       (int8_t);
void cwptt_on(void);//CWhigh
void cwptt_off(void);//CWlow
void setCW();//モールスをセットします
void intervalCW();//CWの間隔をきめます
char callsign(uint8_t);
#separate char assignment1(uint8_t);
#separate char assignment2(uint8_t);
#separate char assignment3(uint8_t);
#separate char assignment4(uint8_t);
#separate char assignment5(uint8_t);
//#separate char assignment6(uint8_t);
#separate char lastwill_message(uint8_t);
char convert_to_hex1(uint8_t);//16しんすうになおします
char convert_to_hex0(uint8_t);
char hex(int);



//以下PTTセット
void out_bit_cw(short);
void strobe_cw(void);
void out_bits_cw(int8_t, uint16_t);
#separate void set_reference_divider_cw(uint16_t);
#separate void set_option_register_cw(int8_t, int8_t, int8_t, int8_t, int8_t);
#separate void set_programmable_divider_cw(uint8_t, uint16_t, int8_t);
#separate void pll_init_cw(uint16_t, uint8_t, uint16_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t);
void set_pll_cw(void);