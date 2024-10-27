#ifndef _TMR2_H__
#define _TMR2_H__

#define TMR2_UNIT                       (CM_TMR2_4)//Timer2单元1
#define TMR2_CH                         (TMR2_CH_A)//通道A
#define TMR2_PERIPH_CLK                 (FCG2_PERIPH_TMR2_4)//Timer2时钟

//按如下配置，CNT计数到CMP需要1us，由参考手册25.3.2 比较输出 可以看出，周期为2us，由于此定时器只有一个基准定时器，因此不合适做PWM+DMA+GPIO
#define TMR2_CLK_SRC                    (TMR2_CLK_PCLK1)//选择时钟源为PCLK1,此工程中PCLK1=120MHZ
#define TMR2_CLK_DIV                    (TMR2_CLK_DIV8)//8分频 15MHZ
#define TMR2_CMP_VAL                    (15UL - 1U)//基准值寄存器，相当于STM32中的ARR，CNT达到此值就会重装载

/*
这里选择PC4是因为方便PCB走线，HUB75就在PE2~PE15这14个IO中选13个，然后再加一个PC14
虽然跟之前的时序不同，但是硬件上性能更好（考虑做成品板时）
*/
#define TMR2_PWM_PORT                   (GPIO_PORT_C)
#define TMR2_PWM_PIN                    (GPIO_PIN_04)
#define TMR2_PWM_PIN_FUNC               (GPIO_FUNC_16)//为什么是GPIO_FINC_16以及为什么是PA2,去看数据手册中的引脚功能表(2.2 Pin function table)


void Tmr2Config(void);

#endif

