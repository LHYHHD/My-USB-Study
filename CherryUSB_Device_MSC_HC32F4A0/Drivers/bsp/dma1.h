#ifndef _DMA1_H_
#define _DMA1_H_

#define DMA_UNIT            (CM_DMA1)
#define DMA_CH              (DMA_CH3)
#define DMA_MX_CH           (DMA_MX_CH3)
#define DMA_TC              (0UL)
#define DMA_BC              (1UL)
#define DMA_DW              (DMA_DATAWIDTH_16BIT)
#define DMA_INT_SRC         (INT_SRC_DMA1_TC3)
#define DMA_IRQn            (INT000_IRQn)

#define DMA_SRC_RPT_SIZE    (2U)

#define DMA_TRIGGER_CH      (AOS_DMA1_3)

void DmaInit(void);

#endif
