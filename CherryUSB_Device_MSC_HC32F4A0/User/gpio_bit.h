#ifndef _GPIO_BIT_H__
#define _GPIO_BIT_H__

/*
宏定义解释
BIT_ADDR(byte_offset, bitnum) 宏计算特定位的位带别名地址。这里，0x42000000 是位带区域的基地址，byte_offset 是目标寄存器相对于外设基地址 0x40000000 的偏移量，bitnum 是寄存器中的位号（0-31）。这个宏返回一个指向该位带别名地址的指针。
GPIOB_OCTL_OFFSET 定义了GPIOB输出数据寄存器（ODR，偏移量为0x14）相对于外设基地址的字节偏移量。
GPIOB_ISTAT_OFFSET 定义了GPIOB输入数据状态寄存器（IDR，偏移量为0x10）相对于外设基地址的字节偏移量。
PBout(n) 和 PBin(n) 宏分别用于访问GPIOB的第n位的输出和输入状态。这些宏通过 BIT_ADDR 宏计算出的地址来操作特定的位，n是位号（0-15）。
需要注意的是当你使用位带功能时，要访问的变量必须用volatile来定义。因为c编译器并不知道同一个比特可以有两个地址。通过volatile使得编译器每次都如实地把新数值写入存储器，避免语句被优化。
*/

/* 声明开启了位带操作 */
#define	BIT_GPIO


// 位带操作的宏定义
#define BIT_ADDR(byte_offset,bitnum)  (volatile unsigned long*)(0x42000000 + (byte_offset * 32) + (bitnum * 4))

#define GPIOA_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 0)) - 0x40000000)
#define GPIOB_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 1)) - 0x40000000)
#define GPIOC_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 2)) - 0x40000000)
#define GPIOD_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 3)) - 0x40000000)
#define GPIOE_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 4)) - 0x40000000)
#define GPIOF_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 5)) - 0x40000000)
#define GPIOG_OCTL_OFFSET ((CM_GPIO_BASE + 0x4 + (0x10 * 6)) - 0x40000000)

#define GPIOA_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 0)) - 0x40000000)
#define GPIOB_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 1)) - 0x40000000)
#define GPIOC_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 2)) - 0x40000000)
#define GPIOD_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 3)) - 0x40000000)
#define GPIOE_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 4)) - 0x40000000)
#define GPIOF_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 5)) - 0x40000000)
#define GPIOG_ISTAT_OFFSET ((CM_GPIO_BASE + (0x10 * 6)) - 0x40000000)


// 定义输出和输入宏
#define PAin(n)     *(BIT_ADDR(GPIOA_ISTAT_OFFSET,n))   // PA输入
#define PBin(n)     *(BIT_ADDR(GPIOB_ISTAT_OFFSET,n))   // PB输入
#define PCin(n)     *(BIT_ADDR(GPIOC_ISTAT_OFFSET,n))   // PC输入
#define PDin(n)     *(BIT_ADDR(GPIOD_ISTAT_OFFSET,n))   // PD输入
#define PEin(n)     *(BIT_ADDR(GPIOE_ISTAT_OFFSET,n))   // PE输入
#define PFin(n)     *(BIT_ADDR(GPIOF_ISTAT_OFFSET,n))   // PF输入
#define PGin(n)     *(BIT_ADDR(GPIOG_ISTAT_OFFSET,n))   // PG输入

#define PAout(n)    *(BIT_ADDR(GPIOA_OCTL_OFFSET,n))    // PA输出
#define PBout(n)    *(BIT_ADDR(GPIOB_OCTL_OFFSET,n))    // PB输出
#define PCout(n)    *(BIT_ADDR(GPIOC_OCTL_OFFSET,n))    // PC输出
#define PDout(n)    *(BIT_ADDR(GPIOD_OCTL_OFFSET,n))    // PD输出
#define PEout(n)    *(BIT_ADDR(GPIOE_OCTL_OFFSET,n))    // PE输出
#define PFout(n)    *(BIT_ADDR(GPIOF_OCTL_OFFSET,n))    // PF输出
#define PGout(n)    *(BIT_ADDR(GPIOG_OCTL_OFFSET,n))    // PG输出



#endif

