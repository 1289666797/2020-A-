#ifndef __PULSE__H_
#define __PULSE__H_
#include "stdint.h"
#define DATA_SIZE 20

typedef enum {FALSE, TRUE} BOOL;
#define SAMPLE_PERIOD 2

extern uint16_t readData, preReadData;		   		// ��ȡ���� AD ֵ
extern uint16_t timeCount ;         // �������ڼ�������
extern uint16_t firstTimeCount ;    // ��һ������ʱ��
extern uint16_t secondTimeCount ;   // �ڶ�������ʱ��

extern uint16_t IBI, BPM, SIG;

extern uint16_t data[DATA_SIZE]; // �������ݻ���
extern uint16_t idx, filter, pulseCount, max, min, mid;;				// �˲�ֵ

extern BOOL PULSE ;				// ��Ч����
extern BOOL PRE_PULSE ;         // ��ǰ��Ч����



 uint16_t Get_Array_Max(uint16_t* array, uint32_t size);
 uint16_t Get_Array_Min(uint16_t* array, uint32_t size);


#endif