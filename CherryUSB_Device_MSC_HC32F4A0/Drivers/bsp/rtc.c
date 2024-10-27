#include "rtc.h"
#include "hc32_ll_rtc.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll_interrupts.h"
#include "hc32f4a0_ll_interrupts_share.h"


/*
实现RTC计时功能，可选择12/24小时制，可选择闹钟中断，1时周期中断(整点报时功能要用)，
1秒周期中断(显示时间时要用)，可调整日期
上述中1秒周期中断不需要，因为显示时间时直接轮询获取时间就行了。

备份寄存器的作用：备份寄存器作用就相当于EEPROM一样的，它的数据掉电不丢失，我们
可以往备份寄存器写指定数据，依次来标记程序运行时是否需要更新日期时间；
（这里多说一下为什么要这么做，因为程序编好后，第一次上电运行，会将RTC的日期和时间设置为
我们程序中设定好的日期和时间，断电后第二次上电（包括以后），那么RTC中的日期和时间就不需要
再次设置为我们程序中设定的日期和时间了的，不然你掉点后RTC所计的时不就白计了吗）
但是需要注意的是它本质上是RAM，掉电不丢失是靠VBAT来完成的，除此之外还有4Kb的SRAMB备份SRAM
具体参考手册PWC章节


备份寄存器的读写操作的函数在hc32_ll_pwc.c中
	uint8_t PWC_BKR_Read(uint8_t u8RegNum)//读备份寄存器，返回读回的值
	void PWC_BKR_Write(uint8_t u8RegNum, uint8_t u8RegVal)//写备份寄存器，指定地址和写入的数据
	备份寄存器只有128字节

关于闹钟功能：
	调用rtc.c中setAlarm设置了闹钟后，就会自动开启闹钟中断；只能设置一个闹钟，
	可以设置一周中任意一天或者几天响铃，或者一周的每一天，这些都是由硬件完成的，
	再后续开发中可以加上只响铃一次的功能，这个需要软件上的判断，
	可以调用rtc.h中closeAlarmInterrupt(）关闭已有闹钟，调用rtc.openAlarmInterrupt(）
	打开已有闹钟，
	闹钟中断回调函数在rtc.c中RTC_Alarm_IrqHandler()
关于整点报时功能(1小时周期中断)：
	周期中断回调函数在rtc.c中RTC_Period_IrqHandler()
	调用rtc.h中的close1HourPreiodInterrupt()关闭整点报时
	调用rtc.h中的open1HourPreiodInterrupt()打开整点报时
关于小时制时切换：
	24小时制切换为12小时制，调用rtc.c中的sw12Hour();它会将之前24小时制下设的闹钟也改为12
	小时制下的闹钟，并且不会改变闹钟中断开启的状态，如果之前是开启的，那么闹钟到点后，回调函
	数还是会触发
	12小时制切换为24小时制，调用rtc.c中的sw24Hour();它会将之前12小时制下设的闹钟也改为24
	小时制下的闹钟(待验证)，并且不会改变闹钟中断开启的状态，如果之前是开启的，那么闹钟到点后，回调函
	数还是会触发
	
	
使用rtc.c开发成品时，时制的切换和闹钟结合一定要多测试，不能保证不出问题，因为rtc.c的功能都是
在没有屏幕，没有串口数据输出，纯靠debug功能测出出来的

*/


/*
sys_data.u8Day 日(1~31)
sys_data.u8Month 月(1~12)
sys_data.u8Weekday 周(0~6,0为周日,6为周六)
sys_data.u8Year 年(0~99)
*/
stc_rtc_date_t sys_data={
	.u8Day = 15,
	.u8Month = 7,
	.u8Weekday = 1,
	.u8Year = 24
};

/*
sys_time.u8Hour 时(0~23)
sys_time.u8Minute 分(0~59)
sys_time.u8Second 秒(0~59)
sys_time.u8AmPm 24小时制不起作用，12小时制下0为Am，1为Pm
*/
stc_rtc_time_t sys_time = {
	.u8Hour = 22,
	.u8Minute = 59,
	.u8Second = 40,
	.u8AmPm = 0
};


uint8_t sys_hour_flg = 0;//0为24小时制，1为12小时制


uint8_t csvar=0;//debug用的
//RTC周期中断回调函数,是一个弱定义函数
void RTC_Period_IrqHandler(void){
    //周期中断开启的是1小时周期中断
	//所以在这里进行整点报时
    RTC_ClearStatus(RTC_FLAG_PERIOD);//清除周期中断标志位
	csvar=0x99;
	sw12Hour();
}

