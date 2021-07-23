#ifndef __DGUSVPRO_H_
#define __DGUSVPRO_H_

/*
** Э���������
*/
#define D_DP_FrameStart1				0x5A		                   		
#define D_DP_FrameStart2				0xA5		                   		

/*
** ���ܲ���˵��
** 1.ָ����ַд�Ĵ�������
** 2.ָ����ַ���Ĵ�������
** 3.ָ����ַ��ʼд�����ݴ��������洢��
** 4.�ӱ����洢��ָ����ַ��ʼ����RD_LEN��������
** 5.д���߻���������
*/
#define D_DP_CtrlWrite					0x80	                     		
#define D_DP_CtrlRead					0x81		                    		
#define D_DP_DataWirte					0x82	                     		
#define D_DP_DataRead					0x83		                    		
#define D_DP_CurveWirte				0x84	                     	

typedef struct __DP_FramePara__
{
	uint16 iAddress;        	 										
	uint8 ucFunCode;         										
	uint8 ucRegLen;         										  	
	uint8 ucDataSpace[256];  									
} DP_FramePara;

bool  DP_PackageFrame(void *pPara, void *pBuf1, uint16 *ucDatalen);
bool  DP_ParseFrame(void *pPara, void *pBuf1, uint16 ucDatalen);

#endif

