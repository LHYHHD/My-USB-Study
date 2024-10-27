#include "dma2.h"
#include "hc32_ll_dma.h"
#include "hc32f4a0.h"
#include "hc32_ll_aos.h"
#include "hc32_ll_fcg.h"


//用adc1序列a扫描完成事件触发dma2搬运


//源
//&CM_ADC1->DR0
//目标
uint16_t u32DesBuf[2]={0xAA,0xAA};
/*
若是用128点快速傅里叶变换，那么此数组长度应为128*2，
目标地址重复次数也应该改为DMA2_DES_RPT_SIZE=128*2
*/


void Dma2Init(void){
	
    stc_dma_init_t stcDmaInit;
    stc_dma_repeat_init_t stcDmaRepeatInit;//重复传输结构体
	stc_dma_nonseq_init_t stcDmaNonSeqInit;//不连续重复传输结构体
	/* DMA/AOS/TMR2 FCG enable */
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA2|FCG0_PERIPH_AOS, ENABLE);//开DMA、AOS时钟
	
    AOS_SetTriggerEventSrc(DMA2_TRIGGER_CH, EVT_SRC_ADC1_EOCA);//设置DMA触发源

    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn      = DMA_INT_DISABLE;
    stcDmaInit.u32BlockSize  = DMA2_BC;//块的大小
    stcDmaInit.u32TransCount = DMA2_TC;//传输次数,一次传 DMA_TC*DMA_BC个数据
    stcDmaInit.u32DataWidth  = DMA2_DW;//数据大小16bit
    stcDmaInit.u32DestAddr   = (uint32_t)(u32DesBuf);//目标地址
	stcDmaInit.u32SrcAddr    = (uint32_t)(&CM_ADC1->DR0);//源地址
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;//源地址递增
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;//目标地址不递增
    (void)DMA_Init(DMA2_UNIT, DMA2_CH, &stcDmaInit);

    (void)DMA_RepeatStructInit(&stcDmaRepeatInit);
    stcDmaRepeatInit.u32Mode     = DMA_RPT_BOTH;//源地址重复，目标地址不重复
    stcDmaRepeatInit.u32SrcCount = DMA2_SRC_RPT_SIZE;//虽然源地址跳了一个，但是也是2次重复一次，而不是3次重复一次
	stcDmaRepeatInit.u32DestCount = DMA2_DES_RPT_SIZE;
    (void)DMA_RepeatInit(DMA2_UNIT, DMA2_CH, &stcDmaRepeatInit);
	
    (void)DMA_NonSeqStructInit(&stcDmaNonSeqInit);
    stcDmaNonSeqInit.u32Mode      = DMA_NON_SEQ_SRC;//只有源地址不连续重复传输
    stcDmaNonSeqInit.u32SrcOffset = DMA2_SRC_OFFSET;//偏移量
    stcDmaNonSeqInit.u32SrcCount  = DMA2_SRC_NSEQCNT;//重复次数（重复多少次就偏移）
    (void)DMA_NonSeqInit(DMA2_UNIT, DMA2_CH, &stcDmaNonSeqInit);
	
	
	/* DMA module enable */
    DMA_Cmd(CM_DMA2, ENABLE);
	/* DMA channel enable */
    (void)DMA_ChCmd(CM_DMA2, DMA_CH3, ENABLE);
}