//RTC闹钟中断回调函数,是一个弱定义函数
void RTC_Alarm_IrqHandler(void){
    //闹钟处理
    RTC_ClearStatus(RTC_FLAG_ALARM);//清除闹钟中断标志位
	csvar=0x88;
	sw24Hour();
}

void setDataTime(stc_rtc_date_t data,stc_rtc_time_t time){
	//以十进制写时间和日期数据
    RTC_SetDate(RTC_DATA_FMT_DEC, &data);
    RTC_SetTime(RTC_DATA_FMT_DEC, &time);
}

void getDataTime(stc_rtc_date_t *data,stc_rtc_time_t *time){
	//以十进制读取数据
	RTC_GetDate(RTC_DATA_FMT_DEC, data);
	RTC_GetTime(RTC_DATA_FMT_DEC, time);
}

//主要是实现RTC中断初始化，但不开启中断
//RTC配置初始化和日期时间初是否初始化取决于宏RTC_UPDATE_FLG的值是否有改变
void RTC_Config(void){
	uint8_t v1=0,v2=0;

	//关于中断的配置，每次上电都是需要配置的
	(void)INTC_ShareIrqCmd(INT_SRC_RTC_ALM, ENABLE);
    (void)INTC_ShareIrqCmd(INT_SRC_RTC_PRD, ENABLE);
	/* RTC_INT_IRQn就是INT131_IRQn，参考手册10.3.2， INT131_IRQn就是RTC的中断*/
	NVIC_ClearPendingIRQ(RTC_INT_IRQn);//参考手册10.3.2中
	NVIC_SetPriority(RTC_INT_IRQn, DDL_IRQ_PRIO_15);//设置INT131_IRQn中断优先级
	NVIC_EnableIRQ(RTC_INT_IRQn);//使能INT131_IRQn
	
	/* 清除闹钟和周期中断 */
	RTC_ClearStatus(RTC_FLAG_CLR_ALL);
	
	v1 = PWC_BKR_Read(0);//读取备份寄存器中的第0个
	v2 = PWC_BKR_Read(1);//读取备份寄存器中的第1个
	if(RTC_UPDATE_FLG != (v2<<8|v1)){//说明需要更新
		stc_rtc_init_t stcRtcInit;
		PWC_VBAT_Reset();
		PWC_BKR_PowerCmd(ENABLE);//使能备份寄存器电源
		PWC_VBAT_PowerCmd(ENABLE);//使能VBAT电源
		
		RTC_DeInit();
		/* Stop RTC */
        RTC_Cmd(DISABLE);
        /* Configure structure initialization */
        (void)RTC_StructInit(&stcRtcInit);
        /* Configuration RTC structure */
        stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_XTAL32;//外部低速晶振做时钟源
        stcRtcInit.u8HourFormat = RTC_HOUR_FMT_24H;//默认第一次上电是24小时制
        stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_HOUR;//1小时周期中断（整点报时功能用）
        (void)RTC_Init(&stcRtcInit);

		sys_hour_flg = 0;//24小时制度
		PWC_BKR_Write(3,(uint8_t)(sys_hour_flg));
		
        /* 将sys_data和sys_time中的值写入rtc */
        setDataTime(sys_data,sys_time);
		
		//需要的时候由专门的函数来打开
        //RTC_IntCmd(RTC_INT_PERIOD, ENABLE);//打开周期中断
		//RTC_IntCmd(RTC_INT_ALARM, ENABLE);//打开闹钟中断
		
		PWC_BKR_Write(0,(uint8_t)(RTC_UPDATE_FLG&0xff));
		PWC_BKR_Write(1,(uint8_t)(RTC_UPDATE_FLG>>8));
		
		/* Startup RTC count */
        RTC_Cmd(ENABLE);
	}else{
		//获取时间制度
		sys_hour_flg = PWC_BKR_Read(3);//0为24，1为12
	}
}

/*
会自动打开闹钟中断
24小时制下，AmPm只能为0
weekday:	0x01日
			0x02一
			0x04二
			0x08三
			0x10四
			0x20五
			0x40六
			0x7F每天
周日到周一可以利用或运算任意搭配
*/
void setAlarm(uint8_t alarm_weekday,uint8_t hour,uint8_t minute,uint8_t AmPm){
	stc_rtc_alarm_t stcRtcAlarm;
	stcRtcAlarm.u8AlarmHour    = hour;
	stcRtcAlarm.u8AlarmMinute  = minute;
	stcRtcAlarm.u8AlarmWeekday = alarm_weekday;//注意闹钟的周和日期中的周是不一样的
	stcRtcAlarm.u8AlarmAmPm    = AmPm;
	RTC_SetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);
	openAlarmInterrupt();//打开中断,同时开启Alarm功能
}


