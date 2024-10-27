#ifndef _RTC_H__
#define _RTC_H__

#include "hc32_ll.h"
#include "hc32_ll_rtc.h"

//ֻҪ��ֵ���ϴ����ع���ʱ�б仯(0x0~0xffff)���ͻ����RTC�е����ں�ʱ������
#define RTC_UPDATE_FLG	0x9999
//ע�⣺���ݼĴ����еĵ�ַ0�͵�ַ1���RTC�Ƿ���±�־)

#define RTC_INT_IRQn	INT131_IRQn//�ο��ֲ�10.3.2�ж��¼��������
#define RTC_INT_PRIO	DDL_IRQ_PRIO_15//�ж����ȼ���;��У����������ӻ��Ǳ�ʱ��ֻ��Ҫ����ж����ȼ���
#define RTC_CLK_SCR		RTC_CLK_SRC_XTAL32//�ⲿ����ʱ��

/* �������ӵ���ʱ������������ר���ܣ����յ������������û������������ */
#define ALARM_SUNDAY                (0x01U)//�����ļ�ֻҪ����rtc.h��������
#define ALARM_MONDAY                (0x02U)
#define ALARM_TUESDAY               (0x04U)
#define ALARM_WEDNESDAY             (0x08U)
#define ALARM_THURSDAY              (0x10U)
#define ALARM_FRIDAY                (0x20U)
#define ALARM_SATURDAY              (0x40U)
#define ALARM_EVERYDAY              (0x7FU)
/* �����ļ�����������������ȡ���ں�ʱ�� */
extern stc_rtc_date_t sys_data;
extern stc_rtc_time_t sys_time;
/* ʱ���ƶ� */
extern uint8_t sys_hour_flg;//0Ϊ24Сʱ�ƶȣ�1Ϊ12Сʱ�ƶȣ��ϵ�ӱ��ݼĴ����л�ȡ
/*��/�ر������ж�*/
#define openAlarmInterrupt()	{RTC_IntCmd(RTC_INT_ALARM, ENABLE);RTC_AlarmCmd(ENABLE);}
#define closeAlarmInterrupt()	{RTC_IntCmd(RTC_INT_ALARM, DISABLE);RTC_AlarmCmd(DISABLE);}
/*��/�ر�1Сʱ�����ж�*/
#define open1HourPreiodInterrupt()	RTC_IntCmd(RTC_INT_PERIOD, ENABLE)
#define close1HourPreiodInterrupt()	RTC_IntCmd(RTC_INT_PERIOD, DISABLE)




void RTC_Config(void);
void setDataTime(stc_rtc_date_t data,stc_rtc_time_t time);
void getDataTime(stc_rtc_date_t *data,stc_rtc_time_t *time);
void setAlarm(uint8_t weekday,uint8_t hour,uint8_t minute,uint8_t AmPm);
void sw12Hour(void);
void sw24Hour(void);
#endif
