/*************************************************************************************
* �t�@�C�����@�F�@teleme.h
* �^�C�g���@�@�F�@INVADER�e�����g���E�Z�b�g�p�w�b�_�t�@�C��
* ����ҁ@�@�@�F�@03-110317 �ؑ����I
* �����@�@�@�@�F�@������w�H�w���q��F���H�w��
* ������@�@�@�F�@2012.1.31
* ����@�@�@�@�F�@C
**************************************************************************************
* �����l��
*
*************************************************************************************/
#ifndef TELEME_H_
#define TELEME_H_

// EEPROM�̃A�h���X�}�b�v
// IIC0-0~3(128byte mode)	:�ڍח���
// IIC1-0~2(128byte mode)	:�e����
// IIC1-3(32byte mode)		:API�z�M����(0x0000~0x0BFF)�C���������(0x0C00,0x0C01)

#define MODE128	128						// 128byte�������݁E�ǂݍ��݃��[�h
#define PAGE_PER_EEPROM_MODE128 0x0400	// 128byte�������ݎ���EEPROM1������̃y�[�W��

#define MODE32 32						// 32byte�������݁E�ǂݍ��݃��[�h

#define API_SLAVE 3						// API�z�M���X���[�u�A�h���X
#define API_LASTPAGE 0x0BFF				// API�z�M�������݃A�h���X�̍ő�l

#define INITIAL_SLAVE 3					// ���������X���[�u�A�h���X
#define OBCTIME_PAGE 0x0C00				// ��������񏑂����݃A�h���X�iOBC�����j
#define ERRORCOUNT_PAGE 0x0C01			// ��������񏑂����݃A�h���X�i�G���[�J�E���g�j
#define ROUGH_SLAVE_PAGE 0x0C02	
#define ROUGH_PAGE 0x0C03
#define API_PAGE 0x0C04

#define BootModeLength 1				//�@BootMode�̒���
#define BootFlagLength 32				//�@BootFlag�̒���
#define CameraParamLength 18
#define MorseParamLength 18
#define DigitalkerParamLength 18
#define NoteParamLength 18
#define TextLength 18

volatile unsigned char flag_rteleme[3];	// �e�e�����g���������݃^�C�}���荞�݃t���O
volatile unsigned char flag_dteleme[3];	// �ڍ׃e�����g���������݃^�C�}���荞�݃t���O
volatile unsigned char flag_ateleme[3];	// API�e�����g���������݃^�C�}���荞�݃t���O
volatile unsigned char flag_iteleme[3];	// ��������񏑂����݃^�C�}���荞�݃t���O
volatile unsigned char flag_oteleme[3];	// �����p�e�����g���o�̓^�C�}���荞�݃t���O

volatile unsigned char ena_rteleme[3];	// �e�������[�`���C�l�[�u��
volatile unsigned char ena_dteleme[3];	// �ڍח������[�`���C�l�[�u��
volatile unsigned char ena_ateleme[3];	// API�f�[�^�������[�`���C�l�[�u��
volatile unsigned char ena_iteleme[3];	// ��������񃋁[�`���C�l�[�u��
volatile unsigned char ena_oteleme[3];	// �����p�e�����g�����[�`���C�l�[�u��

unsigned char InitTelemetry(void);
unsigned char SetTelemetry_Rough(void);
unsigned char SetTelemetry_Detail(unsigned char mode);
unsigned char SetTelemetry_API(void);
unsigned char SetTelemetry_Initial(void);
unsigned char Read_Initial(void);
unsigned char OBC_Majority(unsigned char data_set[], unsigned char i);
unsigned char Error_Majority(unsigned char data_set[], unsigned char i);

unsigned char Setup_DTelemetry(volatile unsigned char* param, unsigned char param_length);

unsigned char WriteIicBootMode(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicBootFlag(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicBootTime(volatile unsigned long time);
unsigned char WriteIicCameraParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicMorseParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicDigitalkerParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicNoteParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextDebug(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextZParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextYParam(volatile unsigned char* param, unsigned char param_length);
unsigned char WriteIicTextXParam(volatile unsigned char* param, unsigned char param_length);

unsigned char ReadSharedROM(volatile unsigned char* param, unsigned char param_length);
unsigned char LongReadSharedROM(volatile unsigned char* param, unsigned char param_length);

unsigned char GeneralWriteSharedMomery(volatile unsigned char* param, unsigned char param_length);
unsigned char GeneralWriteSharedAscii(volatile unsigned char* param, unsigned char param_length);
unsigned char GeneralReadSharedMomery(volatile unsigned char* param, unsigned char param_length);

unsigned char GeneralReadIIC0(volatile unsigned char* param, unsigned char param_length);
unsigned char GeneralReadIIC1(volatile unsigned char* param, unsigned char param_length);

static unsigned char rough_slave;
static unsigned long int rough_eep_page;
static unsigned long int api_eep_page;		// EEPROM�̃y�[�W�ԍ�



#endif