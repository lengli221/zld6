#include "includes.h"

static const TEMP_RELATION tempTabel[]=
{
	{20,1170},
	{21,1208},
	{22,1248},
	{23,1288},
	{24,1328},
	{25,1369},
	{26,1410},
	{27,1452},
	{28,1494},
	{29,1536},
	{30,1579},
	{31,1622},
	{32,1666},
	{33,1709},
	{34,1753},
	{35,1798},
	{36,1842},
	{37,1886},
	{38,1931},
	{39,1976},
	{40,2020},
};

int8 Temp_CalTempByAd(uint16 adVal)
{
	uint8 i=0;
	int8 temp = 0;
	for(i=0;i<sizeof(tempTabel)/sizeof(TEMP_RELATION);i++)
	{
		if(adVal<tempTabel[i].AD)
		{
			break;
		}
	}
	if(i==0)
	{
		temp = -40;
	}
	else if(i >= sizeof(tempTabel)/sizeof(TEMP_RELATION))
	{
		temp = 120;
	}
	else
	{
		temp = tempTabel[i-1].temp;
	}
	return(temp);
}

void CC_Control_Fan(void)
{
	uint16 adTemp = 0;
	int8 temp=0;
	static uint32 itick=0;
	
	adTemp = ADC_GetADResultAD(Temper1_ADResult);
	temp = Temp_CalTempByAd((uint16)adTemp);
	if(temp>=35 && TickOut((uint32 *)itick,5015))
	{
		TurnOnFan(ac_Fan);
	}
	else if(temp<=25)
	{
		TurnOffFan(ac_Fan);
		TickOut((uint32 *)&itick,0x00);
	}
	else
	{
		TickOut((uint32 *)&itick,0x00);
	}
}

