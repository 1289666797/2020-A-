/*******************************************************************************
* �ļ���	: main.c
* ��  ��	: GGTEAM
* ��  ��	: V1.0
* ��  ��	: 2020-10-07
* ��  ��	:
*******************************************************************************/
#include "sys.h"         //ϵͳ����
#include "delay.h"       //��ʱ
#include "usart.h"       //����  		
#include "led.h"
#include "ADS1292.h"
#include "Timer.h"
#include "dma.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
#include "pulse.h"
#include "stdbool.h"
void HMISendb(u8 k);

s32 get_volt(u32 num);//�Ѳɵ���3���ֽڲ���ת���з���32λ��
u8 text_buffer[128];
//main
#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.

u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
uchar_T Flag_Run=1;
extern unsigned int Uart2_Rx,Uart2_Tx;
u16 StepCount,Distance;
unsigned int Uart2_Sta;
extern u8 USART2_RX_BUF[UART_LENGTH];
extern unsigned int Uart2_Len;
u8 step_str[5];
u8 distance_str[5];


int main(void)
{
    u16 adcx;
   // u8 count=0;
    s32 int_val;
    double temp;
    double tem;
    u8 res,i,sum;
    u8 data_to_send[60];//���ڷ��ͻ���
    u8 usbstatus=0;
    s32 cannle[2];	//�洢����ͨ��������
    s32	p_Temp[2];	//���ݻ���
    // float temperature[10];
    u32 times_ECG,times_T;
    double  sum_T ,j;
	u32 count=0;
	u32 temp_count = 0;


    int32_t data_rdc_in;
	int16_t data_data;
	

    u32 data_count = 0;
	// ����������
	u32 status_byte;
	u8 LeadStatus;
	bool leadoff_deteted;
	long Mac = 0;
	s32 line_part;
	int16_t data_part[DATA_SIZE] = {0}; 
    //  float data_send[20];//���ڷ��ͻ���
	

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�


    Adc_Init();


//��ʼ��ϵͳʱ��	 72M
    SystemInit();
    delay_init();
    delay_ms(2000);
   
	
	
	uart1_init(115200);	 	//���ڳ�ʼ��Ϊ115200
    uart2_init(115200);
    DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);//����1DMA����
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA
    LED_Init();

    ADS1292_Init();	//��ʼ��ads1292
    while(Set_ADS1292_Collect(0))//0 �����ɼ�  //1 1mV1Hz�ڲ������ź� //2 �ڲ��̽���������
    {
        printf("1292�Ĵ�������ʧ��\r\n");
        delay_s(1);
  
    }
    printf("�Ĵ������óɹ�\r\n");
    delay_s(1);



    Subsystem_initialize();


    EXTI->IMR |= EXTI_Line5;//��DRDY�ж�




    while(1)//ѭ����������
    {

	
        if(	Flag_Run) {
			

			
	
			
            if(ads1292_recive_flag)
            {
				PAout(7) = 1;
                //	cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//��ȡԭʼ����
                cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
				status_byte=ads1292_Cache[0]<<16 | ads1292_Cache[1]<<8 | ads1292_Cache[2];//��ȡԭʼ����

                //p_Temp[0] = get_volt(cannle[0]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
                p_Temp[1] = get_volt(cannle[1]);	//�Ѳɵ���3���ֽ�ת���з���32λ��
          //     rtU.In1	= (int16_t)p_Temp[1] >> 8;
           rtU.In1	= (int16_t)(ads1292_Cache[6]<<8 | ads1292_Cache[7]);//p_Temp[1] >> 8;
                Subsystem_step();

                


				
				
    status_byte  = (status_byte & 0x0f8000) >> 15;  // bit15 gives the lead status
    
    LeadStatus = (unsigned char ) status_byte ;  

    if(!((LeadStatus & 0x1c) == 0 ))
      leadoff_deteted  = true; 
    else
      leadoff_deteted  = false;
				
				
				
			
				
				
				
                cannle[1]=(int32_t)rtY.Out1;
                data_rdc_in = (int32_t)rtY.Out1;
				
				if(leadoff_deteted)
				{
					data_rdc_in = 0;
					printf("t5.txt=\"---\"\xff\xff\xff");
					//printf("\"\xff\xff\xff");
				}
	
	

				
				
				//����ֱ������
				
			//	data_part[0] = data_rdc_in;

					
				data_part[0] = data_rdc_in;

				for ( i = (DATA_SIZE-1); i > 0; i--)
				{
					data_part[i] = data_part[i - 1];
					Mac += data_part[i];
				}

				line_part=Mac/DATA_SIZE;
				
				
				
				
				 preReadData = readData;	
				readData = (data_rdc_in-line_part)/10;
			 if((readData - preReadData) < filter) 
				  data[idx++] = readData;	


        if(idx >= DATA_SIZE)
        {
            idx = 0;	// ������������ͷ����

            // ͨ�����������ȡ�����źŵĲ��塢����ֵ���������м�ֵ��Ϊ�ж��ο���ֵ
            max = Get_Array_Max(data, DATA_SIZE);
            min = Get_Array_Min(data, DATA_SIZE);
            mid = (max + min)* 8/10;
			//mid = (max + mid) *8/10;
            filter = (max - min) / 2;
        }

        PRE_PULSE = PULSE;	// ���浱ǰ����״̬
        PULSE = (readData > mid) ? TRUE : FALSE;  // ����ֵ�����м�ֵΪ��Ч����

        if(PRE_PULSE == FALSE && PULSE == TRUE)   // Ѱ�ҵ����ź�����������м�λ�á��������㣬��⵽һ����Ч����
        {
            pulseCount++;
            pulseCount %= 2;

            if(pulseCount == 1) // ���������ĵ�һ��
            {
                firstTimeCount = timeCount;   // ��¼��һ������ʱ��
            }
            if(pulseCount == 0)  // ���������ĵڶ���
            {
                secondTimeCount = timeCount;  // ��¼�ڶ�������ʱ��
                timeCount = 0;

                if((secondTimeCount > firstTimeCount))
                {
                    IBI = (secondTimeCount - firstTimeCount) * SAMPLE_PERIOD;	// ������������������ʱ�䣬�õ� IBI
                    BPM = 60000 / IBI;  // ͨ�� IBI �õ�����ֵ BPM
                    if(BPM > 200)     //����BPM�����ʾֵ
                        BPM = 200;
                    if(BPM < 30)     //����BPM�����ʾֵ
                        BPM = 30;
                }
            }

		
			printf("t5.txt=\"%d\"\xff\xff\xff",(int)BPM);
		
		
        }

        timeCount++;  // ʱ������ۼ�
       Mac = 0;
			
		



			printf("add 1,0,%d\xff\xff\xff",(data_rdc_in - line_part)/10);
         

                ads1292_recive_flag=0;
   
				
                
                count++;

				
            }
			
			
			
			if(Uart2_Sta)
			{
				StepCount = USART2_RX_BUF[1]<<8|USART2_RX_BUF[2];
				Distance = USART2_RX_BUF[3]<<8|USART2_RX_BUF[4];
				for(i=0;i<UART_LENGTH;++i)
				USART2_RX_BUF[i] = 0;
				Uart2_Rx = 0;
				Uart2_Tx = 0;
				Uart2_Sta=0;
			}
			
			
if(count % 250 == 0){
		   temp_count++;
           adcx=Get_Adc(ADC_Channel_1);
           
           temp=(double)adcx*(3.3/4096)*1000;
			
           tem =(double)((-1.809628e-9)*temp*temp*temp+(-3.325395e-6)*temp*temp+(-1.814103e-1)*temp+205.5894);
           sum_T+=tem;
if(temp_count>6){
	sum_T=sum_T/7;
	printf("t2.txt=\"%2.2f\"\xff\xff\xff",sum_T);

				   temp_count=0;
				   sum_T=0;
				   j=0;
	      
}
        
			
 			
				
				
		
				
if(count % 500 == 0){
			
				printf("t8.txt=\"%d\"\xff\xff\xff",StepCount);
			
}
if(count % 750 == 0){					
printf("t9.txt=\"%d\"\xff\xff\xff",Distance);
count=0;	
}
           }
			
		   	PAout(7) = 0;	

        }
		

    }
}



/*���ܣ��Ѳɵ���3���ֽ�ת���з���32λ�� */
s32 get_volt(u32 num)
{
    s32 temp;
    temp = num;
    temp <<= 8;
    temp >>= 8;
    return temp;
}




/**********************************************************************
����������ļ������ݵ����壺
Code����ʾ������ռ�� FLASH �Ĵ�С��FLASH��
RO-data���� Read Only-data�� ��ʾ������ĳ������� const ���ͣ�FLASH��
RW-data���� Read Write-data�� ��ʾ�ѱ���ʼ����ȫ�ֱ�����SRAM��
ZI-data���� Zero Init-data�� ��ʾδ����ʼ����ȫ�ֱ���(SRAM)
***********************************************************************/

void HMISendb(u8 k)		         //�ֽڷ��ͺ���
{
    u8 i;
    for(i=0; i<3; i++)
    {
        printf("%c",k);
    }
}