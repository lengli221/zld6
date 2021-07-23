#include "includes.h"

const int g_wDaysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void RTC_Config(void)
{
    __IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div25); //1M
    SynchPrediv = 0x1f3f;  //max_value：0x1ffe
    AsynchPrediv = 0x7c; //
    RCC_RTCCLKCmd(ENABLE);
    /* Wait for RTC APB registers synchronisation */
    while( RTC_WaitForSynchro() == ERROR)
    {}
    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);
    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);
}

//设置时间
void RTC_Set_time(Common_Time st_Time)
{
	uint8 cnt;
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    time.RTC_Hours = st_Time.ucHour;
    time.RTC_Minutes = st_Time.ucMin;
    time.RTC_Seconds = st_Time.ucSec;
    //while( RTC_SetTime(RTC_Format_BIN, &time) == ERROR)
    //{}
    cnt = 5;
    while(cnt--)
    {
		if( RTC_SetTime(RTC_Format_BIN, &time) == SUCCESS)
		{
			break;
		}
    }
    date.RTC_WeekDay = 0;
    date.RTC_Date = st_Time.ucDay;
    date.RTC_Month = st_Time.ucMonth;
    date.RTC_Year = st_Time.iYear;
    cnt = 5;
	while(cnt--)
	{
		if(RTC_SetDate(RTC_Format_BIN, &date) == SUCCESS)
		{
			break;
		}
	}
}


void rtc_Init(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    RTC_InitStructure.RTC_AsynchPrediv = 0x7f;
    RTC_InitStructure.RTC_SynchPrediv = 0xff;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RCC_RTCCLKCmd(ENABLE);
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
}

static int32 DaysFrom2000(const Common_Time time)
{
    int y, m, d, days;
    y = 2000;
    m = 1;
    d = 1;

    days = 0;
    while (y < time.iYear)
    {
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) //闰年
            days += 366;
        else
            days += 365;

        y++;
    }

    //现在：y == year
    while (m < time.ucMonth)
    {
        if (m == 2)
        {
            if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) //闰年
                days += 29;
            else
                days += 28;

        }
        else
        {
            days += g_wDaysOfMonth[m - 1];
        }
        m++;
    }
    //现在：m == month
    days += time.ucDay - d;
    return days;
}

//获取当前的时间
void GetCurrentTime(Common_Time *curTime)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	curTime->iYear = RTC_DateStructure.RTC_Year;
	curTime->ucMonth = RTC_DateStructure.RTC_Month;
	curTime->ucDay = RTC_DateStructure.RTC_Date;
	//curTime->ucWeekday = RTC_DateStructure.RTC_WeekDay;
	curTime->ucHour =  RTC_TimeStructure.RTC_Hours;
	curTime->ucSec =  RTC_TimeStructure.RTC_Seconds;
	curTime->ucMin =  RTC_TimeStructure.RTC_Minutes;
}

int32 TimeToSeconds(Common_Time cdzTime)
{
    return DaysFrom2000(cdzTime) * 60 * 60 * 24 +
           (int32 )(cdzTime.ucHour) * 60 * 60 +
           cdzTime.ucMin * 60 + cdzTime.ucSec;
}

int32 TimeToMins(Common_Time cdzTime)
{
    return DaysFrom2000(cdzTime) * 60 * 24 +
           (int32 )(cdzTime.ucHour)  * 60 +
           cdzTime.ucMin;
}

//获取当前的秒数 从2000年开始计算
int32 GetCurSecond(void)
{
	Common_Time curTime;
	
	GetCurrentTime(&curTime);

	return TimeToSeconds(curTime);
}

//获取系统滴答数
uint32 GetSysTick(void)
{

    return (OSTimeGet());

}

