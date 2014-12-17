/******************************************************************************
* ファイル名　：　iic.h
* タイトル　　：　IIC通信用ヘッダファイル
* 製作者　　　：　03-103002　木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.28
* 言語　　　　：　C
*******************************************************************************
* ※備考※
* EEPROMとI2C通信をするためのプログラム
*
******************************************************************************/

#ifndef IIC_H_
#define IIC_H_

void IIC0_EEPSTOP(void);
void IIC0_EEPREAD(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte);
void IIC0_EEPWRITE(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte);
void IIC0_EEPRESET(void);
unsigned char IIC0_EEPINIT(void);
unsigned char IIC0_EEPCHANGE(unsigned char byte);

void IIC1_EEPSTOP(void);
void IIC1_EEPREAD(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte);
void IIC1_EEPWRITE(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte);
void IIC1_EEPRESET(void);
unsigned char IIC1_EEPINIT(void);
unsigned char IIC1_EEPCHANGE(unsigned char byte);

void IIC2_EEPSTOP(void);
void IIC2_EEPREAD(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte);
void IIC2_EEPWRITE(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte);
void IIC2_EEPRESET(void);
unsigned char IIC2_EEPINIT(void);
unsigned char IIC2_EEPCHANGE(unsigned char byte);



#endif

