#include "dma2.h"
#include "hc32_ll_dma.h"
#include "hc32f4a0.h"
#include "hc32_ll_aos.h"
#include "hc32_ll_fcg.h"


//��adc1����aɨ������¼�����dma2����


//Դ
//&CM_ADC1->DR0
//Ŀ��
uint16_t u32DesBuf[2]={0xAA,0xAA};
/*
������128����ٸ���Ҷ�任����ô�����鳤��ӦΪ128*2��
Ŀ���ַ�ظ�����ҲӦ�ø�ΪDMA2_DES_RPT_SIZE=128*2
*/


void Dma2Init(void){
	
    stc_dma_init_t stcDmaInit;
    stc_dma_repeat_init_t stcDmaRepeatInit;//�ظ�����ṹ��
	stc_dma_nonseq_init_t stcDmaNonSeqInit;//�������ظ�����ṹ��
	/* DMA/AOS/TMR2 FCG enable */
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA2|FCG0_PERIPH_AOS, ENABLE);//��DMA��AOSʱ��
	
    AOS_SetTriggerEventSrc(DMA2_TRIGGER_CH, EVT_SRC_ADC1_EOCA);//����DMA����Դ

    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn      = DMA_INT_DISABLE;
    stcDmaInit.u32BlockSize  = DMA2_BC;//��Ĵ�С
    stcDmaInit.u32TransCount = DMA2_TC;//�������,һ�δ� DMA_TC*DMA_BC������
    stcDmaInit.u32DataWidth  = DMA2_DW;//���ݴ�С16bit
    stcDmaInit.u32DestAddr   = (uint32_t)(u32DesBuf);//Ŀ���ַ
	stcDmaInit.u32SrcAddr    = (uint32_t)(&CM_ADC1->DR0);//Դ��ַ
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;//Դ��ַ����
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;//Ŀ���ַ������
    (void)DMA_Init(DMA2_UNIT, DMA2_CH, &stcDmaInit);

    (void)DMA_RepeatStructInit(&stcDmaRepeatInit);
    stcDmaRepeatInit.u32Mode     = DMA_RPT_BOTH;//Դ��ַ�ظ���Ŀ���ַ���ظ�
    stcDmaRepeatInit.u32SrcCount = DMA2_SRC_RPT_SIZE;//��ȻԴ��ַ����һ��������Ҳ��2���ظ�һ�Σ�������3���ظ�һ��
	stcDmaRepeatInit.u32DestCount = DMA2_DES_RPT_SIZE;
    (void)DMA_RepeatInit(DMA2_UNIT, DMA2_CH, &stcDmaRepeatInit);
	
    (void)DMA_NonSeqStructInit(&stcDmaNonSeqInit);
    stcDmaNonSeqInit.u32Mode      = DMA_NON_SEQ_SRC;//ֻ��Դ��ַ�������ظ�����
    stcDmaNonSeqInit.u32SrcOffset = DMA2_SRC_OFFSET;//ƫ����
    stcDmaNonSeqInit.u32SrcCount  = DMA2_SRC_NSEQCNT;//�ظ��������ظ����ٴξ�ƫ�ƣ�
    (void)DMA_NonSeqInit(DMA2_UNIT, DMA2_CH, &stcDmaNonSeqInit);
	
	
	/* DMA module enable */
    DMA_Cmd(CM_DMA2, ENABLE);
	/* DMA channel enable */
    (void)DMA_ChCmd(CM_DMA2, DMA_CH3, ENABLE);
}
