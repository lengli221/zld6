#ifndef __GPIO_H_
#define __GPIO_H_

#define KC1P_OFF				(GPIOF->BSRRH = GPIO_Pin_8)
#define KC1P_ON				(GPIOF->BSRRL = GPIO_Pin_8)
#define KC3P_OFF				(GPIOF->BSRRH = GPIO_Pin_6)
#define KC3P_ON				(GPIOF->BSRRL = GPIO_Pin_6)
#define KC5P_OFF				(GPIOF->BSRRH = GPIO_Pin_14)
#define KC5P_ON				(GPIOF->BSRRL = GPIO_Pin_14)
#define KC7P_OFF				(GPIOC->BSRRH = GPIO_Pin_1)
#define KC7P_ON				(GPIOC->BSRRL = GPIO_Pin_1)
#define KC9P_OFF				(GPIOE->BSRRH = GPIO_Pin_7)
#define KC9P_ON				(GPIOE->BSRRL = GPIO_Pin_7)
#define KC11P_OFF			(GPIOG->BSRRH = GPIO_Pin_0)
#define KC11P_ON				(GPIOG->BSRRL = GPIO_Pin_0)
#define KC13P_OFF			(GPIOD->BSRRH = GPIO_Pin_11)
#define KC13P_ON				(GPIOD->BSRRL = GPIO_Pin_11)
#define KC15P_OFF			(GPIOE->BSRRH = GPIO_Pin_13)
#define KC15P_ON				(GPIOE->BSRRL = GPIO_Pin_13)
#define KC2P_OFF				(GPIOF->BSRRH = GPIO_Pin_7)
#define KC2P_ON				(GPIOF->BSRRL = GPIO_Pin_7)
#define KC4P_OFF				(GPIOF->BSRRH = GPIO_Pin_9)
#define KC4P_ON				(GPIOF->BSRRL = GPIO_Pin_9)
#define KC6P_OFF				(GPIOF->BSRRH = GPIO_Pin_12)
#define KC6P_ON				(GPIOF->BSRRL = GPIO_Pin_12)
#define KC8P_OFF				(GPIOF->BSRRH = GPIO_Pin_13)
#define KC8P_ON				(GPIOF->BSRRL = GPIO_Pin_13)
#define KC10P_OFF			(GPIOF->BSRRH = GPIO_Pin_15)
#define KC10P_ON				(GPIOF->BSRRL = GPIO_Pin_15)
#define KC12P_OFF			(GPIOG->BSRRH = GPIO_Pin_1)
#define KC12P_ON				(GPIOG->BSRRL = GPIO_Pin_1)
#define KC14P_OFF			(GPIOE->BSRRH = GPIO_Pin_14)
#define KC14P_ON				(GPIOE->BSRRL = GPIO_Pin_14)

#define YX1_IN  				(GPIOG->IDR & GPIO_Pin_12)  					
#define YX3_IN  				(GPIOG->IDR & GPIO_Pin_14) 					
#define YX5_IN   				(GPIOB->IDR & GPIO_Pin_3)				
#define YX7_IN  				(GPIOB->IDR & GPIO_Pin_7)						
#define YX9_IN  				(GPIOB->IDR & GPIO_Pin_9)							
#define YX11_IN  				(GPIOE->IDR & GPIO_Pin_1)
#define YX13_IN  				(GPIOE->IDR & GPIO_Pin_3)  					
#define YX15_IN  				(GPIOE->IDR & GPIO_Pin_5) 					
#define YX2_IN   				(GPIOG->IDR & GPIO_Pin_13)/*(GPIOF->IDR & GPIO_Pin_4)--中天*/		
#define YX4_IN  				(GPIOG->IDR & GPIO_Pin_15)						
#define YX6_IN  				(GPIOB->IDR & GPIO_Pin_4)							
#define YX8_IN  				(GPIOB->IDR & GPIO_Pin_8)
#define YX10_IN  				(GPIOE->IDR & GPIO_Pin_0)							
#define YX12_IN  				(GPIOE->IDR & GPIO_Pin_2)
#define YX14_IN				(GPIOE->IDR & GPIO_Pin_4)
/*脱扣具有恢复功能*/
#define YX_EPO_IN			(GPIOF->IDR & GPIO_Pin_1)//急停故障 低--故障 高--正常 逻辑关系:检测到急停信号之后再脱扣	
#define YX_SPD_IN			(GPIOF->IDR & GPIO_Pin_5)//防雷检测 低--正常 高--故障 逻辑关系:故障报警 -- 修改:烟感报警 故障--脱扣
#define WaterChk_In			(GPIOF->IDR & GPIO_Pin_3)

#define FAN_ON				(GPIOE->BSRRL = GPIO_Pin_11)//风扇控制
#define FAN_OFF				(GPIOE->BSRRH = GPIO_Pin_11)

#define Breaker_ON			(GPIOE->BSRRH = GPIO_Pin_12)
#define Breaker_OFF			(GPIOE->BSRRL = GPIO_Pin_12)

#define ChgACCtr_ON			(GPIOE->BSRRL = GPIO_Pin_10)
#define ChgACCtr_Off			(GPIOE->BSRRH = GPIO_Pin_10)

enum
{
	sysCtrLed = 0,//系统控制灯
};

enum
{
	ac_Fan = 0,//交流风扇 
};

enum
{
	ac_Breaker=0,//AC断路器
};

enum
{
	waring_Led = 0,//报警灯
};

enum
{
	sys_EPO = 0x00,//急停
	sys_SPD = 0x01,//防雷 -- 烟感
	sys_water = 0x02,//水位监测
};

void GPIO_Init_Config(void);
void TurnOnKC(uint8 index);
void TurnOffKC(uint8 index);
uint8 YXState(uint8 index);
void TurnOnLed(uint8 led);
void TurnOffLed(uint8 led);
void TurnOnFan(uint8 fan);
void TurnOffFan(uint8 fan);
void TurnOnBreaker(uint8 breaker);
void TurnOffBreaker(uint8 breaker);
uint8 YXStateSysCtr(uint8 sysPara);
void TurnOnWaringLed(uint8 waringLed);
void TurnOffWaringLed(uint8 waringLed);
#endif

