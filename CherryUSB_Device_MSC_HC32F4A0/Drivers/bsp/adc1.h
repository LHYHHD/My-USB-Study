#ifndef _ADC1_H__
#define _ADC1_H__

#include "hc32_ll_adc.h"

#define PERIPHCLK_CLK_SCR               (CLK_PERIPHCLK_PCLK)//外设时钟源，这里的外设指ADC/DAC/TRNG/I2S

#define ADC_UNIT                        (CM_ADC1)
#define ADC_PERIPH_CLK                  (FCG3_PERIPH_ADC1)//ADC1时钟
#define ADC_CHL                         (ADC_CH0)
#define ADC_CHL_PORT                    (GPIO_PORT_A)
#define ADC_CHL_PIN                     (GPIO_PIN_00)//从数据手册Pin function table或者XHSC XHCode软件获取

#define ADC_CHR                         (ADC_CH2)
#define ADC_CHR_PORT                    (GPIO_PORT_A)
#define ADC_CHR_PIN                     (GPIO_PIN_02)//从数据手册Pin function table或者XHSC XHCode软件获取

#define ADC_SEQ                         (ADC_SEQ_A)//ADC1的序列A
#define ADC_EOC_FLAG                    (ADC_FLAG_EOCA)


#define SH_PERIPH_CLK                   (FCG3_PERIPH_CMBIAS)//采样保持时钟


void ADC1_CH0_CH2_Init(void);

#endif

