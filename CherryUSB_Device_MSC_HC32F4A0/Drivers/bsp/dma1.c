#include "hc32_ll_dma.h"
#include "hc32f4a0.h"
#include "hc32_ll_aos.h"
#include "hc32_ll_fcg.h"
#include "dma1.h"




//源
uint16_t u32SrcBuf[2] = {0x0,0xffff};
//目标
//CM_GPIOE->PO


void DmaInit(void){
	
    stc_dma_init_t stcDmaInit;
    stc_dma_repeat_init_t stcDmaRepeatInit;
	/* DMA/AOS/TMR2 FCG enable */
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA1|FCG0_PERIPH_AOS, ENABLE);//开DMA、AOS时钟
	
    AOS_SetTriggerEventSrc(DMA_TRIGGER_CH, EVT_SRC_TMRA_11_OVF);//设置DMA触发源

    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn      = DMA_INT_DISABLE;
    stcDmaInit.u32BlockSize  = DMA_BC;//块的大小
    stcDmaInit.u32TransCount = DMA_TC;//传输次数,一次传 DMA_TC*DMA_BC个数据
    stcDmaInit.u32DataWidth  = DMA_DW;//数据大小16bit
    stcDmaInit.u32DestAddr   = (uint32_t)(&CM_GPIO->PODRE);//目标地址
    stcDmaInit.u32SrcAddr    = (uint32_t)(u32SrcBuf);//源地址
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;//源地址递增
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;//目标地址不递增
    (void)DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit);

    (void)DMA_RepeatStructInit(&stcDmaRepeatInit);
    stcDmaRepeatInit.u32Mode     = DMA_RPT_SRC;//源地址重复，目标地址不重复
    stcDmaRepeatInit.u32SrcCount = DMA_SRC_RPT_SIZE;//源地址每2次就重复一次（所以说，就只有源的前2个数据再重复传）
    (void)DMA_RepeatInit(DMA_UNIT, DMA_CH, &stcDmaRepeatInit);
	
	
	/* DMA module enable */
    DMA_Cmd(DMA_UNIT, ENABLE);
	/* DMA channel enable */
    (void)DMA_ChCmd(DMA_UNIT, DMA_CH, ENABLE);
}
