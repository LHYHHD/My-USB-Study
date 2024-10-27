#include "rtc.h"
#include "hc32_ll_rtc.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll_interrupts.h"
#include "hc32f4a0_ll_interrupts_share.h"


/*
ʵ��RTC��ʱ���ܣ���ѡ��12/24Сʱ�ƣ���ѡ�������жϣ�1ʱ�����ж�(���㱨ʱ����Ҫ��)��
1�������ж�(��ʾʱ��ʱҪ��)���ɵ�������
������1�������жϲ���Ҫ����Ϊ��ʾʱ��ʱֱ����ѯ��ȡʱ������ˡ�

���ݼĴ��������ã����ݼĴ������þ��൱��EEPROMһ���ģ��������ݵ��粻��ʧ������
���������ݼĴ���дָ�����ݣ���������ǳ�������ʱ�Ƿ���Ҫ��������ʱ�䣻
�������˵һ��ΪʲôҪ��ô������Ϊ�����ú󣬵�һ���ϵ����У��ὫRTC�����ں�ʱ������Ϊ
���ǳ������趨�õ����ں�ʱ�䣬�ϵ��ڶ����ϵ磨�����Ժ󣩣���ôRTC�е����ں�ʱ��Ͳ���Ҫ
�ٴ�����Ϊ���ǳ������趨�����ں�ʱ���˵ģ���Ȼ������RTC���Ƶ�ʱ���Ͱ׼�����
������Ҫע���������������RAM�����粻��ʧ�ǿ�VBAT����ɵģ�����֮�⻹��4Kb��SRAMB����SRAM
����ο��ֲ�PWC�½�


���ݼĴ����Ķ�д�����ĺ�����hc32_ll_pwc.c��
	uint8_t PWC_BKR_Read(uint8_t u8RegNum)//�����ݼĴ��������ض��ص�ֵ
	void PWC_BKR_Write(uint8_t u8RegNum, uint8_t u8RegVal)//д���ݼĴ�����ָ����ַ��д�������
	���ݼĴ���ֻ��128�ֽ�

�������ӹ��ܣ�
	����rtc.c��setAlarm���������Ӻ󣬾ͻ��Զ����������жϣ�ֻ������һ�����ӣ�
	��������һ��������һ����߼������壬����һ�ܵ�ÿһ�죬��Щ������Ӳ����ɵģ�
	�ٺ��������п��Լ���ֻ����һ�εĹ��ܣ������Ҫ����ϵ��жϣ�
	���Ե���rtc.h��closeAlarmInterrupt(���ر��������ӣ�����rtc.openAlarmInterrupt(��
	���������ӣ�
	�����жϻص�������rtc.c��RTC_Alarm_IrqHandler()
�������㱨ʱ����(1Сʱ�����ж�)��
	�����жϻص�������rtc.c��RTC_Period_IrqHandler()
	����rtc.h�е�close1HourPreiodInterrupt()�ر����㱨ʱ
	����rtc.h�е�open1HourPreiodInterrupt()�����㱨ʱ
����Сʱ��ʱ�л���
	24Сʱ���л�Ϊ12Сʱ�ƣ�����rtc.c�е�sw12Hour();���Ὣ֮ǰ24Сʱ�����������Ҳ��Ϊ12
	Сʱ���µ����ӣ����Ҳ���ı������жϿ�����״̬�����֮ǰ�ǿ����ģ���ô���ӵ���󣬻ص���
	�����ǻᴥ��
	12Сʱ���л�Ϊ24Сʱ�ƣ�����rtc.c�е�sw24Hour();���Ὣ֮ǰ12Сʱ�����������Ҳ��Ϊ24
	Сʱ���µ�����(����֤)�����Ҳ���ı������жϿ�����״̬�����֮ǰ�ǿ����ģ���ô���ӵ���󣬻ص���
	�����ǻᴥ��
	
	
ʹ��rtc.c������Ʒʱ��ʱ�Ƶ��л������ӽ��һ��Ҫ����ԣ����ܱ�֤�������⣬��Ϊrtc.c�Ĺ��ܶ���
��û����Ļ��û�д����������������debug���ܲ��������

*/


/*
sys_data.u8Day ��(1~31)
sys_data.u8Month ��(1~12)
sys_data.u8Weekday ��(0~6,0Ϊ����,6Ϊ����)
sys_data.u8Year ��(0~99)
*/
stc_rtc_date_t sys_data={
	.u8Day = 15,
	.u8Month = 7,
	.u8Weekday = 1,
	.u8Year = 24
};

/*
sys_time.u8Hour ʱ(0~23)
sys_time.u8Minute ��(0~59)
sys_time.u8Second ��(0~59)
sys_time.u8AmPm 24Сʱ�Ʋ������ã�12Сʱ����0ΪAm��1ΪPm
*/
stc_rtc_time_t sys_time = {
	.u8Hour = 22,
	.u8Minute = 59,
	.u8Second = 40,
	.u8AmPm = 0
};


uint8_t sys_hour_flg = 0;//0Ϊ24Сʱ�ƣ�1Ϊ12Сʱ��


