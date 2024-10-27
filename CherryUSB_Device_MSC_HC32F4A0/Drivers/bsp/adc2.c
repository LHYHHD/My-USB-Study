#include "adc2.h"
#include "hc32_ll_clk.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_aos.h"
#include "hc32_ll_gpio.h"

#define PERIPHCLK_CLK_SCR               (CLK_PERIPHCLK_PCLK)//外设时钟源，这里的外设指ADC/DAC/TRNG/I2S


static void AdcSetPinAnalogMode(void){
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(ADC2_CH_PORT, ADC2_CH_PIN, &stcGpioInit);
}

void ADC2_CH2_Init(void){
	
	stc_adc_init_t stcAdcInit;
	
	/*时钟初始化的时候就是这样配置的，PCLK2 60MHZ , PCLK4 120MHZ ,满足PCLK2:PCLK4=1:2*/
	CLK_SetClockDiv((CLK_BUS_PCLK2 | CLK_BUS_PCLK4), (CLK_PCLK2_DIV4 | CLK_PCLK4_DIV2));
	/*设置外设时钟源，这里的外设指ADC/DAC/TRNG/I2S*/
	CLK_SetPeriClockSrc(PERIPHCLK_CLK_SCR);//让PCLK2、PCLK4的时钟源来自系统时钟,除此之外还可以来自PLLH、PLLA
	
    /* 1. Enable ADC peripheral clock. */
    FCG_Fcg3PeriphClockCmd(ADC2_PERIPH_CLK, ENABLE);

    /* 2. Modify the default value depends on the application. Not needed here. */
    (void)ADC_StructInit(&stcAdcInit);
	stcAdcInit.u16ScanMode = ADC_MD_SEQA_SINGLESHOT;//序列A单次扫描，序列B禁用
	stcAdcInit.u16Resolution = ADC_RESOLUTION_12BIT;//ADC分辨率
	stcAdcInit.u16DataAlign = ADC_DATAALIGN_RIGHT;//右对其
    /* 3. Initializes ADC. */
    (void)ADC_Init(ADC2_UNIT, &stcAdcInit);

    /* 4. ADC channel configuration. */
    /* 4.1 Set the ADC pin to analog input mode. */
    AdcSetPinAnalogMode();
    /* 4.2 Enable ADC channels. Call ADC_ChCmd() again to enable more channels if needed. */
    ADC_ChCmd(ADC2_UNIT, ADC2_SEQ, ADC2_CH, ENABLE);

    /* 5. Set sampling time if needed. */
    ADC_SetSampleTime(ADC2_UNIT, ADC2_CH, 0x40U);//设置采样时间，转换时间是根据ADC位数固定的，每个通道都可以单独设置采样时间

    /* 6. Conversion data average calculation function, if needed.
          Call ADC_ConvDataAverageChCmd() again to enable more average channels if needed. */
//    ADC_ConvDataAverageConfig(ADC_UNIT, ADC_AVG_CNT8);
//    ADC_ConvDataAverageChCmd(ADC_UNIT, ADC_CH, ENABLE);//不需要多次采样计算平均值

	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);//开AOS时钟
	ADC_TriggerConfig(ADC2_UNIT, ADC2_SEQ, ADC_HARDTRIG_EVT0);//ADC触发配置，只能选ADC_HARDTRIG_EVT0，不能选ADC_HARDTRIG_EVT1，看了手册没搞懂为什么
	AOS_SetTriggerEventSrc(AOS_ADC2_0, EVT_SRC_TMR0_1_CMP_A);//AOS_ADC2_0的意思不是adc2通道0的意思
	ADC_TriggerCmd(ADC2_UNIT, ADC2_SEQ, ENABLE);

	
}
