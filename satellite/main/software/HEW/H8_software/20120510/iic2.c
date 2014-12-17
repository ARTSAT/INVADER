/******************************************************************************
* �t�@�C�����@�F�@iic2.c
* �^�C�g���@�@�F�@EEPROM��i2c�ʐM���s���v���O�����B
* ����ҁ@�@�@�F�@03-110317 �ؑ����I, 03-103002 ���蒼��
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.28
* ����@�@�@�@�F�@C
*******************************************************************************
* �����l��
* EEPROM��I2C�ʐM�����邽�߂̃v���O����
* P5B6-SCL,P5B7-SDA�Ƃ��Ďg�p
*
******************************************************************************/
#include "iodefine.h"
#include "iic.h"

#define eepstop		IIC2_EEPSTOP
#define	eepread		IIC2_EEPREAD
#define eepwrite	IIC2_EEPWRITE
#define	eepreset	IIC2_EEPRESET
#define eepinit		IIC2_EEPINIT
#define	eepwait		IIC2_EEPWAIT
#define	writeb		IIC2_WRITEB
#define readb		IIC2_READB
#define eepstart	IIC2_EEPSTART
#define eepchange   IIC2_EEPCHANGE

#define		SCL_H	(P2DDR = P2_DDR = P2_DDR &= ~64)	// P2DDR.BIT.B6�݂̂�0�ɂ���(���͐ݒ�) ~64=1011 1111
#define		SCL_L	(P2DDR = P2_DDR |=  64)				// P2DDR.BIT.B6�݂̂�1�ɂ���(�o�͐ݒ�)  64=0100 0000
#define		SDA_H	(P2DDR = P2_DDR &= ~128)			// P2DDR.BIT.B7�݂̂�0�ɂ���(���͐ݒ�) ~128=0111 1111
#define		SDA_L	(P2DDR = P2_DDR |=  128)			// P2DDR.BIT.B7�݂̂�1�ɂ���(�o�͐ݒ�)  128=1000 0000
#define		WR_CYC	1250//80000						// �y�[�W�������݌�̑҂�����5ms�ȏ� 25MHz*5msec = 125000
#define		CYCLE	0//160								// [SCL]�p���X��(��10us) 25MHz*10us = 250

unsigned char P2_DDR;

/******************************************************************************
		wait()�֐�
******************************************************************************/
void eepwait(volatile unsigned long count)
{
	while (count--);
}

/******************************************************************************
		1�o�C�g�������� 
******************************************************************************/
static void writeb(unsigned char dat)
{
	unsigned char i;
	
	eepwait(CYCLE);
	for (i=0; i<=7; i++){
		if (dat & 0x80) SDA_H; else SDA_L;	// dat��BIT.B7��1�Ȃ�SDA_H�C0�Ȃ�SDA_L
		dat <<= 1;							// dat��BIT.B6��B7�ɓ���D
 		eepwait(CYCLE);
		SCL_H;								// SCL��High
		eepwait(CYCLE);
		while ((P2DR.BYTE & 64) == 0);		// SCL��High�Ŗ����Ȃ�܂Ń��[�v
		SCL_L;								// SCL_Low
	}
	
	SDA_H;									// SDA��High
	eepwait(CYCLE);							// SCL��High�C�A�N�m���b�W�M��
	SCL_H;
	eepwait(CYCLE);
	SDA_L;									// �X�g�b�v�R���f�B�V�����ƂȂ�̂�h�~(�J�n����)
	SCL_L;									// SCL��Low
}

/******************************************************************************
	1�o�C�g�ǂݏo��
	f=1: ACK��EEPROM�ɕԂ��Cf=0: ACK��EEPROM�ɕԂ��Ȃ�(NO ACK:�ǂݏo���I��)
******************************************************************************/
static void readb(unsigned char *dat, unsigned char f)
{
	unsigned char i;
	
	SDA_H;									// High-Imp
	eepwait(CYCLE);
	for (i=0; i<=7; i++){
		SCL_H;
		*dat <<= 1;
		if (P2DR.BYTE & 128) *dat |= 0x01;	// SDA��High�����͂���Ă���Ȃ�dat��1��Ԃ�
		eepwait(CYCLE);
		while ((P2DR.BYTE & 64) == 0);		// SCL��Low�ɂȂ�܂ő҂�
		SCL_L;
	}
	if(f) SDA_H; else SDA_L;				// ACK �ŏI��"H"�ŏI��
	eepwait(CYCLE);
	SCL_H;
	eepwait(CYCLE);
	SCL_L;
}

