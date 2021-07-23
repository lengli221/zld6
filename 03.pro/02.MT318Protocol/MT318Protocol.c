#include "MT318Protocol.h"
#include "type.h"
#include <math.h>
#include <string.h>

static uint8	   ucRecvDaBuf[100];//接收数据缓存                 
static uint16	   iRcvDaLen = 0;//接收到的数据长度               
static uint8	   ucCmd[2];//命令字、命令参数              

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
函数名称： MT_PackageFrame
功能描述： 打包指令
输入参数： pPara：打包所需参数，MT318_FramePara结构
输出参数： pBuf：完成打包后的命令缓存。
           ucDatalen：指令长度
返 回 值： false 失败，true成功
修改记录：
*********************************************************************************************************/
bool MT_PackageFrame(void  *pPara,  void *pBuf1, uint16 *ucDatalen)
{
	MT318_FramePara * MT318_pPara = (MT318_FramePara *)pPara;                    
	uint8 iCounts = 0;//帧长度计数器                          
	uint32 ucCheck;//校验 
	uint8 * pBuf = pBuf1;

	if(pPara == null || pBuf == null || ucDatalen == null)
		return false;

	pBuf[iCounts++] = MT318_pPara->ucSTX;//帧头					
	pBuf[iCounts++] = (uint8) (MT318_pPara->ucDataLen >> 8);//数据域长度	 说明:高在前低在后
	pBuf[iCounts++] = (uint8)  MT318_pPara->ucDataLen;			
	pBuf[iCounts++] = (uint8) (MT318_pPara->ucCmdCode >> 8);//命令字,命令字参数
	pBuf[iCounts++] = (uint8)  MT318_pPara->ucCmdCode;	    	
	ucCmd[0] = (uint8) (MT318_pPara->ucCmdCode >> 8);//保存命令字、命令参数
	ucCmd[1] = (uint8) MT318_pPara->ucCmdCode;               
	memcpy(pBuf + iCounts, MT318_pPara->ucDataSpace , MT318_pPara->ucDataLen - 2);//数据	
	iCounts += (MT318_pPara->ucDataLen - 2); 			    	
	pBuf[iCounts++] = P_MT318_FrameEnd;//打包帧尾	         			    	
	ucCheck = Check(pBuf, iCounts);//打包校验结果
	pBuf[iCounts++] = ucCheck;
	*ucDatalen = iCounts;
	return true;
}

/********************************************************************************************************
函数名称： MT_ParseFrame
功能描述： 解析应答帧
输入参数： pBuf：接收到的数据缓存。
   		   ucDatalen：数据长度。
输出参数： pPara：解析后的数据，MT318_FramePara结构
返 回 值： 返回值：false 失败，true成功。
修改记录：
*********************************************************************************************************/
bool MT_ParseFrame(void  *pPara,  void *pBuf, uint16  ucDatalen)	//说明:通讯格式
{																					//采用RS232,全双工通讯方式,读卡器接收到完整数据包处理完成后立即响应
	MT318_FramePara *MT318_pPara = (MT318_FramePara *)pPara;	//不同的指令响应时间不同,建议超时时间设置为2秒
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

	if(ucRecvDaBuf[0] != P_MT318_FrameStart   || 	//判帧头、帧尾、命令字+命令字参数
	ucRecvDaBuf[3] != ucCmd[0] ||					//说明:每一帧数据的CMD命令字+命令参数字不同,故在底层控制
	ucRecvDaBuf[4] != ucCmd[1] ||
	ucRecvDaBuf[iRcvDaLen - 2] != P_MT318_FrameEnd)
		return false;

	iDataLenTemp = (uint32)(ucRecvDaBuf[2]);//判帧长度	
	iDataLen = iDataLenTemp;
	iDataLenTemp = (uint32)(ucRecvDaBuf[1]) << 8;
	iDataLen += iDataLenTemp;

	if((iDataLen + 5) != iRcvDaLen) 
		return false;

	ucCheck = Check(ucRecvDaBuf, iRcvDaLen - 1);

	if(ucRecvDaBuf[iRcvDaLen - 1] != ucCheck)//判校验参数
	return false;

	MT318_pPara->ucSTX = P_MT318_FrameStart;//读取帧头 
														//读取数据域长度					
	MT318_pPara->ucState = ucRecvDaBuf[5]; //读取状态字 			说明:0x59--正确命令 0x4E--预处理过程失败 0x47--PSAM复位失败 
	memcpy(MT318_pPara->ucDataSpace, ucRecvDaBuf + 6, iDataLen - 3);//读取数据域     			0x48--用户卡,密钥卡,设置卡复位失败 0x57--卡机内失败

	return true;
}

