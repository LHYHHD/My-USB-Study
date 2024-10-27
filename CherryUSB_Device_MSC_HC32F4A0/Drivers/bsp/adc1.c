#include "adc1.h"
#include "hc32_ll_clk.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_aos.h"
#include "hc32_ll_gpio.h"
#include "delay_systick.h"

static void AdcSetPinAnalogMode(void){
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(ADC_CHL_PORT, ADC_CHL_PIN, &stcGpioInit);
	(void)GPIO_Init(ADC_CHR_PORT, ADC_CHR_PIN, &stcGpioInit);
}

void ADC1_CH0_CH2_Init(void){
	
	stc_adc_init_t stcAdcInit;
	
	/*ʱ�ӳ�ʼ����ʱ������������õģ�PCLK2 60MHZ , PCLK4 120MHZ ,����PCLK2:PCLK4=1:2*/
	CLK_SetClockDiv((CLK_BUS_PCLK2 | CLK_BUS_PCLK4), (CLK_PCLK2_DIV4 | CLK_PCLK4_DIV2));
	/*��������ʱ��Դ�����������ָADC/DAC/TRNG/I2S*/
	CLK_SetPeriClockSrc(PERIPHCLK_CLK_SCR);//��PCLK2��PCLK4��ʱ��Դ����ϵͳʱ��,����֮�⻹��������PLLH��PLLA
	
    /* 1. Enable ADC peripheral clock. */
    FCG_Fcg3PeriphClockCmd(ADC_PERIPH_CLK, ENABLE);

    /* 2. Modify the default value depends on the application. Not needed here. */
    (void)ADC_StructInit(&stcAdcInit);
	stcAdcInit.u16ScanMode = ADC_MD_SEQA_SINGLESHOT;//����A����ɨ�裬����B����
	stcAdcInit.u16Resolution = ADC_RESOLUTION_12BIT;//ADC�ֱ���
	stcAdcInit.u16DataAlign = ADC_DATAALIGN_RIGHT;//�Ҷ���
    /* 3. Initializes ADC. */
    (void)ADC_Init(ADC_UNIT, &stcAdcInit);

    /* 4. ADC channel configuration. */
    /* 4.1 Set the ADC pin to analog input mode. */
    AdcSetPinAnalogMode();
    /* 4.2 Enable ADC channels. Call ADC_ChCmd() again to enable more channels if needed. */
    ADC_ChCmd(ADC_UNIT, ADC_SEQ, ADC_CHL, ENABLE);
	ADC_ChCmd(ADC_UNIT, ADC_SEQ, ADC_CHR, ENABLE);
    /* 5. Set sampling time if needed. */
    ADC_SetSampleTime(ADC_UNIT, ADC_CHL, 0x80U);//���ò���ʱ�䣬ת��ʱ���Ǹ���ADCλ���̶��ģ�ÿ��ͨ�������Ե������ò���ʱ��
	ADC_SetSampleTime(ADC_UNIT, ADC_CHR, 0x80U);//ת��ʱ��Ͳ���ʱ����������ͬ�Ķ��������忴�ֲ�
    /* 6. Conversion data average calculation function, if needed.
          Call ADC_ConvDataAverageChCmd() again to enable more average channels if needed. */
//    ADC_ConvDataAverageConfig(ADC_UNIT, ADC_AVG_CNT8);
//    ADC_ConvDataAverageChCmd(ADC_UNIT, ADC_CH, ENABLE);//����Ҫ��β�������ƽ��ֵ

	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);//��AOSʱ��
	ADC_TriggerConfig(ADC_UNIT, ADC_SEQ, ADC_HARDTRIG_EVT0);//ADC�������ã�ֻ��ѡADC_HARDTRIG_EVT0������ѡADC_HARDTRIG_EVT1�������ֲ�û�㶮Ϊʲô
	AOS_SetTriggerEventSrc(AOS_ADC1_0, EVT_SRC_TMR0_1_CMP_A);//AOS_ADC1_0����adc1 ͨ��0����˼
	ADC_TriggerCmd(ADC_UNIT, ADC_SEQ, ENABLE);

	/* �������� */
    /* 1. Enable sample-hold peripheral clock. */
    FCG_Fcg3PeriphClockCmd(SH_PERIPH_CLK, ENABLE);

    /* 2. Delay 2us is needed by sample-hold. */
    delay_us(5U);

    /**
     * 3. Specify the sampling time of sample-hold. The time should longer than 0.4us.
     *    In this example, PCLK2 is the clock of ADC analog circuit which is 8MHz.
     *    0.4us = (1 / 8) * 3.2.
	 *    ������pclk2=60mhz�����Դ�100��1/6*3.2=0.53us
     */
    ADC_SH_SetSampleTime(ADC_UNIT, 100U);
    /* 4. Enable sample hold channel. */
    ADC_SH_ChCmd(ADC_UNIT, ADC_CHL, ENABLE);
    ADC_SH_ChCmd(ADC_UNIT, ADC_CHR, ENABLE);

}




