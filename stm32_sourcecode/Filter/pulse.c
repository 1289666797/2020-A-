#include "pulse.h"

uint16_t readData, preReadData;		   		// ��ȡ���� AD ֵ
uint16_t timeCount = 0;         // �������ڼ�������
uint16_t firstTimeCount = 0;    // ��һ������ʱ��
uint16_t secondTimeCount = 0;   // �ڶ�������ʱ��

uint16_t IBI, BPM, SIG;

uint16_t data[DATA_SIZE] = {0}; // �������ݻ���

uint16_t idx, filter, pulseCount, max, min, mid;			// �˲�ֵ

BOOL PULSE = FALSE;				// ��Ч����
BOOL PRE_PULSE = FALSE;         // ��ǰ��Ч����



uint16_t Get_Array_Max(uint16_t* array, uint32_t size)
{
    uint16_t max = array[0];
    uint32_t i;
    for(i = 1; i < size; i++)
    {
        if(array[i] > max)
            max = array[i];
    }
    return max;
}

uint16_t Get_Array_Min(uint16_t* array, uint32_t size)
{
    uint16_t min = array[0];
    uint32_t i;
    for(i = 1; i < size; i++)
    {
        if(array[i] < min)
            min = array[i];
    }
    return min;
}

