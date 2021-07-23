#ifndef RTCTIME_H
#define RTCTIME_H

#include "stm32f2xx.h"
#include "stm32f2xx_conf.h"
#include "stdio.h"
#include "type.h"


/* TIME STRUCT */
typedef  struct _TIME_INFO_
{
    uint32 iYear;
    uint8  ucMonth;
    uint8  ucDay;
    uint8  ucWeek;
    uint8  uchour;
    uint8  ucMin;
    uint8  ucSec;

} Time_Info;  

/* 时间结构定义 */
typedef struct _COMMON_TIME_
{
	uint16  iYear;     									//年
	uint8  ucMonth;    								//月
	uint8  ucDay;      									//日
	//uint8  ucWeekday;	 							//周
	uint8  ucHour;     								//时
	uint8  ucMin;      									//分
	uint8  ucSec;      									//秒
} Common_Time;


void RTC_Config(void);
void rtc_Init(void);
void showTime(void);
void RTC_Set_time(Common_Time pTime);
void GetCurrentTime(Common_Time *curTime);
//获取当前的秒数 从2000年开始计算
int32 GetCurSecond(void);
uint32 GetSysTick(void);
int32 TimeToSeconds(Common_Time cdzTime);
int32 TimeToMins(Common_Time cdzTime);

#endif

