#ifndef _DEBUG_H__
#define _DEBUG_H__

#include "stdio.h"

/* debug层 */
#define	BSP_DRIVER      0U	//板级驱动层
#define	Middl_EWARES	1U	//中间组件层
#define	USER_APP		2U	//APP层

/* 不同层debug开关 */
#define DEBUG_SWITCH	0x7//bit0:APP层，bit1:中间层，bit2：板级驱动层


//板级驱动层
#if	DEBUG_SWITCH&0x4 != 0
	#define debugOut0(f__,...)	printf("File:%s,Fun:%s,Line:%d"f__,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
	#define debugOut0(f__,...)
#endif

//中间组件层
#if	DEBUG_SWITCH&0x2 != 0
	#define debugOut1(f__,...)	printf("Fun:%s,Line:%d"f__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
	#define debugOut1(f__,...)
#endif

//APP层
#if	DEBUG_SWITCH&0x1 != 0
	#define debugOut2(f__,...)	printf(f__,##__VA_ARGS__)
#else
	#define debugOut2(f__,...)
#endif

#define debugOut(type,f__,...)	if(type==BSP_DRIVER)debugOut0(f__,##__VA_ARGS__); \
								else if(type==Middl_EWARES)debugOut1(f__,##__VA_ARGS__); \
								else if(type==USER_APP)debugOut2(f__,##__VA_ARGS__)

#endif

