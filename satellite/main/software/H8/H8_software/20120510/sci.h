/*************************************************************************************
* ファイル名　：　sci.h
* タイトル　　：　INVADERシリアル通信(CH0)用ヘッダファイル
* 製作者　　　：　03-110351 三好賢聖　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.29
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* sci0_putdeg()関数にバグあり。1の位が1に固定される。
*
*************************************************************************************/

#ifndef SCI_H_
#define SCI_H_

unsigned char sci0_init(unsigned short bps, unsigned char bit, unsigned char parity, unsigned char stopBit);
short sci0_putc(unsigned char data);
void sci0_puthex(unsigned char data);
void sci0_putdec(unsigned long data);
void sci0_putdoublehex(unsigned short data);
void sci0_putfourhex(unsigned long data);
void sci0_puteighthex(unsigned long data);
void sci0_putcrlf(void);
short sci0_puts(unsigned char *str);
short sci0_getc();

unsigned char sci1_init(unsigned short bps, unsigned char bit, unsigned char parity, unsigned char stopBit);
short sci1_putc(unsigned char data);
void sci1_puthex(unsigned char data);
void sci1_putdec(unsigned long data);
void sci1_putdoublehex(unsigned short data);
void sci1_putfourhex(unsigned long data);
void sci1_puteighthex(unsigned long data);
void sci1_putcrlf(void);
short sci1_puts(unsigned char *str);
void sci1_puts_rx(unsigned char *buff,int num);
short sci1_getc();

unsigned char sci2_init(unsigned short bps, unsigned char bit, unsigned char parity, unsigned char stopBit);
short sci2_putc(unsigned char data);
void sci2_puthex(unsigned char data);
void sci2_putdec(unsigned long data);
void sci2_putdoublehex(unsigned short data);
void sci2_putfourhex(unsigned long data);
void sci2_puteighthex(unsigned long data);
void sci2_putcrlf(void);
short sci2_puts(unsigned char *str);
short sci2_getc();

unsigned char debug_sci_init(void);
void debug_putc(unsigned char data);
void debug_puts(unsigned char *str);
short debug_getc(void);
void debug_puthex(unsigned long data);
void debug_putdec(unsigned long data);

#endif