/******************************************************************************
		�X�^�[�g�R���f�B�V���� �` �f�o�C�X�A�h���X�ݒ� (RW: 0=write, 1=read)
******************************************************************************/
static void eepstart(unsigned char rw, unsigned char s_ad, unsigned char block)
{
	SCL_H;
	SDA_H;
	eepwait(CYCLE);
	SDA_L;                                // Start
	eepwait(CYCLE);
	SCL_L;
	//writeb((rw & 1) | 0xA0);
	//writeb(0xA0 | ( ((s_ad<<1)&(0x0e)) | rw&1 ));
	writeb(0xA0 | (block<<3)&(0x08) | ( ((s_ad<<1)&(0x06)) | rw&1 ));        // 1010-(block adress:1bit)-(�X���[�uadress:2bit)-(R/W:1bit)
	/*
	debug_puthex(0xA0 | (block<<3)&(0x08) | ( ((s_ad<<1)&(0x06)) | rw&1));   // for debug
	debug_puts("\r\n");
	*/
}

/******************************************************************************
		�X�g�b�v�R���f�B�V����
******************************************************************************/
void eepstop(void)
{
	SCL_L;
	SDA_L;
	eepwait(CYCLE);
	SCL_H;
	eepwait(CYCLE);
	SDA_H;
	eepwait(CYCLE);
}

/******************************************************************************
		EEP-ROM �ǂݏo��
		Entry	uchar page : EEP-ROM �y�[�W�ԍ�(0�`3FFFh)
				uchar *buf : �ǂݍ��݃f�[�^�o�b�t�@(8byte)�ւ̃|�C���^
******************************************************************************/
void eepread(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte)
{
	unsigned char i;
	unsigned char block=0;
	
	P2DR.BYTE &= ~0xc0;                  // ~0xc0 = 00111111 : �f�[�^���W�X�^�͏o�͎��ɐ�Ώ����Ƃ���"L"
	
	if(page > 0x1FF)	block = 1;
	eepstart(0,s_ad,block);
	
	unsigned char bit = eepchange(byte);
	
	/* �A�h���X�̎w�� */
	writeb((unsigned char)(page >> (8-bit)));   // ���ʂֈړ���A8�܂łɋK������
	writeb((unsigned char)(page << bit));   // 1�y�[�W128�o�C�g�̈�128�{����
	
	eepwait(WR_CYC);
	
	/* �f�[�^�̑��M */
	eepstart(1,s_ad,block);
	for (i=0; i<(byte-1); i++){
		readb(buf++,0);	
	}
	readb(buf,1);                         // �Ō��NO ACK�ŏI��
	eepstop();
}

/******************************************************************************
	EEPROM ��������
	Entry	unsigned char page : EEP-ROM �y�[�W�ԍ�(0�`3FFFh)
			unsigned char *buf : �������݃f�[�^�o�b�t�@(8bytes)�ւ̃|�C���^
******************************************************************************/
void eepwrite(unsigned long int page, unsigned char *buf, unsigned char s_ad, unsigned char byte)
{
	unsigned char i;
	unsigned char block=0;
	
	P2DR.BYTE &= ~0xc0;                  // ~0xc0 = 00111111�F�f�[�^���W�X�^�͏o�͎��ɐ�Ώ����Ƃ���"L"
	
	if(page > 0x1FF)	block = 1;
	eepstart(0,s_ad,block);
	
	unsigned char bit = eepchange(byte);
	
	/* �A�h���X�̎w�� */
	writeb((unsigned char)(page >> (8-bit)));   // ���ʂֈړ���A8�܂łɋK������
	writeb((unsigned char)(page << bit));   // 1�y�[�W128�o�C�g�ׁ̈C128�{����
	
	/* �f�[�^�̑��M */
	for (i=0; i<byte; i++){
		writeb(*buf++);
	}
	eepstop();
	eepwait(WR_CYC);
}

/******************************************************************************
		EEPROM �\�t�g���Z�b�g
******************************************************************************/
void eepreset(void)
{
	unsigned char i;
	
	for (i=0; i<9; i++){
		SDA_L;
		eepwait(CYCLE);
		SCL_L;
		eepwait(CYCLE);
		SDA_H;
		eepwait(CYCLE);
		SCL_H;
		eepwait(CYCLE);
	}
}

/*******************************************************************************
		EEPROM�p���o�͏�����
*******************************************************************************/
unsigned char eepinit(void)
{
	P2PCR.BYTE |= 0xC0;				// �v���A�b�v��R�ǉ�
	return 1;
}

/*******************************************************************************
		EEPROM�p�o�C�g���ϊ��֐�
		Entry unsigned long int:32byte��5
		�@�@�@�@�@�@�@�@�@�@�@�@128byte��7�ɕϊ�
*******************************************************************************/
unsigned char eepchange(unsigned char byte) {
	if(byte == 32) {
		byte = 5;
	}
	else if(byte == 128) {
		byte = 7;
	}
	return byte;
}