uint8_t csvar=0;//debug�õ�
//RTC�����жϻص�����,��һ�������庯��
void RTC_Period_IrqHandler(void){
    //�����жϿ�������1Сʱ�����ж�
	//����������������㱨ʱ
    RTC_ClearStatus(RTC_FLAG_PERIOD);//��������жϱ�־λ
	csvar=0x99;
	sw12Hour();
}

//RTC�����жϻص�����,��һ�������庯��
void RTC_Alarm_IrqHandler(void){
    //���Ӵ���
    RTC_ClearStatus(RTC_FLAG_ALARM);//��������жϱ�־λ
	csvar=0x88;
	sw24Hour();
}

void setDataTime(stc_rtc_date_t data,stc_rtc_time_t time){
	//��ʮ����дʱ�����������
    RTC_SetDate(RTC_DATA_FMT_DEC, &data);
    RTC_SetTime(RTC_DATA_FMT_DEC, &time);
}

void getDataTime(stc_rtc_date_t *data,stc_rtc_time_t *time){
	//��ʮ���ƶ�ȡ����
	RTC_GetDate(RTC_DATA_FMT_DEC, data);
	RTC_GetTime(RTC_DATA_FMT_DEC, time);
}

//��Ҫ��ʵ��RTC�жϳ�ʼ�������������ж�
//RTC���ó�ʼ��������ʱ����Ƿ��ʼ��ȡ���ں�RTC_UPDATE_FLG��ֵ�Ƿ��иı�
void RTC_Config(void){
	uint8_t v1=0,v2=0;

	//�����жϵ����ã�ÿ���ϵ綼����Ҫ���õ�
	(void)INTC_ShareIrqCmd(INT_SRC_RTC_ALM, ENABLE);
    (void)INTC_ShareIrqCmd(INT_SRC_RTC_PRD, ENABLE);
	/* RTC_INT_IRQn����INT131_IRQn���ο��ֲ�10.3.2�� INT131_IRQn����RTC���ж�*/
	NVIC_ClearPendingIRQ(RTC_INT_IRQn);//�ο��ֲ�10.3.2��
	NVIC_SetPriority(RTC_INT_IRQn, DDL_IRQ_PRIO_15);//����INT131_IRQn�ж����ȼ�
	NVIC_EnableIRQ(RTC_INT_IRQn);//ʹ��INT131_IRQn
	
	/* ������Ӻ������ж� */
	RTC_ClearStatus(RTC_FLAG_CLR_ALL);
	
	v1 = PWC_BKR_Read(0);//��ȡ���ݼĴ����еĵ�0��
	v2 = PWC_BKR_Read(1);//��ȡ���ݼĴ����еĵ�1��
	if(RTC_UPDATE_FLG != (v2<<8|v1)){//˵����Ҫ����
		stc_rtc_init_t stcRtcInit;
		PWC_VBAT_Reset();
		PWC_BKR_PowerCmd(ENABLE);//ʹ�ܱ��ݼĴ�����Դ
		PWC_VBAT_PowerCmd(ENABLE);//ʹ��VBAT��Դ
		
		RTC_DeInit();
		/* Stop RTC */
        RTC_Cmd(DISABLE);
        /* Configure structure initialization */
        (void)RTC_StructInit(&stcRtcInit);
        /* Configuration RTC structure */
        stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_XTAL32;//�ⲿ���پ�����ʱ��Դ
        stcRtcInit.u8HourFormat = RTC_HOUR_FMT_24H;//Ĭ�ϵ�һ���ϵ���24Сʱ��
        stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_HOUR;//1Сʱ�����жϣ����㱨ʱ�����ã�
        (void)RTC_Init(&stcRtcInit);

		sys_hour_flg = 0;//24Сʱ�ƶ�
		PWC_BKR_Write(3,(uint8_t)(sys_hour_flg));
		
        /* ��sys_data��sys_time�е�ֵд��rtc */
        setDataTime(sys_data,sys_time);
		
		//��Ҫ��ʱ����ר�ŵĺ�������
        //RTC_IntCmd(RTC_INT_PERIOD, ENABLE);//�������ж�
		//RTC_IntCmd(RTC_INT_ALARM, ENABLE);//�������ж�
		
		PWC_BKR_Write(0,(uint8_t)(RTC_UPDATE_FLG&0xff));
		PWC_BKR_Write(1,(uint8_t)(RTC_UPDATE_FLG>>8));
		
		/* Startup RTC count */
        RTC_Cmd(ENABLE);
	}else{
		//��ȡʱ���ƶ�
		sys_hour_flg = PWC_BKR_Read(3);//0Ϊ24��1Ϊ12
	}
}

/*
���Զ��������ж�
24Сʱ���£�AmPmֻ��Ϊ0
weekday:	0x01��
			0x02һ
			0x04��
			0x08��
			0x10��
			0x20��
			0x40��
			0x7Fÿ��
���յ���һ�������û������������
*/
void setAlarm(uint8_t alarm_weekday,uint8_t hour,uint8_t minute,uint8_t AmPm){
	stc_rtc_alarm_t stcRtcAlarm;
	stcRtcAlarm.u8AlarmHour    = hour;
	stcRtcAlarm.u8AlarmMinute  = minute;
	stcRtcAlarm.u8AlarmWeekday = alarm_weekday;//ע�����ӵ��ܺ������е����ǲ�һ����
	stcRtcAlarm.u8AlarmAmPm    = AmPm;
	RTC_SetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);
	openAlarmInterrupt();//���ж�,ͬʱ����Alarm����
}


