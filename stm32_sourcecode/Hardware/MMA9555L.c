/******************************************************************** 
*  �� �� ��: dvMMA9555L.c 
* 
*  ������: ��������������
*  �� �� ��: 
*  ����ʱ��: 2014��01��22��
*  ˵    ��: 
*------�޸���ʷ----------------------------------------- 
* �ļ���	: main.c
* ��  ��	: GGTEAM
* ��  ��	: V1.0
* ��  ��	: 2020-10-07
* ��  ��	:
* 
******************************************************************/ 

#include "MMA9555L.h"
#include "IIC.h"  
#include "usart.h"
#include "delay.h"





/******************************************************************** 
* ��������: pedometer_main
* ��������: �ǲ���������
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_main(void)
{
   unsigned char Buf[20];
   u8 xyz;
   int StepCount,Distance; //wenxue
   pedometer_cmd_readstatus(); // read  // д��{0x15,0x30,0x00,0x0C}
	
     MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 2);
     if(Buf[1]==0x80)
      {
        MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 16);  
		StepCount = Buf[6] * 256 + Buf[7];
		Distance  = Buf[8] * 256 + Buf[9];
		printf("t8.txt=\"%d\"\xff\xff\xff",StepCount);
		printf("t11.txt=\"%d\"\xff\xff\xff",Distance);
       }
 //   }
    
    // wenxue      
      //  for(int i=0;i<16;i++)
         // printf("Buf[%d]=%02x\r\n",i,Buf[i]);       

       //m_status.Distance  = Buf[8] * 256 + Buf[9];
       //m_status.Calories  = Buf[12] * 256 + Buf[13];
}

/******************************************************************** 
* ��������: pedometer_clear
* ��������: �ǲ������
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: ���Ӧ�ó�����ⲿ״̬�������ڲ����±������䡣�Ʋ�������
*           Ҫ������ܣ���˷���һ����ָ������Ӧ��Ӧ�ó�����
* ע    ��:  
********************************************************************/
void pedometer_clear(void)
{
//    m_status.StepCount = 0;
//    m_status.Progress = 0;
//    m_status.Calories = 13;
}

/******************************************************************** 
* ��������: pedometer_init
* ��������: ��ʼ������
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: ��λ��ִ��һ�Ρ�������������Ӧ�ó��������������Ժ�Ҫ��
*           ��̬�ڴ���䡣
* ע    ��:  
********************************************************************/
void pedometer_init(void)
{
  unsigned char Buf[20];  
  
  int StepCount; //wenxue
   int i;
    //pedometer_reset();          // reset pedometer
    pedometer_write_config();       // config
    pedometer_enable();         // enable pedometer
    pedometer_int0_enable();    // enable INT_O pin
    pedometer_active();         // active MMA9553
    pedometer_wakeup();
    

    pedometer_afe_config(); // afe config    wenxue
   // delay_wwx();
    SixDirection_Init();    
    pedometer_cmd_readstatus();
        
    while(1)  
    {
       MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 2);
       printf("Buf[1]==%02x\r\n",Buf[1]);// wenxue
       if(Buf[1]==0x80)
       {
         MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 16); // 12 bytes status data + 4 bytes frame start 
         break; // wenxue
    
        }
     }        
      // wenxue      
      for( i=0;i<16;i++)
         printf("Buf[%d]=%02x\r\n",i,Buf[i]);
        
      StepCount = Buf[6] * 256 + Buf[7];
      printf("StepCount=%d\r\n",StepCount);
    
}

/******************************************************************** 
* ��������: pedometer_cmd_readstatus
* ��������: �ǲ�����״̬����
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_cmd_readstatus(void)
{
    unsigned char Buf[]={0x15,0x30,0x00,0x0C};
    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
}

/******************************************************************** 
* ��������: pedometer_cmd_readconfig
* ��������: �ǲ���������
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_cmd_readconfig(void)
{
    unsigned char Buf[]={0x15,0x10,0x00,0x10};

    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
}

void pedometer_cmd_readwakeup(void)
{
    unsigned char Buf[]={0x12,0x10,0x06,0x01};
    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
}
/******************************************************************** 
* ��������: pedometer_write_config
* ��������: ���ò���
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_write_config(void)
{
    unsigned char Buf[]={0x15,0x20,0x00,0x10,
                           0x0C,0xE0,
                           0x13,0x20,
                           0x00,0x96,
                           0x60,0x50,
                           0xAF,0x50,
                           0x04,0x03,
                           0x05,0x01,
                           0x00,0x00};

    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 20);
}


/******************************************************************** 
* ��������: pedometer_reset
* ��������: ���ú���
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_reset(void)
{
   unsigned char Buf[]={0x17,0x20,0x01,0x01,0x20};
    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/******************************************************************** 
* ��������: pedometer_enable
* ��������: �ǲ����ܿ���
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_enable(void)
{
    unsigned char Buf[]={0x17,0x20,0x05,0x01,0x00};

    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/******************************************************************** 
* ��������: pedometer_disable
* ��������: �ǲ����ܹر�
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_disable(void)
{
   unsigned char Buf[]={0x17,0x20,0x05,0x01,0x20};
   MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/******************************************************************** 
* ��������: pedometer_active
* ��������: ����
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_active(void)
{
    unsigned char Buf[]={0x15,0x20,0x06,0x01,0x80};
    
    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/******************************************************************** 
* ��������: pedometer_wakeup
* ��������: ����
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_wakeup(void)
{
    unsigned char Buf[]={0x12,0x20,0x06,0x01,0x00};
    
    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/******************************************************************** 
* ��������: pedometer_int0_enable
* ��������: �ж�0����
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_int0_enable(void)
{
    unsigned char Buf[]={0x18,0x20,0x00,0x01,0xC0};

    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}


/********************************************************************
* ��������  :   MMA9555L_Read                 
* ��������  :   ��gsensor
* �������  :   deviceAddr��I2C������ַ
*               regAddr���Ĵ�����ַ
* �������  :   pdata����ȡֵ
* ����ֵ    :   ��
* ����ģ��  :   I2C����������
* ����˵��  :
********************************************************************/
void MMA9555L_Read(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len)
{
    
 // I2c_ReadRegister(I2C0_BASE_PTR,deviceAddr,regAddr,data,len);

  	I2c_ReadRegister(deviceAddr,regAddr,data,len); // wenxue
}

