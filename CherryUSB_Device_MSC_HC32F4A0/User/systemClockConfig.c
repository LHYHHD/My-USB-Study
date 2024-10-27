#include "hc32_ll_clk.h"
#include "hc32_ll_sram.h"
#include "hc32_ll_efm.h"
#include "hc32_ll_gpio.h"


void systemClockConfig240MHZ(void)
{
//    /* PH0 set to XTAL-IN */
//	  stc_gpio_init_t stcGpioInit;
//    (void)GPIO_StructInit(&stcGpioInit);
//    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
//    (void)GPIO_Init(GPIO_PORT_H, GPIO_PIN_00, &stcGpioInit);

//    /* PH1 set to XTAL-OUT */
//    (void)GPIO_StructInit(&stcGpioInit);
//    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
//    (void)GPIO_Init(GPIO_PORT_H, GPIO_PIN_01, &stcGpioInit);
	
    /* Set bus clock div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                                   CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));
    
	
	/* 片上SRAM 时钟和读写等待 设置(必须在CPU时钟设置之前设置好) */
	//片上SRAM分为4KB 备份SRAM（SRAMB）和512KB系统SRAM（SRAMH/SRAM1/SRAM2/SRAM3/SRAM4）
	// SRAMB/SRAMH/SRAM1/SRAM2/SRAM3/SRAM4的读写等待周期都是可以单独配置的
	//主频小于等于240MHZ，各SRAM读写等待周期都可设为1
    SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
	//主频小于等于240MHZ，SRAMH读写等待周期可设为0
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
	
    /* 片上Flash 时钟和读写等待 设置(必须在CPU时钟设置之前设置好) */
	EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
//	if(f>200) EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
//	else if(f>160) EFM_SetWaitCycle(EFM_WAIT_CYCLE4);
//	else if(f>120) EFM_SetWaitCycle(EFM_WAIT_CYCLE3);
//	else if(f>80) EFM_SetWaitCycle(EFM_WAIT_CYCLE2);
//	else if(f>40) EFM_SetWaitCycle(EFM_WAIT_CYCLE1);
//	else EFM_SetWaitCycle(EFM_WAIT_CYCLE0);
	
	/* 在手册GPIO外设部分搜"等待"*/
	/*当系统运行在高速时钟下，且 PINAER 设置为 0 时，由于 I/O 输入存在延迟，单周期可能无法
	  正确读取输入状态值。此时需要设置寄存器 PCCR.RDWT[2:0]，插入若干等待周期。具体参考寄存
	  器PCCR 说明。*/
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT4);
	
    /* XTAL config */
    stc_clock_xtal_init_t stcXtalInit;
    (void)CLK_XtalStructInit(&stcXtalInit);
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_HIGH;
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);
	
	/*对着手册表 4-2 和XHCode中的时钟树配，将所有时钟拉满即可*/
    /* PLLH config */
	// PLLHP = 8MHZ / M * N / P = 240MHZ
	// PLLHQ = 8MHZ / M * N / Q = 240MHZ
	// PLLHR = 8MHZ / M * N / R = 240MHZ
    stc_clock_pll_init_t stcPLLHInit;
    (void)CLK_PLLStructInit(&stcPLLHInit);
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = (1UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLN = (120UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLP = (4UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLQ = (4UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLR = (4UL - 1UL);
    stcPLLHInit.u8PLLState = CLK_PLL_ON;
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcPLLHInit);
    /* PLLA config */
	// PLLAP = 8MHZ / M * N / P = 240MHZ
	// PLLAQ = 8MHZ / M * N / Q = 240MHZ
	// PLLAR = 8MHZ / M * N / R = 48MHZ 用来给USB FS提高时钟
    stc_clock_pllx_init_t stcPLLAInit;
    (void)CLK_PLLxStructInit(&stcPLLAInit);
    stcPLLAInit.PLLCFGR = 0UL;
    stcPLLAInit.PLLCFGR_f.PLLM = (2UL - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLN = (120UL - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLP = (2UL - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLQ = (2UL - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLR = (10UL - 1UL);
    stcPLLAInit.u8PLLState = CLK_PLLX_ON;
    (void)CLK_PLLxInit(&stcPLLAInit);

    /* 设置系统时钟源 */
	/*
		1） 外部高速振荡器（XTAL）
		2） 外部低速振荡器（XTAL32）
		3） PLLH 时钟（PLLH）
		4） 内部高速振荡器（HRC）
		5） 内部中速振荡器（MRC）
		6） 内部低速振荡器（LRC）
	*/
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);//将系统时钟切换位PLL
	
}

