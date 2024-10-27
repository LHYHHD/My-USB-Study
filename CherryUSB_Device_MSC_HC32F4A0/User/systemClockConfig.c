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
    
	
	/* Ƭ��SRAM ʱ�ӺͶ�д�ȴ� ����(������CPUʱ������֮ǰ���ú�) */
	//Ƭ��SRAM��Ϊ4KB ����SRAM��SRAMB����512KBϵͳSRAM��SRAMH/SRAM1/SRAM2/SRAM3/SRAM4��
	// SRAMB/SRAMH/SRAM1/SRAM2/SRAM3/SRAM4�Ķ�д�ȴ����ڶ��ǿ��Ե������õ�
	//��ƵС�ڵ���240MHZ����SRAM��д�ȴ����ڶ�����Ϊ1
    SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
	//��ƵС�ڵ���240MHZ��SRAMH��д�ȴ����ڿ���Ϊ0
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
	
    /* Ƭ��Flash ʱ�ӺͶ�д�ȴ� ����(������CPUʱ������֮ǰ���ú�) */
	EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
//	if(f>200) EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
//	else if(f>160) EFM_SetWaitCycle(EFM_WAIT_CYCLE4);
//	else if(f>120) EFM_SetWaitCycle(EFM_WAIT_CYCLE3);
//	else if(f>80) EFM_SetWaitCycle(EFM_WAIT_CYCLE2);
//	else if(f>40) EFM_SetWaitCycle(EFM_WAIT_CYCLE1);
//	else EFM_SetWaitCycle(EFM_WAIT_CYCLE0);
	
	/* ���ֲ�GPIO���貿����"�ȴ�"*/
	/*��ϵͳ�����ڸ���ʱ���£��� PINAER ����Ϊ 0 ʱ������ I/O ��������ӳ٣������ڿ����޷�
	  ��ȷ��ȡ����״ֵ̬����ʱ��Ҫ���üĴ��� PCCR.RDWT[2:0]���������ɵȴ����ڡ�����ο��Ĵ�
	  ��PCCR ˵����*/
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT4);
	
    /* XTAL config */
    stc_clock_xtal_init_t stcXtalInit;
    (void)CLK_XtalStructInit(&stcXtalInit);
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_HIGH;
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);
	
	/*�����ֲ�� 4-2 ��XHCode�е�ʱ�����䣬������ʱ����������*/
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
	// PLLAR = 8MHZ / M * N / R = 48MHZ ������USB FS���ʱ��
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

    /* ����ϵͳʱ��Դ */
	/*
		1�� �ⲿ����������XTAL��
		2�� �ⲿ����������XTAL32��
		3�� PLLH ʱ�ӣ�PLLH��
		4�� �ڲ�����������HRC��
		5�� �ڲ�����������MRC��
		6�� �ڲ�����������LRC��
	*/
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);//��ϵͳʱ���л�λPLL
	
}

