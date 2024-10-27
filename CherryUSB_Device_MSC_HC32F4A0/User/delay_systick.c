/*
systick定时器介绍：
systick有三个常用寄存器分别是
CTRL	:控制和状态寄存器(每一位的作用去百度，通用的)
LOAD	:重装载寄存器
VAL		:当前值寄存器
systick是一个24位的倒计时（递减）寄存器，当计数到0（VAL = 0）时，将从
LOAD里面重装载初始值，并产生中断，继续计数。只要不把控制和状态寄存器中的
使能位清除，就会永不停息的计数下去。

立创给的资料里面延时函数是用systick定时器的中断实现的，1ms产生一次中断
这对CPU 消耗很大，就算没有延时 时把定时器关闭，也会造成CPU资源浪费，因为 
在延时时，CPU会一直进入systick更新中断，这是用这种方法不可避免的。

下面是仿照正点原子的，利用systick实现的延时

要实现中断功能可用参考立创的函数
*/
#include "delay_systick.h"



static uint8_t  fac_us=0;							//us延时倍乘数			   
static uint32_t fac_ms=0;							//ms延时倍乘数

/*
由数据手册可知，systick定时器为24位向下计数定时器

此函数实现设置systick定时器运行频率为系统主时钟/8
（
	系统主时钟为240MHZ，那么systick的频率为30MHZ
	那么SysTick->CNT减30所需要的时间为1us
）
同时初始化us和ms延时的倍乘数

param:系统主频（单位为MHZ）

*/
void delay_init(uint8_t SYS_CLK){
	SysTick->CTRL |= 1<<2|0;  //设置时钟来源、关中断
	fac_us =  SYS_CLK/1;//最重要的是搞清楚systick的时钟频率，这里是HCLK
	fac_ms = fac_us * 1000;
}


/*
us 延时函数
param
	n:延时 n us
	(传入最大值为 2^24 / (SYS_CLK) 即 16777216 / (240) = 69,905 )
最大延时69,905us
*/
void delay_us(uint32_t n)
{
	uint32_t temp;	    	 
	SysTick->LOAD=n*fac_us; 									//时间加载	  		 
	SysTick->VAL=0x00;        								//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//开始倒数 	 
	do
	{
		temp=SysTick->CTRL;//之所以引入一个变量，是因为下面有两处判断，如果不引入变量会不严谨
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
	SysTick->VAL =0X00;       				//清空计数器 
}


/*
ms 延时函数
param
	n:延时 n ms
	(传入最大值为 2^24 / ( (SYS_CLK)*1000 ) 即 16777216 / ( (240) * 1000)= 69 )
最大延时69ms
*/
void delay_xms(uint16_t n)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=n*fac_ms;			//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           			//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;     		  		//清空计数器	  	    
} 


/*
ms 延时函数
param
	n:延时 n ms
	(传入最大值为65535)
最大延时65535ms
*/
void delay_ms(uint16_t n)
{	 	 
	/* 下面这种写法是非常经典的 */
	uint8_t repeat=n/69;																
	uint16_t remain=n%69;
	
	while(repeat)
	{
		delay_xms(69);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 


