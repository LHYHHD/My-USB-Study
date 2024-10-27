#include "ots.h"
#include "hc32_ll_ots.h"
#include "hc32_ll_clk.h"
#include "hc32_ll_fcg.h"

//使用内置默认参数测温

void OtsInit(void){
    stc_ots_init_t stcOTSInit;

    (void)OTS_StructInit(&stcOTSInit);
    stcOTSInit.u16ClockSrc = OTS_CLK_SEL;
#if (OTS_CLK_SEL == OTS_CLK_XTAL)
    /* Real XTAL frequency(MHz). */
    stcOTSInit.stcParaCond.u16ClockFreq = 8U;
#else
    /* Real HRC frequency(MHz). */
    stcOTSInit.stcParaCond.u16ClockFreq = 16U;
#endif /* #if (OTS_CLK_SEL == OTS_CLK_XTAL) */

    /* 1. Enable OTS peripheral clock. */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_OTS, ENABLE);
    /* 2. Initialize OTS. */
    (void)OTS_Init(&stcOTSInit);
	
#if (OTS_CLK_SEL == OTS_CLK_HRC)
    /* 1. Enable HRC for OTS. */
    (void)CLK_HrcCmd(ENABLE);
    /* 2. Enable XTAL32 when clock source of OTS is HRC. */
    (void)CLK_Xtal32Cmd(ENABLE);
#else
    /* Enable XTAL for OTS. */
    /* XTAL is already enabled in SystemClockConfig() */
#endif
    /* Enable LRC for OTS. */
    (void)CLK_LrcCmd(ENABLE);
}

float32_t getTemp(void){
	OTS_Start();
	return OTS_CalculateTemp();
}