/*
24小时制切换为12小时制，调用rtc.c中的sw12Hour();它会将之前24小时制下设的闹钟也改为12
小时制下的闹钟，并且不会改变闹钟中断开启的状态，如果之前是开启的，那么闹钟到点后，回调函
数还是会触发
*/
void sw12Hour(void){
	stc_rtc_init_t stcRtcInit;
	stc_rtc_alarm_t stcRtcAlarm;
	
	getDataTime(&sys_data,&sys_time);//获取切换前的时间
	RTC_GetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//获取切换前的闹钟

	RTC_Cmd(DISABLE);
	(void)RTC_StructInit(&stcRtcInit);
	/* Configuration RTC structure */
	stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_XTAL32;//外部低速晶振做时钟源
	stcRtcInit.u8HourFormat = RTC_HOUR_FMT_12H;//默认第一次上电是24小时制
	stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_HOUR;//1小时周期中断（整点报时功能用）
	(void)RTC_Init(&stcRtcInit);
	/*更改数据的这一部分放在RTC_Cmd(DISABLE);之前就不行！会影响sw24Hour*/
	if(sys_time.u8Hour>12){//小于12不用处理
		sys_time.u8Hour -= 12;
		sys_time.u8AmPm = RTC_HOUR_12H_PM;//设置为PM
		setDataTime(sys_data,sys_time);
	}
	if(stcRtcAlarm.u8AlarmHour>12){//小于12不用处理
		stcRtcAlarm.u8AlarmHour -= 12;
		stcRtcAlarm.u8AlarmAmPm = RTC_HOUR_12H_PM;//设置为PM
		//setAlarm();这里不调用这个函数，因为这个函数会开启闹钟中断，我们只需要切换时制，切换前时什么用就得保持什么样
		RTC_SetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//更新闹钟数据
	}
	RTC_Cmd(ENABLE);
	
	sys_hour_flg = 1;//12小时制度
	PWC_BKR_Write(3,(uint8_t)(sys_hour_flg));
}

/*
12小时制切换为24小时制，调用rtc.c中的sw24Hour();它会将之前12小时制下设的闹钟也改为24
小时制下的闹钟(待验证)，并且不会改变闹钟中断开启的状态，如果之前是开启的，那么闹钟到点后，回调函
数还是会触发
*/
void sw24Hour(void){
	stc_rtc_init_t stcRtcInit;
	stc_rtc_alarm_t stcRtcAlarm;
	
	getDataTime(&sys_data,&sys_time);//获取切换前的时间
	RTC_GetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//获取切换前的闹钟

	RTC_Cmd(DISABLE);
	(void)RTC_StructInit(&stcRtcInit);
	/* Configuration RTC structure */
	stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_XTAL32;//外部低速晶振做时钟源
	stcRtcInit.u8HourFormat = RTC_HOUR_FMT_24H;//默认第一次上电是24小时制
	stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_HOUR;//1小时周期中断（整点报时功能用）
	(void)RTC_Init(&stcRtcInit);
	
	if(sys_time.u8AmPm==RTC_HOUR_12H_PM){//RTC_HOUR_12H_AM不需要处理
		sys_time.u8Hour += 12;
		sys_time.u8AmPm = 0;//设置为0（其实也就是RTC_HOUR_12H_AM）
		setDataTime(sys_data,sys_time);
		csvar = 0x77;
	}
	if(stcRtcAlarm.u8AlarmHour==RTC_HOUR_12H_PM){//RTC_HOUR_12H_AM不需要处理
		stcRtcAlarm.u8AlarmHour += 12;
		stcRtcAlarm.u8AlarmAmPm = 0;//设置为0（其实也就是RTC_HOUR_12H_AM）
		//setAlarm();这里不调用这个函数，因为这个函数会开启闹钟中断，我们只需要切换时制，切换前时什么用就得保持什么样
		RTC_SetAlarm(RTC_DATA_FMT_DEC, &stcRtcAlarm);//更新闹钟数据
	}
	RTC_Cmd(ENABLE);
	
	sys_hour_flg = 0;//24小时制度
	PWC_BKR_Write(3,(uint8_t)(sys_hour_flg));
}



