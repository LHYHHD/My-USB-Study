#include "tmr0.h"
#include "hc32_ll_interrupts.h"
#include "hc32_ll_fcg.h"
#include "delay_systick.h"
#include "hc32f4xx.h"

/*
利用Tmr0 的unit2 的 chB 实现计时计数功能，可以用来测量一段函数执行的时间
之所以选择Tmr0，是因为其功能最少。

所测量的时间可能并不精准，但是可以提供参考，对开发来说是非常有帮助的，比如测量填充整个屏幕耗时、
测量一次软件sin计算耗时、测量一次硬件sin计算耗时等等。

Tmr0 的时钟来源为内部PCLK1，最高120MHZ
Tmr0Unit2CHB_Init(TMR0_CLK_DIV1);传入不同的分频参数，即可获得不同的最小计时单位，没有开启中断，故
最多计时65535*最小计时单位。

1分频时，最小计时单位为 1/120 微秒，最多计时65535*1/120=546.125us
2分频时，最小计时单位为 1/60 微秒
4分频时，最小计时单位为 1/30 微秒
8分频时，最小计时单位为 1/15 微秒
16分频时，最小计时单位为 1/7.5 微秒
32分频时，最小计时单位 1/3.75 微秒
64分频时，最小计时单位 1/1.875 微秒
128分频时，最小计时单位 1/0.9375 微秒
256分频时，最小计时单位 1/0.46875 微秒
512分频时，最小计时单位 1/0.234375 微秒
1024分频时，最小计时单位 1/0.1171875 微秒

可选择的分频系数有 2、4、8、16、32、64、128、256、512、1024

对于Tmr0 Unit2 CHB来说，计时开启和关闭是由寄存器CM_TMR0_2->BCONR的CSTB（Bit16）位来控制的
对于Tmr0 Unit2 CHA来说，计时开启和关闭是由寄存器CM_TMR0_2->BCONR的CSTA（Bit0） 位来控制的

所以这里，我们启动定时和结束定时就不调用ll库函数，直接自己操作寄存器,将其封装成两个宏
CountTimer_Start()
CountTimer_Stop()

使用实例：
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

//    TMR0_IntCmd(TMR0_UNIT, TMR0_INT_TYPE, ENABLE);//使能中断

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






