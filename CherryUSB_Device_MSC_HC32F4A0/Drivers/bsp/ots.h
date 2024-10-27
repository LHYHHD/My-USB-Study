#ifndef _OTS_H__
#define _OTS_H__

#include "hc32_ll.h"
/**
 * Specifies a clock source for OTS in this example.
 * 'OTS_CLK_SEL' can be defined as 'OTS_CLK_XTAL' or 'OTS_CLK_HRC'. */
#define OTS_CLK_SEL                     (OTS_CLK_HRC)//OTS时钟源
//使用OTS_CLK_HRC需要 外部低速振荡器（XTAL32），天空星默认没有接这个晶振
//没接这个晶振的时候用OTS_CLK_XTAL也是可以的，ots.c中做了处理，只要改一个宏就行了


void OtsInit(void);
float32_t getTemp(void);


#endif
