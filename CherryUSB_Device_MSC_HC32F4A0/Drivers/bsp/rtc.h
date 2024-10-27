#ifndef _RTC_H__
#define _RTC_H__

#include "hc32_ll.h"
#include "hc32_ll_rtc.h"

//只要此值较上次下载工程时有变化(0x0~0xffff)，就会更新RTC中的日期和时间数据
#define RTC_UPDATE_FLG	0x9999
//注意：备份寄存器中的地址0和地址1存放RTC是否更新标志)

#define RTC_INT_IRQn	INT131_IRQn//参考手册10.3.2中断事件请求序号
#define RTC_INT_PRIO	DDL_IRQ_PRIO_15//中断优先级最低就行（不管是闹钟还是报时都只需要最低中断优先级）
#define RTC_CLK_SCR		RTC_CLK_SRC_XTAL32//外部低速时钟

/* 设置闹钟的周时，必须用闹钟专用周，周日到周六可以利用或运算任意搭配 */
#define ALARM_SUNDAY                (0x01U)//其他文件只要包含rtc.h就能用了
#define ALARM_MONDAY                (0x02U)
#define ALARM_TUESDAY               (0x04U)
#define ALARM_WEDNESDAY             (0x08U)
#define ALARM_THURSDAY              (0x10U)
#define ALARM_FRIDAY                (0x20U)
#define ALARM_SATURDAY              (0x40U)
#define ALARM_EVERYDAY              (0x7FU)
/* 其他文件就用这两个变量获取日期和时间 */
extern stc_rtc_date_t sys_data;
extern stc_rtc_time_t sys_time;
/* 时间制度 */
extern uint8_t sys_hour_flg;//0为24小时制度，1为12小时制度，上电从备份寄存器中获取
/*打开/关闭闹钟中断*/
#define openAlarmInterrupt()	{RTC_IntCmd(RTC_INT_ALARM, ENABLE);RTC_AlarmCmd(ENABLE);}
#define closeAlarmInterrupt()	{RTC_IntCmd(RTC_INT_ALARM, DISABLE);RTC_AlarmCmd(DISABLE);}
/*打开/关闭1小时周期中断*/
#define open1HourPreiodInterrupt()	RTC_IntCmd(RTC_INT_PERIOD, ENABLE)
#define close1HourPreiodInterrupt()	RTC_IntCmd(RTC_INT_PERIOD, DISABLE)




void RTC_Config(void);
void setDataTime(stc_rtc_date_t data,stc_rtc_time_t time);
void getDataTime(stc_rtc_date_t *data,stc_rtc_time_t *time);
void setAlarm(uint8_t weekday,uint8_t hour,uint8_t minute,uint8_t AmPm);
void sw12Hour(void);
void sw24Hour(void);
#endif
