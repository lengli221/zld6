#ifndef __ADC_H_
#define __ADC_H_

#define cSampleTimes													10
#define cChannelNum   												2
#define ADC1_DR_ADDRESS    										((uint32_t)0x4001204C)

#define Temper1_ADResult   											0

void AD_UseInit(void);
uint16 ADC_GetADResultAD(uint8 channel);

#endif

