#ifndef _TMR0_H__
#define _TMR0_H__

#include "hc32_ll_tmr0.h"


#define TMR0_UNIT                       (CM_TMR0_2)
#define TMR0_CLK_SRC                    (TMR0_CLK_SRC_INTERN_CLK)
//#define TMR0_CLK_DIV                    (TMR0_CLK_DIV8)
#define TMR0_CLK                        (FCG2_PERIPH_TMR0_2)
#define TMR0_CH                         (TMR0_CH_B)
#define TMR0_CMP_VALUE                  (65536-1)

//#define TMR0_INT_TYPE                   (TMR0_INT_CMP_B)//TMR0 ��������жϣ�CNT�����ڼĴ�����ֵTMRA_PERIOD_VAL���ʱ����
//#define TMR0_INT_PRIO                   (DDL_IRQ_PRIO_00)//�ж����ȼ�
//#define TMR0_INT_SRC                    (INT_SRC_TMR0_2_CMP_B)//hc32f4a0.h�е�en_int_src_t��ѡȡ���ؼ��㣺��ʱ��0��2��Ԫ��ͨ��B��CMP��
//#define TMR0_INT_IRQn                   (INT050_IRQn)//hc32f4a0.h�е�IRQn_Type��ѡȡ��ѡȡ���ݣ��ο��ֲ�10.INTC->10.3.����������
//#define TMR0_INT_FLAG                   (TMR0_FLAG_CMP_B)//TMR0��ͨ��B��CMP״̬��־


#define CountTimer_Start()	TMR0_UNIT->BCONR |= 0x00010000
#define CountTimer_Stop()	TMR0_UNIT->BCONR &= 0xFFFEFFFF


void CountTimer_Tmr0Unit2CHB_Init(unsigned int unit);



#define ADC_TMR0_UNIT                       (CM_TMR0_1)
#define ADC_TMR0_CLK_SRC                    (TMR0_CLK_SRC_INTERN_CLK)
#define ADC_TMR0_CLK_DIV                    (TMR0_CLK_DIV2)//60mhz
#define ADC_TMR0_CLK                        (FCG2_PERIPH_TMR0_1)
#define ADC_TMR0_CH                         (TMR0_CH_A)
#define ADC_TMR0_CMP_VALUE                  (1250-1)//48k������

void Tmr0Uint1TriggeADC(void);

#endif
