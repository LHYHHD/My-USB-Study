#include "tmrA.h"
#include "hc32_ll_tmra.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_interrupts.h"
#include "hc32_ll_gpio.h"


//static void TMRA_IrqCallback(void){
//    TMRA_ClearStatus(TMRA_UNIT, TMRA_INT_FLAG);
//	GPIO_TogglePins(GPIO_PORT_E,GPIO_PIN_ALL);
//}

//static void TmrAIrqConfig(void){
//    stc_irq_signin_config_t stcIrq;

//    stcIrq.enIntSrc    = TMRA_INT_SRC;
//    stcIrq.enIRQn      = TMRA_INT_IRQn;
//    stcIrq.pfnCallback = &TMRA_IrqCallback;
//    (void)INTC_IrqSignIn(&stcIrq);

//    NVIC_ClearPendingIRQ(stcIrq.enIRQn);
//    NVIC_SetPriority(stcIrq.enIRQn, TMRA_INT_PRIO);
//    NVIC_EnableIRQ(stcIrq.enIRQn);

//    /* Enable the specified interrupts of TimerA. */
//    TMRA_IntCmd(TMRA_UNIT, TMRA_INT_TYPE, ENABLE);
//}


void TmrAConfig(void){
	
    stc_tmra_init_t stcTmraInit;
	stc_tmra_pwm_init_t stcPwmInit;
	
    /* 1. Enable TimerA peripheral clock. */
    FCG_Fcg2PeriphClockCmd(TMRA_PERIPH_CLK, ENABLE);

    /* 2. Set a default initialization value for stcTmraInit. */
    (void)TMRA_StructInit(&stcTmraInit);

    /* 3. Modifies the initialization values depends on the application. */
	stcTmraInit.u8CountSrc = TMR_COUNT_SRC;
    stcTmraInit.sw_count.u8ClockDiv  = TMRA_CLK_DIV;
    stcTmraInit.sw_count.u8CountMode = TMRA_MD;
    stcTmraInit.sw_count.u8CountDir  = TMRA_DIR;

    stcTmraInit.u32PeriodValue = TMRA_PERIOD_VAL;
    (void)TMRA_Init(TMRA_UNIT, &stcTmraInit);

//    /* 4. Configures IRQ if needed. */
//    TmrAIrqConfig();
	
	(void)TMRA_PWM_StructInit(&stcPwmInit);
    stcPwmInit.u32CompareValue = TMRA_PWM_CMP_VAL;
	stcPwmInit.u16StartPolarity = TMRA_PWM_HIGH;//这里只指定开始计数时的极性为高就行了，如果没用指定，默认为低
    GPIO_SetFunc(TMRA_PWM_PORT, TMRA_PWM_PIN, TMRA_PWM_PIN_FUNC);
    (void)TMRA_PWM_Init(TMRA_UNIT, TMRA_PWM_CH, &stcPwmInit);
    TMRA_PWM_OutputCmd(TMRA_UNIT, TMRA_PWM_CH, ENABLE);
}