/********************************************************************
* ��������  :   MMA9555L_Write                 
* ��������  :   ���ֽ�д�����ַI2C����
* �������  :   deviceAddr��I2C������ַ
*               regAddr���Ĵ�����ַ
*               data������ָ��
*               len,д�볤��
* �������  :   ��
* ����ֵ    :   ��
* ����ģ��  :   
* ����˵��  :
********************************************************************/
void MMA9555L_Write(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len)
{
   //I2c_WriteRegister(I2C0_BASE_PTR,deviceAddr,regAddr,data,len);
  I2c_WriteRegister(deviceAddr,regAddr,data,len);  // wenxue
  
}





// wenxue 
/********************************************************************
* ��������  :   pedometer_cmd__readlpfxyz                 
* ��������  :   ��xyz �����һ����Ϣ
* �������  :   deviceAddr��I2C������ַ
*               regAddr���Ĵ�����ַ
*               data������ָ��
*               len,д�볤��
* �������  :   ��
* ����ֵ    :   ��
* ����ģ��  :   
* ����˵��  :
********************************************************************/
void pedometer_cmd__readlpfxyz(void)
{
    unsigned char Buf[]={0x06,0x30,0x18,0x06};

    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
  
}
  


/********************************************************************
* ��������  :   pedometer_cmd__readrawxyz                 
* ��������  :   ��xyz raw ��Ϣ
* �������  :   deviceAddr��I2C������ַ
*               regAddr���Ĵ�����ַ
*               data������ָ��
*               len,д�볤��
* �������  :   ��
* ����ֵ    :   ��
* ����ģ��  :   
* ����˵��  :
********************************************************************/
void pedometer_cmd__readrawxyz(void)
{
    unsigned char Buf[]={0x06,0x30,0x12,0x06};

    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
  
}



/******************************************************************** 
* ��������: pedometer_mainrawxyz
* ��������: �ǲ���������
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: ѡ�����¼�ÿ�η�����ִ�е���Ҫ���ܡ��ڼƲ���������£�
*           �ù���ִ�л�ȡÿ���µļ��ٶȼ���Ʒ�󲢵������Ʋ�������
*           ��������Ʒ��
* ע    ��:  
********************************************************************/
void pedometer_mainrawxyz(void)
{
   unsigned char Buf[20];
   short x, y, z,i; 
   
   pedometer_cmd__readrawxyz(); //
        
        while(1)
        {
           MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 2);
           //printf("Buf[1]==%02x\r\n",Buf[1]);// wenxue

           if(Buf[1]==0x80)
           {
              MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 10);  
              break;
            }
       }
    
        // wenxue      
       for(i=0;i<10;i++)
         printf("Buf[%d]=%02x\r\n",i,Buf[i]);       
           
       x = Buf[4] * 256 + Buf[5];
       y = Buf[6] * 256 + Buf[7];
       z = Buf[8] * 256 + Buf[9];
       
       printf("x=%d\r\n",x);
       printf("y=%d\r\n",y);
       printf("z=%d\r\n",z);
}




/******************************************************************** 
* ��������: pedometer_afe_config
* ��������: Configure the AFE range
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_afe_config(void)
{
  // unsigned char Buf[]={0x06,0x20,0x00,0x01,0x40}; // 2g mode FS=01    1g--16393
   //unsigned char Buf[]={0x06,0x20,0x00,0x01,0x80}; // 4g mode FS=10    4g--8196
   unsigned char Buf[]={0x06,0x20,0x00,0x01,0x00}; // 8g mode FS=00    8g--4098
   
    MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}





/******************************************************************** 
* ��������: pedometer_afe_config_read
* ��������: Read Config data
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void pedometer_afe_config_read(void)
{
  
   unsigned char Buf[]={0x06,0x10,0x00,0x01}; 

   MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf,4);
}

/******************************************************************** 
* ��������: SixDirection_Init
* ��������: 
* �������:  
* �� �� ֵ:  
* ȫ�ֱ���:  
* ����ģ��:  
* ˵    ��: 
* ע    ��:  
********************************************************************/
void SixDirection_Init(void)
{
   unsigned char Buf[]={0x1A,0x20,0x00,0x02,0x04,0x01};		  //�����񶯴����ж�����ı�
    unsigned char Buf1[]={0x17,0x20,0x04,0x01,0xFB};		  //ʹ��
   MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 6);
   
   MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf1, 5);
}

void SixDirection_Statues(u8 *data)
{
	unsigned char Buf[]={0x1A,0x30,0x00,0x02};
   MMA9555L_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4);

   MMA9555L_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, data, 2); 
   

}

/******************************************************************** 
*          �ļ����� 
********************************************************************/


