#include "tmr0.h"
#include "hc32_ll_interrupts.h"
#include "hc32_ll_fcg.h"
#include "delay_systick.h"
#include "hc32f4xx.h"

/*
����Tmr0 ��unit2 �� chB ʵ�ּ�ʱ�������ܣ�������������һ�κ���ִ�е�ʱ��
֮����ѡ��Tmr0������Ϊ�书�����١�

��������ʱ����ܲ�����׼�����ǿ����ṩ�ο����Կ�����˵�Ƿǳ��а����ģ�����������������Ļ��ʱ��
����һ�����sin�����ʱ������һ��Ӳ��sin�����ʱ�ȵȡ�

Tmr0 ��ʱ����ԴΪ�ڲ�PCLK1�����120MHZ
Tmr0Unit2CHB_Init(TMR0_CLK_DIV1);���벻ͬ�ķ�Ƶ���������ɻ�ò�ͬ����С��ʱ��λ��û�п����жϣ���
����ʱ65535*��С��ʱ��λ��

1��Ƶʱ����С��ʱ��λΪ 1/120 ΢�룬����ʱ65535*1/120=546.125us
2��Ƶʱ����С��ʱ��λΪ 1/60 ΢��
4��Ƶʱ����С��ʱ��λΪ 1/30 ΢��
8��Ƶʱ����С��ʱ��λΪ 1/15 ΢��
16��Ƶʱ����С��ʱ��λΪ 1/7.5 ΢��
32��Ƶʱ����С��ʱ��λ 1/3.75 ΢��
64��Ƶʱ����С��ʱ��λ 1/1.875 ΢��
128��Ƶʱ����С��ʱ��λ 1/0.9375 ΢��
256��Ƶʱ����С��ʱ��λ 1/0.46875 ΢��
512��Ƶʱ����С��ʱ��λ 1/0.234375 ΢��
1024��Ƶʱ����С��ʱ��λ 1/0.1171875 ΢��

��ѡ��ķ�Ƶϵ���� 2��4��8��16��32��64��128��256��512��1024

����Tmr0 Unit2 CHB��˵����ʱ�����͹ر����ɼĴ���CM_TMR0_2->BCONR��CSTB��Bit16��λ�����Ƶ�
����Tmr0 Unit2 CHA��˵����ʱ�����͹ر����ɼĴ���CM_TMR0_2->BCONR��CSTA��Bit0�� λ�����Ƶ�

�����������������ʱ�ͽ�����ʱ�Ͳ�����ll�⺯����ֱ���Լ������Ĵ���,�����װ��������
CountTimer_Start()
CountTimer_Stop()

ʹ��ʵ����
	TMR0_UNIT->CNTBR=0;
	CountTimer_Start();
	xxx
	CountTimer_Stop();
	count_i = TMR0_UNIT->CNTBR;
*/


//#include "hc32_ll_gpio.h"

//static void TMR0_CompareIrqCallback(void){
//	GPIO_TogglePins(GPIO_PORT_E,GPIO_PIN_02);
//    TMR0_ClearStatus(TMR0_UNIT, TMR0_INT_FLAG);
//}

/*
TMR0_CLK_DIV1024
TMR0_CLK_DIV512
TMR0_CLK_DIV256
TMR0_CLK_DIV128
TMR0_CLK_DIV64
TMR0_CLK_DIV32
TMR0_CLK_DIV16
TMR0_CLK_DIV8
TMR0_CLK_DIV4
TMR0_CLK_DIV2
TMR0_CLK_DIV1
*/
void CountTimer_Tmr0Unit2CHB_Init(unsigned int DIV){
    stc_tmr0_init_t stcTmr0Init;
//    stc_irq_signin_config_t stcIrqSignConfig;

    /* Enable timer0 and AOS clock */
    FCG_Fcg2PeriphClockCmd(TMR0_CLK, ENABLE);
//    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);

    /* TIMER0 configuration */
    (void)TMR0_StructInit(&stcTmr0Init);
    stcTmr0Init.u32ClockSrc     = TMR0_CLK_SRC;
    stcTmr0Init.u32ClockDiv     = DIV;
    stcTmr0Init.u32Func         = TMR0_FUNC_CMP;
    stcTmr0Init.u16CompareValue = (uint16_t)TMR0_CMP_VALUE;
    (void)TMR0_Init(TMR0_UNIT, TMR0_CH, &stcTmr0Init);

//    TMR0_IntCmd(TMR0_UNIT, TMR0_INT_TYPE, ENABLE);//ʹ���ж�

//    /* Interrupt configuration */
//    stcIrqSignConfig.enIntSrc    = TMR0_INT_SRC;
//    stcIrqSignConfig.enIRQn      = TMR0_INT_IRQn;
//    stcIrqSignConfig.pfnCallback = &TMR0_CompareIrqCallback;
//    (void)INTC_IrqSignIn(&stcIrqSignConfig);
//    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
//    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
//    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);

	CountTimer_Stop();
	TMR0_UNIT->CNTBR=0;
}

void Tmr0Uint1TriggeADC(void){
    stc_tmr0_init_t stcTmr0Init;

    /* Enable timer0 and AOS clock */
    FCG_Fcg2PeriphClockCmd(ADC_TMR0_CLK, ENABLE);

    /* TIMER0 configuration */
    (void)TMR0_StructInit(&stcTmr0Init);
    stcTmr0Init.u32ClockSrc     = ADC_TMR0_CLK_SRC;
    stcTmr0Init.u32ClockDiv     = ADC_TMR0_CLK_DIV;
    stcTmr0Init.u32Func         = TMR0_FUNC_CMP;
    stcTmr0Init.u16CompareValue = (uint16_t)ADC_TMR0_CMP_VALUE;
    (void)TMR0_Init(ADC_TMR0_UNIT, ADC_TMR0_CH, &stcTmr0Init);

}






