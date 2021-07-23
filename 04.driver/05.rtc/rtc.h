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

/* ʱ��ṹ���� */
typedef struct _COMMON_TIME_
{
	uint16  iYear;     									//��
	uint8  ucMonth;    								//��
	uint8  ucDay;      									//��
	//uint8  ucWeekday;	 							//��
	uint8  ucHour;     								//ʱ
	uint8  ucMin;      									//��
	uint8  ucSec;      									//��
} Common_Time;


void RTC_Config(void);
void rtc_Init(void);
void showTime(void);
void RTC_Set_time(Common_Time pTime);
void GetCurrentTime(Common_Time *curTime);
//��ȡ��ǰ������ ��2000�꿪ʼ����
int32 GetCurSecond(void);
uint32 GetSysTick(void);
int32 TimeToSeconds(Common_Time cdzTime);
int32 TimeToMins(Common_Time cdzTime);

#endif

