#ifndef _TMRA_H__
#define _TMRA_H__

#define TMRA_UNIT                       (CM_TMRA_11)//定时器A单元11
#define TMR_COUNT_SRC					(TMRA_CNT_SRC_SW)//计数时钟源，为PCLK或者其他,这里为PCLK
/*本系列产品搭载的 12 个单元的 TimerA，其中单元 1~单元 4 的总线时钟和计数时钟均为 PCLK0
（此时，框图中所示 PCLK 是指 PCLK0）；单元 5~单元 12 的总线时钟和计数时钟为 PCLK1（此时，
框图中所示 PCLK 是指 PCLK1）。*/

#define TMRA_PERIPH_CLK                 (FCG2_PERIPH_TMRA_11)//外设时钟
#define TMRA_CLK_DIV                    (TMRA_CLK_DIV1)//分频系数
#define TMRA_MD                         (TMRA_MD_SAWTOOTH)//锯齿波模式，参考手册24.3.1 波形模式
#define TMRA_DIR						TMRA_DIR_UP//计数方向
#define TMRA_PERIOD_VAL					10-1//周期寄存器（重装载）

//#define TMRA_INT_TYPE                   (TMRA_INT_OVF)//TMRA 计数溢出中断，CNT与周期寄存器的值TMRA_PERIOD_VAL相等时产生
//#define TMRA_INT_PRIO                   (DDL_IRQ_PRIO_03)//中断优先级
//#define TMRA_INT_SRC                    (INT_SRC_TMRA_11_OVF)//hc32f4a0.h中的en_int_src_t中选取（关键点：定时器2的1单元的通道A的CMP）
//#define TMRA_INT_IRQn                   (INT098_IRQn)//hc32f4a0.h中的IRQn_Type中选取（选取依据：参考手册10.INTC->10.3.外设向量表）
//#define TMRA_INT_FLAG                   (TMRA_FLAG_OVF)//TMRA的OVF状态标志

/*
选PC6是为了方便走线，PC6以及FUNC_5怎么来的，查看数据手册Pin function table
*/
#define TMRA_PWM_PORT                   (GPIO_PORT_C)
#define TMRA_PWM_PIN                    (GPIO_PIN_06)
#define TMRA_PWM_PIN_FUNC               (GPIO_FUNC_5)


#define TMRA_PWM_CMP_VAL                (5 - 1U)
#define TMRA_PWM_CH                     (TMRA_CH4)//由PC6决定


void TmrAConfig(void);

#endif
