#ifndef _ADC2_H__
#define _ADC2_H__

#include "hc32_ll_adc.h"


#define ADC2_UNIT                        (CM_ADC2)
#define ADC2_PERIPH_CLK                  (FCG3_PERIPH_ADC2)//ADC2时钟
#define ADC2_CH                          (ADC_CH2)
#define ADC2_CH_PORT                     (GPIO_PORT_A)
#define ADC2_CH_PIN                      (GPIO_PIN_02)//从数据手册Pin function table或者XHSC XHCode软件获取

#define ADC2_SEQ                         (ADC_SEQ_A)//ADC2的序列A
#define ADC2_EOC_FLAG                    (ADC_FLAG_EOCA)


void ADC2_CH2_Init(void);

#endif

