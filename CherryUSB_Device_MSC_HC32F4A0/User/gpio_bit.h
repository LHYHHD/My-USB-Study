#ifndef _GPIO_BIT_H__
#define _GPIO_BIT_H__

/*
�궨�����
BIT_ADDR(byte_offset, bitnum) ������ض�λ��λ��������ַ�����0x42000000 ��λ������Ļ���ַ��byte_offset ��Ŀ��Ĵ���������������ַ 0x40000000 ��ƫ������bitnum �ǼĴ����е�λ�ţ�0-31��������귵��һ��ָ���λ��������ַ��ָ�롣
GPIOB_OCTL_OFFSET ������GPIOB������ݼĴ�����ODR��ƫ����Ϊ0x14��������������ַ���ֽ�ƫ������
GPIOB_ISTAT_OFFSET ������GPIOB��������״̬�Ĵ�����IDR��ƫ����Ϊ0x10��������������ַ���ֽ�ƫ������
PBout(n) �� PBin(n) ��ֱ����ڷ���GPIOB�ĵ�nλ�����������״̬����Щ��ͨ�� BIT_ADDR �������ĵ�ַ�������ض���λ��n��λ�ţ�0-15����
��Ҫע����ǵ���ʹ��λ������ʱ��Ҫ���ʵı���������volatile�����塣��Ϊc����������֪��ͬһ�����ؿ�����������ַ��ͨ��volatileʹ�ñ�����ÿ�ζ���ʵ�ذ�����ֵд��洢����������䱻�Ż���
*/

/* ����������λ������ */
#define	BIT_GPIO


// λ�������ĺ궨��
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


// ��������������
#define PAin(n)     *(BIT_ADDR(GPIOA_ISTAT_OFFSET,n))   // PA����
#define PBin(n)     *(BIT_ADDR(GPIOB_ISTAT_OFFSET,n))   // PB����
#define PCin(n)     *(BIT_ADDR(GPIOC_ISTAT_OFFSET,n))   // PC����
#define PDin(n)     *(BIT_ADDR(GPIOD_ISTAT_OFFSET,n))   // PD����
#define PEin(n)     *(BIT_ADDR(GPIOE_ISTAT_OFFSET,n))   // PE����
#define PFin(n)     *(BIT_ADDR(GPIOF_ISTAT_OFFSET,n))   // PF����
#define PGin(n)     *(BIT_ADDR(GPIOG_ISTAT_OFFSET,n))   // PG����

#define PAout(n)    *(BIT_ADDR(GPIOA_OCTL_OFFSET,n))    // PA���
#define PBout(n)    *(BIT_ADDR(GPIOB_OCTL_OFFSET,n))    // PB���
#define PCout(n)    *(BIT_ADDR(GPIOC_OCTL_OFFSET,n))    // PC���
#define PDout(n)    *(BIT_ADDR(GPIOD_OCTL_OFFSET,n))    // PD���
#define PEout(n)    *(BIT_ADDR(GPIOE_OCTL_OFFSET,n))    // PE���
#define PFout(n)    *(BIT_ADDR(GPIOF_OCTL_OFFSET,n))    // PF���
#define PGout(n)    *(BIT_ADDR(GPIOG_OCTL_OFFSET,n))    // PG���



#endif

