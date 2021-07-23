#include "includes.h"
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	/*������ò���˵��:��������������ѡ�����ѡ���Group_x�Ǵ�����������������ȷ����
	���Ϸ������:"Cortxe-M3�ں��ļ�"��"Interrupt priority grouping"���˵���Լ��Ĵ���
	Application interrupt and reset control register(SCB->AIRCR)��PRIGROUP bit[10-8]��
	���˵�������Binary ponit����
	���NVIC_InitԴ��ʹ�õ������λ�벢����*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;/*���˵��:ѡ��Group_0,�ʴ˲���ֻ��ѡ0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;/*���˵��:ѡ��Group_2�ʴ˲���ֻ����0-3*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Eeprom_Init(void)
{
	FM24_Init();
}

int8 eeprom_write(int32 addr, uint8 *buf, int32 len)
{
	return FMWrite(buf, addr, len);
}

int8 eeprom_read(int32 addr, uint8 *buf, int32 len)
{
    	return FMRead(buf, addr, len);
}

uint32 BspCPUClkFreq(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	return ((uint32)rcc_clocks.HCLK_Frequency);
}

void BspInit(void)
{
	GPIO_Init_Config();
	rtc_Init();
	PWR_BackupInit();
	watchdogInit();	
	NVIC_Config();
	AD_UseInit();
	Com_Init();
	Eeprom_Init();
	spi_flash_init();
	param_Init();
	/*init ZY Charger Para*/
	 init_ZYChargerPara();	
}

