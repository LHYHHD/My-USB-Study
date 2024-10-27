#ifndef _UART1_H__
#define _UART1_H__

#include "hc32f4xx_conf.h"



#if LL_USART_ENABLE == DDL_ON


/* USART RX/TX pin definition */
#define USART1_RX_PORT                   (GPIO_PORT_A)   /* PA10: USART1_RX */
#define USART1_RX_PIN                    (GPIO_PIN_10)
#define USART1_RX_GPIO_FUNC              (GPIO_FUNC_20)

#define USART1_TX_PORT                   (GPIO_PORT_A)   /* PA9: USART1_TX */
#define USART1_TX_PIN                    (GPIO_PIN_09)
#define USART1_TX_GPIO_FUNC              (GPIO_FUNC_20)

/* USART unit definition */
#define USART1_UNIT                      (CM_USART1)
#define USART1_FCG_ENABLE()              (FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_USART1, ENABLE))

void uart1_init(void);

#endif


#endif