/*
24Сʱ���л�Ϊ12Сʱ�ƣ�����rtc.c�е�sw12Hour();���Ὣ֮ǰ24Сʱ�����������Ҳ��Ϊ12
Сʱ���µ����ӣ����Ҳ���ı������жϿ�����״̬�����֮ǰ�ǿ����ģ���ô���ӵ���󣬻ص���
�����ǻᴥ��
*/
void sw12Hour(void){
	stc_rtc_init_t stcRtcInit;
	stc_rtc_alarm_t stcRtcAlarm;
	
	getDataTime(&sys_data,&sys_time);//��ȡ�л�ǰ��ʱ��
	RTC_GetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//��ȡ�л�ǰ������

	RTC_Cmd(DISABLE);
	(void)RTC_StructInit(&stcRtcInit);
	/* Configuration RTC structure */
	stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_XTAL32;//�ⲿ���پ�����ʱ��Դ
	stcRtcInit.u8HourFormat = RTC_HOUR_FMT_12H;//Ĭ�ϵ�һ���ϵ���24Сʱ��
	stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_HOUR;//1Сʱ�����жϣ����㱨ʱ�����ã�
	(void)RTC_Init(&stcRtcInit);
	/*�������ݵ���һ���ַ���RTC_Cmd(DISABLE);֮ǰ�Ͳ��У���Ӱ��sw24Hour*/
	if(sys_time.u8Hour>12){//С��12���ô���
		sys_time.u8Hour -= 12;
		sys_time.u8AmPm = RTC_HOUR_12H_PM;//����ΪPM
		setDataTime(sys_data,sys_time);
	}
	if(stcRtcAlarm.u8AlarmHour>12){//С��12���ô���
		stcRtcAlarm.u8AlarmHour -= 12;
		stcRtcAlarm.u8AlarmAmPm = RTC_HOUR_12H_PM;//����ΪPM
		//setAlarm();���ﲻ���������������Ϊ��������Ὺ�������жϣ�����ֻ��Ҫ�л�ʱ�ƣ��л�ǰʱʲô�þ͵ñ���ʲô��
		RTC_SetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//������������
	}
	RTC_Cmd(ENABLE);
	
	sys_hour_flg = 1;//12Сʱ�ƶ�
	PWC_BKR_Write(3,(uint8_t)(sys_hour_flg));
}

/*
12Сʱ���л�Ϊ24Сʱ�ƣ�����rtc.c�е�sw24Hour();���Ὣ֮ǰ12Сʱ�����������Ҳ��Ϊ24
Сʱ���µ�����(����֤)�����Ҳ���ı������жϿ�����״̬�����֮ǰ�ǿ����ģ���ô���ӵ���󣬻ص���
�����ǻᴥ��
*/
void sw24Hour(void){
	stc_rtc_init_t stcRtcInit;
	stc_rtc_alarm_t stcRtcAlarm;
	
	getDataTime(&sys_data,&sys_time);//��ȡ�л�ǰ��ʱ��
	RTC_GetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//��ȡ�л�ǰ������

	RTC_Cmd(DISABLE);
	(void)RTC_StructInit(&stcRtcInit);
	/* Configuration RTC structure */
	stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_XTAL32;//�ⲿ���پ�����ʱ��Դ
	stcRtcInit.u8HourFormat = RTC_HOUR_FMT_24H;//Ĭ�ϵ�һ���ϵ���24Сʱ��
	stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_HOUR;//1Сʱ�����жϣ����㱨ʱ�����ã�
	(void)RTC_Init(&stcRtcInit);
	
	if(sys_time.u8AmPm==RTC_HOUR_12H_PM){//RTC_HOUR_12H_AM����Ҫ����
		sys_time.u8Hour += 12;
		sys_time.u8AmPm = 0;//����Ϊ0����ʵҲ����RTC_HOUR_12H_AM��
		setDataTime(sys_data,sys_time);
		csvar = 0x77;
	}
	if(stcRtcAlarm.u8AlarmHour==RTC_HOUR_12H_PM){//RTC_HOUR_12H_AM����Ҫ����
		stcRtcAlarm.u8AlarmHour += 12;
		stcRtcAlarm.u8AlarmAmPm = 0;//����Ϊ0����ʵҲ����RTC_HOUR_12H_AM��
		//setAlarm();���ﲻ���������������Ϊ��������Ὺ�������жϣ�����ֻ��Ҫ�л�ʱ�ƣ��л�ǰʱʲô�þ͵ñ���ʲô��
		RTC_SetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//������������
	}
	RTC_Cmd(ENABLE);
	
	sys_hour_flg = 0;//24Сʱ�ƶ�
	PWC_BKR_Write(3,(uint8_t)(sys_hour_flg));
}



