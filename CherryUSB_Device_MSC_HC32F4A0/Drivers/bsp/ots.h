#ifndef _OTS_H__
#define _OTS_H__

#include "hc32_ll.h"
/**
 * Specifies a clock source for OTS in this example.
 * 'OTS_CLK_SEL' can be defined as 'OTS_CLK_XTAL' or 'OTS_CLK_HRC'. */
#define OTS_CLK_SEL                     (OTS_CLK_HRC)//OTSʱ��Դ
//ʹ��OTS_CLK_HRC��Ҫ �ⲿ����������XTAL32���������Ĭ��û�н��������
//û����������ʱ����OTS_CLK_XTALҲ�ǿ��Եģ�ots.c�����˴���ֻҪ��һ���������


void OtsInit(void);
float32_t getTemp(void);


#endif
