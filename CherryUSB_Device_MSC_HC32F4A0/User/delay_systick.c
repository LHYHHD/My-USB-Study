/*
systick��ʱ�����ܣ�
systick���������üĴ����ֱ���
CTRL	:���ƺ�״̬�Ĵ���(ÿһλ������ȥ�ٶȣ�ͨ�õ�)
LOAD	:��װ�ؼĴ���
VAL		:��ǰֵ�Ĵ���
systick��һ��24λ�ĵ���ʱ���ݼ����Ĵ�������������0��VAL = 0��ʱ������
LOAD������װ�س�ʼֵ���������жϣ�����������ֻҪ���ѿ��ƺ�״̬�Ĵ����е�
ʹ��λ������ͻ�����ͣϢ�ļ�����ȥ��

������������������ʱ��������systick��ʱ�����ж�ʵ�ֵģ�1ms����һ���ж�
���CPU ���ĺܴ󣬾���û����ʱ ʱ�Ѷ�ʱ���رգ�Ҳ�����CPU��Դ�˷ѣ���Ϊ 
����ʱʱ��CPU��һֱ����systick�����жϣ����������ַ������ɱ���ġ�

�����Ƿ�������ԭ�ӵģ�����systickʵ�ֵ���ʱ

Ҫʵ���жϹ��ܿ��òο������ĺ���
*/
#include "delay_systick.h"



static uint8_t  fac_us=0;							//us��ʱ������			   
static uint32_t fac_ms=0;							//ms��ʱ������

/*
�������ֲ��֪��systick��ʱ��Ϊ24λ���¼�����ʱ��

�˺���ʵ������systick��ʱ������Ƶ��Ϊϵͳ��ʱ��/8
��
	ϵͳ��ʱ��Ϊ240MHZ����ôsystick��Ƶ��Ϊ30MHZ
	��ôSysTick->CNT��30����Ҫ��ʱ��Ϊ1us
��
ͬʱ��ʼ��us��ms��ʱ�ı�����

param:ϵͳ��Ƶ����λΪMHZ��

*/
void delay_init(uint8_t SYS_CLK){
	SysTick->CTRL |= 1<<2|0;  //����ʱ����Դ�����ж�
	fac_us =  SYS_CLK/1;//����Ҫ���Ǹ����systick��ʱ��Ƶ�ʣ�������HCLK
	fac_ms = fac_us * 1000;
}


/*
us ��ʱ����
param
	n:��ʱ n us
	(�������ֵΪ 2^24 / (SYS_CLK) �� 16777216 / (240) = 69,905 )
�����ʱ69,905us
*/
void delay_us(uint32_t n)
{
	uint32_t temp;	    	 
	SysTick->LOAD=n*fac_us; 									//ʱ�����	  		 
	SysTick->VAL=0x00;        								//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;//֮��������һ������������Ϊ�����������жϣ��������������᲻�Ͻ�
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
	SysTick->VAL =0X00;       				//��ռ����� 
}


/*
ms ��ʱ����
param
	n:��ʱ n ms
	(�������ֵΪ 2^24 / ( (SYS_CLK)*1000 ) �� 16777216 / ( (240) * 1000)= 69 )
�����ʱ69ms
*/
void delay_xms(uint16_t n)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=n*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           			//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;     		  		//��ռ�����	  	    
} 


/*
ms ��ʱ����
param
	n:��ʱ n ms
	(�������ֵΪ65535)
�����ʱ65535ms
*/
void delay_ms(uint16_t n)
{	 	 
	/* ��������д���Ƿǳ������ */
	uint8_t repeat=n/69;																
	uint16_t remain=n%69;
	
	while(repeat)
	{
		delay_xms(69);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 


