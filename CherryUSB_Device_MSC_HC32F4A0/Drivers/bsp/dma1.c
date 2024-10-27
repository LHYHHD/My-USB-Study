#include "hc32_ll_dma.h"
#include "hc32f4a0.h"
#include "hc32_ll_aos.h"
#include "hc32_ll_fcg.h"
#include "dma1.h"




//Դ
uint16_t u32SrcBuf[2] = {0x0,0xffff};
//Ŀ��
//CM_GPIOE->PO


void DmaInit(void){
	
    stc_dma_init_t stcDmaInit;
    stc_dma_repeat_init_t stcDmaRepeatInit;
	/* DMA/AOS/TMR2 FCG enable */
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA1|FCG0_PERIPH_AOS, ENABLE);//��DMA��AOSʱ��
	
    AOS_SetTriggerEventSrc(DMA_TRIGGER_CH, EVT_SRC_TMRA_11_OVF);//����DMA����Դ

    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn      = DMA_INT_DISABLE;
    stcDmaInit.u32BlockSize  = DMA_BC;//��Ĵ�С
    stcDmaInit.u32TransCount = DMA_TC;//�������,һ�δ� DMA_TC*DMA_BC������
    stcDmaInit.u32DataWidth  = DMA_DW;//���ݴ�С16bit
    stcDmaInit.u32DestAddr   = (uint32_t)(&CM_GPIO->PODRE);//Ŀ���ַ
    stcDmaInit.u32SrcAddr    = (uint32_t)(u32SrcBuf);//Դ��ַ
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;//Դ��ַ����
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;//Ŀ���ַ������
    (void)DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit);

    (void)DMA_RepeatStructInit(&stcDmaRepeatInit);
    stcDmaRepeatInit.u32Mode     = DMA_RPT_SRC;//Դ��ַ�ظ���Ŀ���ַ���ظ�
    stcDmaRepeatInit.u32SrcCount = DMA_SRC_RPT_SIZE;//Դ��ַÿ2�ξ��ظ�һ�Σ�����˵����ֻ��Դ��ǰ2���������ظ�����
    (void)DMA_RepeatInit(DMA_UNIT, DMA_CH, &stcDmaRepeatInit);
	
	
	/* DMA module enable */
    DMA_Cmd(DMA_UNIT, ENABLE);
	/* DMA channel enable */
    (void)DMA_ChCmd(DMA_UNIT, DMA_CH, ENABLE);
}
