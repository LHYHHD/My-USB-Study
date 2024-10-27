#ifndef _TMRA_H__
#define _TMRA_H__

#define TMRA_UNIT                       (CM_TMRA_11)//��ʱ��A��Ԫ11
#define TMR_COUNT_SRC					(TMRA_CNT_SRC_SW)//����ʱ��Դ��ΪPCLK��������,����ΪPCLK
/*��ϵ�в�Ʒ���ص� 12 ����Ԫ�� TimerA�����е�Ԫ 1~��Ԫ 4 ������ʱ�Ӻͼ���ʱ�Ӿ�Ϊ PCLK0
����ʱ����ͼ����ʾ PCLK ��ָ PCLK0������Ԫ 5~��Ԫ 12 ������ʱ�Ӻͼ���ʱ��Ϊ PCLK1����ʱ��
��ͼ����ʾ PCLK ��ָ PCLK1����*/

#define TMRA_PERIPH_CLK                 (FCG2_PERIPH_TMRA_11)//����ʱ��
#define TMRA_CLK_DIV                    (TMRA_CLK_DIV1)//��Ƶϵ��
#define TMRA_MD                         (TMRA_MD_SAWTOOTH)//��ݲ�ģʽ���ο��ֲ�24.3.1 ����ģʽ
#define TMRA_DIR						TMRA_DIR_UP//��������
#define TMRA_PERIOD_VAL					10-1//���ڼĴ�������װ�أ�

//#define TMRA_INT_TYPE                   (TMRA_INT_OVF)//TMRA ��������жϣ�CNT�����ڼĴ�����ֵTMRA_PERIOD_VAL���ʱ����
//#define TMRA_INT_PRIO                   (DDL_IRQ_PRIO_03)//�ж����ȼ�
//#define TMRA_INT_SRC                    (INT_SRC_TMRA_11_OVF)//hc32f4a0.h�е�en_int_src_t��ѡȡ���ؼ��㣺��ʱ��2��1��Ԫ��ͨ��A��CMP��
//#define TMRA_INT_IRQn                   (INT098_IRQn)//hc32f4a0.h�е�IRQn_Type��ѡȡ��ѡȡ���ݣ��ο��ֲ�10.INTC->10.3.����������
//#define TMRA_INT_FLAG                   (TMRA_FLAG_OVF)//TMRA��OVF״̬��־

/*
ѡPC6��Ϊ�˷������ߣ�PC6�Լ�FUNC_5��ô���ģ��鿴�����ֲ�Pin function table
*/
#define TMRA_PWM_PORT                   (GPIO_PORT_C)
#define TMRA_PWM_PIN                    (GPIO_PIN_06)
#define TMRA_PWM_PIN_FUNC               (GPIO_FUNC_5)


#define TMRA_PWM_CMP_VAL                (5 - 1U)
#define TMRA_PWM_CH                     (TMRA_CH4)//��PC6����


void TmrAConfig(void);

#endif
