#include "includes.h"

/*
** д��Ƶ����
*/
void write_VoicePara(uint8* space,uint8 dev){
	uint8 voiceLoc = 0;
	uint8 num = 0;
	
	voiceLoc = dev;
	num = 2;
	/*
	** ���ֲ�������
	**	D3�����β��ŵ���ʼ�Σ�0x00-0xFF 
	**	D2�����β��Ŷ�����0x01-0xFF��DGUS ��������㣻������ģʽ��������ʱ�䣬��λ 8ms
	**	D1��������������λΪ 1/256��
	**	D0��ʵʱ���صĲ��Ž��ȣ�ʣ�ಥ�Ŷ���Ŀ 0x00-0xFF��
	**	D3 �� D2 ͬʱд�� 0x00 ��ֹͣ��ǰ����
	*/
	space[0] = voiceLoc;
	space[1] = num;
	space[2] = 0xFF;
	space[3] = 0xFF;
}


/*
** ��Ƶ����
*/
bool lcd_WriteVoiceCtr(void *pData,uint8 dev){
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;

	if(pData == null){
		return false;
	}

	st_FramePara->iAddress = LCD_ADDR_WriteVoice;
	st_FramePara->ucFunCode = D_DP_DataWirte;
	st_FramePara->ucRegLen = LCD_ADDR_Len_WriteVoice;

	/*
	** д��Ƶ����
	*/	
	write_VoicePara((uint8 *)&st_FramePara->ucDataSpace[0], dev);

	return true;
}


