/*********************************************************
 * ��
 *Created by Skyhorse�i���c ��n�j,2012/00/00
 *Copyright (c) 2012~. All rights reserved.
 *
 *********************************************************/

/**
 *  * �ʐM�p�o�b�t�@
 *   */

#ifndef __COMM_BUFFER_C__
#define __COMM_BUFFER_C__

#include"Rx_protcol.h"
/*
#define com_STARTFLAG_BUFFER_LENGTH 4

#define com_RX_BUFFER_LENGTH 128
#define com_HEADA_BUFFER_LENGTH 14
#define com_FOOTA_BUFFER_LENGTH 3
#define com_RSC_LENGTH 0

#define com_UPLINK_BUFFER_LENGTH 224 //163? �A�b�v�����N��M�o�b�t�@�� 

#include "stdlib.h"
static unsigned char *uplink_rx_buff;  // �A�b�v�����N��p�o�b�t�@
*/

int comm_buffer_init(void){
	int index = 0;
	uplink_rx_buff=(char *)malloc(sizeof(char) * com_UPLINK_BUFFER_LENGTH);
	for(index=0;index<com_UPLINK_BUFFER_LENGTH;index++){
		uplink_rx_buff[index] = 0x00;
	}
	return 1;
}
#endif

