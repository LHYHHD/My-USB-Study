#include "hc32_ll_tmr2.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_gpio.h"
#include "tmr2.h"



#if 0
void Tmr2Config(void){
	
    stc_tmr2_init_t stcTmr2Init;
	stc_tmr2_pwm_init_t stcPwmInit;

    /* 1. Enable Timer2 peripheral clock. */
    FCG_Fcg2PeriphClockCmd(TMR2_PERIPH_CLK, ENABLE);

    /* 2. Set a default initialization value for stcTmr2Init. */
    (void)TMR2_StructInit(&stcTmr2Init);

    /* 3. Modifies the initialization values depends on the application. */
    stcTmr2Init.u32ClockSrc     = TMR2_CLK_SRC;
    stcTmr2Init.u32ClockDiv     = TMR2_CLK_DIV;
    stcTmr2Init.u32Func         = TMR2_FUNC_CMP;//¹¦ÄÜÑ¡Ôñ
    stcTmr2Init.u32CompareValue = TMR2_CMP_VAL;
    (void)TMR2_Init(TMR2_UNIT, TMR2_CH, &stcTmr2Init);

	
	/* 4. PWM configuration. */
    GPIO_SetFunc(TMR2_PWM_PORT, TMR2_PWM_PIN, TMR2_PWM_PIN_FUNC);
    (void)TMR2_PWM_StructInit(&stcPwmInit);
//	stcPwmInit.u32StartPolarity=TMR2_PWM_LOW;
//	stcPwmInit.u32StopPolarity=TMR2_PWM_LOW;
//	stcPwmInit.u32CompareMatchPolarity=TMR2_PWM_HIGH;
    (void)TMR2_PWM_Init(TMR2_UNIT, TMR2_CH, &stcPwmInit);
    TMR2_PWM_OutputCmd(TMR2_UNIT, TMR2_CH, ENABLE);
}
#endif

