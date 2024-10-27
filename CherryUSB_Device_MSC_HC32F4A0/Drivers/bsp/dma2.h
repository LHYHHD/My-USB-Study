#ifndef _DMA2_H__
#define _DMA2_H__

#define DMA2_UNIT            (CM_DMA2)
#define DMA2_CH              (DMA_CH3)
#define DMA2_TC              (0UL)//一直传输，有触发信号就会传输(如果不为0，那么传完TC次后要重新配置DMA才能继续传)
#define DMA2_BC              (2UL)//一次传输搬运两个数据
#define DMA2_DW              (DMA_DATAWIDTH_16BIT)

#define DMA2_SRC_RPT_SIZE	(2u)
#define DMA2_DES_RPT_SIZE	(2u)

//源地址不连续，地址偏移为1，传输1个数据后开始偏移2。这样传输完ADC1->DR0后就会自动跳到ADC1->DR2
#define DMA2_SRC_OFFSET    	(2U)//偏移量  ADC1->DR0偏移0地址不变，偏移1为ADC1->DR1，偏移2才为ADC1->DR2
#define DMA2_SRC_NSEQCNT	(1U)//重复次数

#define DMA2_TRIGGER_CH      (AOS_DMA2_3)

void Dma2Init(void);

#endif

