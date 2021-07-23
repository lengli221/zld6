#include "includes.h"

void GPIO_Init_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_4 | GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_3;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_14 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	

	KC1P_OFF;KC3P_OFF;KC5P_OFF;KC7P_OFF;KC9P_OFF;KC11P_OFF;
	KC13P_OFF;KC15P_OFF;KC2P_OFF;KC4P_OFF;KC6P_OFF;KC8P_OFF;
	KC10P_OFF;
	KC12P_OFF;
	KC14P_OFF;/*Breaker_ON*/Breaker_OFF;FAN_OFF;
	ChgACCtr_ON;
}

void TurnOnKC(uint8 index)
{
	switch(index)
	{
		case 0:
			KC1P_ON;
			break;
		case 1:
			KC3P_ON;
			break;
		case 2:
			KC5P_ON;
			break;
		case 3:
			KC7P_ON;
			break;
		case 4:
			KC9P_ON;
			break;
		case 5:
			KC11P_ON;
			break;
		case 6:
			KC13P_ON;
			break;
		case 7:
			KC15P_ON;
			break;
		case 8:
			KC2P_ON;
			break;
		case 9:
			KC4P_ON;
			break;
		case 10:
			KC6P_ON;
			break;
		case 11:
			KC8P_ON;
			break;
		case 12:
			KC10P_ON;
			break;
		case 13:
			KC12P_ON;
			break;
		case 14:
			KC14P_ON;
			break;
		default:
			break;
	}
}

void TurnOffKC(uint8 index)
{
	switch(index)
	{
		case 0:
			KC1P_OFF;
			break;
		case 1:
			KC3P_OFF;
			break;
		case 2:
			KC5P_OFF;
			break;
		case 3:
			KC7P_OFF;
			break;
		case 4:
			KC9P_OFF;
			break;
		case 5:
			KC11P_OFF;
			break;
		case 6:
			KC13P_OFF;
			break;
		case 7:
			KC15P_OFF;
			break;
		case 8:
			KC2P_OFF;
			break;
		case 9:
			KC4P_OFF;
			break;
		case 10:
			KC6P_OFF;
			break;
		case 11:
			KC8P_OFF;
			break;
		case 12:
			KC10P_OFF;
			break;
		case 13:
			KC12P_OFF;
			break;
		case 14:
			KC14P_OFF;
			break;
		default:
			break;
	}
}

/*
** return @param:
**			1:仓门开
**			0:仓门关
*/
uint8 YXState(uint8 index)
{
	switch(index)
	{
		case 0:
			return ((YX1_IN   == 0 ) ? 1 : 0);  
		case 1:
			return ((YX3_IN   == 0 ) ? 1 : 0); 
		case 2:
			return ((YX5_IN   == 0 ) ? 1 : 0);
		case 3:
			return ((YX7_IN   == 0 ) ? 1 : 0);
		case 4:
			return ((YX9_IN   == 0 ) ? 1 : 0);
		case 5:
			return ((YX11_IN == 0 ) ? 1 : 0);
		case 6:
			return ((YX13_IN   == 0 ) ? 1 : 0);  
		case 7:
			return ((YX15_IN   == 0 ) ? 1 : 0); 
		case 8:
			return ((YX2_IN   == 0 ) ? 1 : 0);
		case 9:
			return ((YX4_IN   == 0 ) ? 1 : 0);
		case 10:
			return ((YX6_IN   == 0 ) ? 1 : 0);
		case 11:
			return ((YX8_IN == 0 ) ? 1 : 0);
		case 12:
			return ((YX10_IN   == 0 ) ? 1 : 0);
		case 13:
			return ((YX12_IN == 0 ) ? 1 : 0);	
		case 14:
			return ((YX14_IN == 0 ) ? 1 : 0);	
		default:
			break;
	}
	return(0);
}

uint8 YXStateSysCtr(uint8 sysPara)
{//返回值说明:0x01--异常 0x00--正常
	switch(sysPara)
	{
		case sys_EPO:
			return ((YX_EPO_IN == 0) ? 1 : 0);
		#if HengSheng_MaskSmokeWater == 1
			case sys_SPD:
				return ((YX_SPD_IN == 0) ? 0 : 0);
			case sys_water:
				return ((WaterChk_In == 0) ? 0 : 0);		
		#else	
			case sys_SPD:
				return ((YX_SPD_IN == 0) ? 0 : 1);
			case sys_water:
				return ((WaterChk_In == 0) ? 0 : 1);
		#endif	
		default:

			break;
	}
	return(0);
}

void TurnOnLed(uint8 led)
{
	switch(led)
	{
		case sysCtrLed:
			
			break;
		default :
			
			break;
	}
}

void TurnOffLed(uint8 led)
{
	switch(led)
	{
		case sysCtrLed:
			
			break;
		default:
			
			break;
	}
}

void TurnOnFan(uint8 fan)
{
	switch(fan)
	{
		case ac_Fan:		
			FAN_ON;
			break;
		default:
			
			break;
	}
}

void TurnOffFan(uint8 fan)
{
	switch(fan)
	{
		case ac_Fan:
			FAN_OFF;
			break;
		default:
			
			break;
	}
}

void TurnOnBreaker(uint8 breaker)
{
	switch(breaker)
	{
		case ac_Breaker:
			Breaker_ON;
			break;
		default:
			
			break;
	}
}

void TurnOffBreaker(uint8 breaker)
{
	switch(breaker)
	{
		case ac_Breaker:
			Breaker_OFF;
			break;
		default:
			
			break;
	}
}

/*恒晟屏蔽告警灯*/
void TurnOnWaringLed(uint8 waringLed)
{
	switch(waringLed)
	{
		case 0:
			
			break;
		default:
			
			break;
	}
}

void TurnOffWaringLed(uint8 waringLed)
{
	switch(waringLed)
	{
		case 0:
			
			break;
		default:
			
			break;
	}
}

