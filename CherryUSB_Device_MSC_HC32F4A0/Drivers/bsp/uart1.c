#include "uart1.h"
#include "stdio.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_usart.h"
#include "./DEBUG/debug.h"

#if LL_USART_ENABLE == DDL_ON

//printf重定向
int fputc(int ch,FILE *p){
	USART1_UNIT->TDR = (uint8_t)ch; 
	while((USART1_UNIT->SR&0X40)==0);
	return ch;
} 

void uart1_init(void){
	stc_usart_uart_init_t stcUartInit;
	float32_t BaudrateErr;
	
    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(USART1_RX_PORT, USART1_RX_PIN, USART1_RX_GPIO_FUNC);
    GPIO_SetFunc(USART1_TX_PORT, USART1_TX_PIN, USART1_TX_GPIO_FUNC);

    /* Enable peripheral clock */
    USART1_FCG_ENABLE();
	
    /* Initialize UART. */
    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockDiv = USART_CLK_DIV16;
    stcUartInit.u32Baudrate = 115200UL;
    stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
    USART_UART_Init(USART1_UNIT, &stcUartInit, &BaudrateErr);//最后一个参数返回波特率错误率
	
    /* Enable RX function */
    USART_FuncCmd(USART1_UNIT, (USART_TX | USART_RX), ENABLE);

	debugOut(Middl_EWARES,"BaudrateErr%.3f\r\n",BaudrateErr);
}




#endif

