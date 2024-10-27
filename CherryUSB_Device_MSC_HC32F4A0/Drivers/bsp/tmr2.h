#ifndef _TMR2_H__
#define _TMR2_H__

#define TMR2_UNIT                       (CM_TMR2_4)//Timer2��Ԫ1
#define TMR2_CH                         (TMR2_CH_A)//ͨ��A
#define TMR2_PERIPH_CLK                 (FCG2_PERIPH_TMR2_4)//Timer2ʱ��

//���������ã�CNT������CMP��Ҫ1us���ɲο��ֲ�25.3.2 �Ƚ���� ���Կ���������Ϊ2us�����ڴ˶�ʱ��ֻ��һ����׼��ʱ������˲�������PWM+DMA+GPIO
#define TMR2_CLK_SRC                    (TMR2_CLK_PCLK1)//ѡ��ʱ��ԴΪPCLK1,�˹�����PCLK1=120MHZ
#define TMR2_CLK_DIV                    (TMR2_CLK_DIV8)//8��Ƶ 15MHZ
#define TMR2_CMP_VAL                    (15UL - 1U)//��׼ֵ�Ĵ������൱��STM32�е�ARR��CNT�ﵽ��ֵ�ͻ���װ��

/*
����ѡ��PC4����Ϊ����PCB���ߣ�HUB75����PE2~PE15��14��IO��ѡ13����Ȼ���ټ�һ��PC14
��Ȼ��֮ǰ��ʱ��ͬ������Ӳ�������ܸ��ã���������Ʒ��ʱ��
*/
#define TMR2_PWM_PORT                   (GPIO_PORT_C)
#define TMR2_PWM_PIN                    (GPIO_PIN_04)
#define TMR2_PWM_PIN_FUNC               (GPIO_FUNC_16)//Ϊʲô��GPIO_FINC_16�Լ�Ϊʲô��PA2,ȥ�������ֲ��е����Ź��ܱ�(2.2 Pin function table)


void Tmr2Config(void);

#endif

