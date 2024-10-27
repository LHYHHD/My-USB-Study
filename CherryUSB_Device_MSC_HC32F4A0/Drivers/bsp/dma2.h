#ifndef _DMA2_H__
#define _DMA2_H__

#define DMA2_UNIT            (CM_DMA2)
#define DMA2_CH              (DMA_CH3)
#define DMA2_TC              (0UL)//һֱ���䣬�д����źžͻᴫ��(�����Ϊ0����ô����TC�κ�Ҫ��������DMA���ܼ�����)
#define DMA2_BC              (2UL)//һ�δ��������������
#define DMA2_DW              (DMA_DATAWIDTH_16BIT)

#define DMA2_SRC_RPT_SIZE	(2u)
#define DMA2_DES_RPT_SIZE	(2u)

//Դ��ַ����������ַƫ��Ϊ1������1�����ݺ�ʼƫ��2������������ADC1->DR0��ͻ��Զ�����ADC1->DR2
#define DMA2_SRC_OFFSET    	(2U)//ƫ����  ADC1->DR0ƫ��0��ַ���䣬ƫ��1ΪADC1->DR1��ƫ��2��ΪADC1->DR2
#define DMA2_SRC_NSEQCNT	(1U)//�ظ�����

#define DMA2_TRIGGER_CH      (AOS_DMA2_3)

void Dma2Init(void);

#endif

