#include "includes.h"

uint16 ADCConvertedValue[cSampleTimes][cChannelNum];

static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//说明:仅针对PC0(IN10)--环境温度检测
	GPIO_Init((GPIO_TypeDef *)GPIOC,(GPIO_InitTypeDef *)&GPIO_InitStructure);
}

static void ADC1_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);											
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	DMA_DeInit(DMA2_Stream0);																				
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize =  cSampleTimes * cChannelNum;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);	
}

static void ADC_CommConfig(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	TIM4_Triger_ADC_Config();
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;								
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;									
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;			
	ADC_CommonInit(&ADC_CommonInitStructure);	
}

static void ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;												
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;														
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;												

    //ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;					
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;							

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;												
    ADC_InitStructure.ADC_NbrOfConversion = cChannelNum;												
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1,  ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 2,  ADC_SampleTime_144Cycles);

     ADC_VBATCmd(ENABLE);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);													
    ADC_DMACmd(ADC1, ENABLE);																					
    ADC_Cmd(ADC1, ENABLE);	
}

void AD_UseInit(void)
{
	ADC_GPIO_Config();
	ADC1_DMA_Config();
	ADC_CommConfig();
	ADC1_Init();
}

uint16 ADC_GetADResultAD(uint8 channel)
{
	uint8 i=0;
	uint16 vol=0;
	unsigned long long tmp=0;

	if(channel == Temper1_ADResult)
	{
		for(i=0;i<cSampleTimes;i++)
		{
			tmp += ADCConvertedValue[i][channel];
		}
		tmp /= cSampleTimes;
		vol = tmp;
	}
	return(vol);
}



