/******************************************************************************
* �t�@�C�����@�F�@iic.h
* �^�C�g���@�@�F�@IIC�ʐM�p�w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-103002�@�ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.28
* ����@�@�@�@�F�@C
*******************************************************************************
* �����l��
* EEPROM��I2C�ʐM�����邽�߂̃v���O����
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

