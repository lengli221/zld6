#include "MT318Protocol.h"
#include "type.h"
#include <math.h>
#include <string.h>

static uint8	   ucRecvDaBuf[100];//�������ݻ���                 
static uint16	   iRcvDaLen = 0;//���յ������ݳ���               
static uint8	   ucCmd[2];//�����֡��������              

uint8 Check(uint8 *data, uint16 dataLen)
{
    uint8 Res = data[0];
    uint16 i;
    for (i = 1; i < dataLen; i++)
    {
        Res = Res ^ data[i];
    }
    return Res;
}

/********************************************************************************************************
�������ƣ� MT_PackageFrame
���������� ���ָ��
��������� pPara��������������MT318_FramePara�ṹ
��������� pBuf����ɴ���������档
           ucDatalen��ָ���
�� �� ֵ�� false ʧ�ܣ�true�ɹ�
�޸ļ�¼��
*********************************************************************************************************/
bool MT_PackageFrame(void  *pPara,  void *pBuf1, uint16 *ucDatalen)
{
	MT318_FramePara * MT318_pPara = (MT318_FramePara *)pPara;                    
	uint8 iCounts = 0;//֡���ȼ�����                          
	uint32 ucCheck;//У�� 
	uint8 * pBuf = pBuf1;

	if(pPara == null || pBuf == null || ucDatalen == null)
		return false;

	pBuf[iCounts++] = MT318_pPara->ucSTX;//֡ͷ					
	pBuf[iCounts++] = (uint8) (MT318_pPara->ucDataLen >> 8);//�����򳤶�	 ˵��:����ǰ���ں�
	pBuf[iCounts++] = (uint8)  MT318_pPara->ucDataLen;			
	pBuf[iCounts++] = (uint8) (MT318_pPara->ucCmdCode >> 8);//������,�����ֲ���
	pBuf[iCounts++] = (uint8)  MT318_pPara->ucCmdCode;	    	
	ucCmd[0] = (uint8) (MT318_pPara->ucCmdCode >> 8);//���������֡��������
	ucCmd[1] = (uint8) MT318_pPara->ucCmdCode;               
	memcpy(pBuf + iCounts, MT318_pPara->ucDataSpace , MT318_pPara->ucDataLen - 2);//����	
	iCounts += (MT318_pPara->ucDataLen - 2); 			    	
	pBuf[iCounts++] = P_MT318_FrameEnd;//���֡β	         			    	
	ucCheck = Check(pBuf, iCounts);//���У����
	pBuf[iCounts++] = ucCheck;
	*ucDatalen = iCounts;
	return true;
}

/********************************************************************************************************
�������ƣ� MT_ParseFrame
���������� ����Ӧ��֡
��������� pBuf�����յ������ݻ��档
   		   ucDatalen�����ݳ��ȡ�
��������� pPara������������ݣ�MT318_FramePara�ṹ
�� �� ֵ�� ����ֵ��false ʧ�ܣ�true�ɹ���
�޸ļ�¼��
*********************************************************************************************************/
bool MT_ParseFrame(void  *pPara,  void *pBuf, uint16  ucDatalen)	//˵��:ͨѶ��ʽ
{																					//����RS232,ȫ˫��ͨѶ��ʽ,���������յ��������ݰ�������ɺ�������Ӧ
	MT318_FramePara *MT318_pPara = (MT318_FramePara *)pPara;	//��ͬ��ָ����Ӧʱ�䲻ͬ,���鳬ʱʱ������Ϊ2��
	uint8 ucCheck;
	uint32  iDataLen, iDataLenTemp;
	uint8 *temp = pBuf;
	
	if(pBuf == null || pPara == null)
		return false;
	
	while((*temp) == 0xFE)
	{
		temp++;
		if(ucDatalen > 0)
		{
			ucDatalen--;
		}
		else
		{
			return false;
		}
	}
	memcpy((ucRecvDaBuf ), temp, ucDatalen);
	iRcvDaLen = ucDatalen;

	if(ucRecvDaBuf[0] != P_MT318_FrameStart   || 	//��֡ͷ��֡β��������+�����ֲ���
	ucRecvDaBuf[3] != ucCmd[0] ||					//˵��:ÿһ֡���ݵ�CMD������+��������ֲ�ͬ,���ڵײ����
	ucRecvDaBuf[4] != ucCmd[1] ||
	ucRecvDaBuf[iRcvDaLen - 2] != P_MT318_FrameEnd)
		return false;

	iDataLenTemp = (uint32)(ucRecvDaBuf[2]);//��֡����	
	iDataLen = iDataLenTemp;
	iDataLenTemp = (uint32)(ucRecvDaBuf[1]) << 8;
	iDataLen += iDataLenTemp;

	if((iDataLen + 5) != iRcvDaLen) 
		return false;

	ucCheck = Check(ucRecvDaBuf, iRcvDaLen - 1);

	if(ucRecvDaBuf[iRcvDaLen - 1] != ucCheck)//��У�����
	return false;

	MT318_pPara->ucSTX = P_MT318_FrameStart;//��ȡ֡ͷ 
														//��ȡ�����򳤶�					
	MT318_pPara->ucState = ucRecvDaBuf[5]; //��ȡ״̬�� 			˵��:0x59--��ȷ���� 0x4E--Ԥ�������ʧ�� 0x47--PSAM��λʧ�� 
	memcpy(MT318_pPara->ucDataSpace, ucRecvDaBuf + 6, iDataLen - 3);//��ȡ������     			0x48--�û���,��Կ��,���ÿ���λʧ�� 0x57--������ʧ��

	return true;
}

