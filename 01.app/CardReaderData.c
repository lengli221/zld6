#include "includes.h"

#if MT_CardVersion == MT622V204
MT_Data st_MTData[2];
static MT_GData st_MTGData;         

bool MT_FindCardGetData(void * pData,uint8 dev)
{//说明:寻卡
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0002;
	ptData->ucCmdCode = P_MT318_FindCard;
	return true;
}

bool MT_FindCardSetData(void *pData,void * temp)
{//说明:寻卡
	MT318_FramePara *ptData = (MT318_FramePara *)pData;

	st_MTData[1].iRcvResult = ptData->ucState;

	if((ptData->ucDataSpace[0] == S1 ) && (ptData->ucDataSpace[1] == S2)) 
	{
		st_MTData[1].ucReState = CardExiting;
	}
	else
	{
		st_MTData[1].ucReState = NoCardExiting;
	}
	return true;
}

bool MT_PreStartGetData(void *pData,uint8 dev)
{//说明:预处理 读取卡片的相关信息
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	
	if(ptData == null)
		return false;
	
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x000C;
	ptData->ucCmdCode = P_MT318_Pretereatment;
	GetCurrentTime(&st_MTGData.st_CommTime);
	memcpy(ptData->ucDataSpace, &st_MTGData.st_CommTime, sizeof(Common_Time));
	param_GetCardPassword((ptData->ucDataSpace)+7);
	
	return true;
}

bool MT_PreSetData(void *pData,void * temp)
{//说明:预处理 读取卡片的相关信息 读卡器上传命令
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	uint8 i = 0;

	st_MTData[1].iRcvResult = ptData->ucState;
	st_MTData[1].st_CardData.ucCardType = ptData->ucDataSpace[0];
	st_MTData[1].st_CardData.iCardState =/*(uint16)*/ptData->ucDataSpace[1]; 
	memcpy(st_MTData[1].st_CardData.byCardNo, ptData->ucDataSpace + 2, 8);
	memcpy(st_MTData[1].st_CardData.byBalance, ptData->ucDataSpace + 10, 4); 
	for(i = 0; i < 4; i++)
	{
		st_MTData[1].st_CardData.iDealCounts +=  ptData->ucDataSpace[14 + i];
		if(i < 3)
		{
			st_MTData[1].st_CardData.iDealCounts = st_MTData[1].st_CardData.iDealCounts << 8;
		}
	}

	return true;
}

bool MT_StartGetData(void * pData,uint8 dev)
{//说明:开始加电 控制读卡器对用户卡和充值卡锁卡操作
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0009;
	ptData->ucCmdCode = P_MT318_StartCharge;
	GetCurrentTime(&st_MTGData.st_CommTime);
	memcpy(ptData->ucDataSpace, &st_MTGData.st_CommTime, sizeof(Common_Time));
	
	return true;
}

bool MT_EndGetData(void *pData,uint8 dev)
{
	uint8 i;
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0011;
	ptData->ucCmdCode = P_MT318_EndCharge;
	for(i = 0; i < 4; i++)
	{
		ptData->ucDataSpace[i] = (uint8)(st_MTGData.iConsume >> (3 - i) * 8);
	}
	GetCurrentTime(&st_MTGData.st_CommTime);
	ptData->ucDataSpace[4] = (uint8)(st_MTGData.st_CommTime.iYear >> 8); 
	ptData->ucDataSpace[5] = (uint8)st_MTGData.st_CommTime.iYear;
	ptData->ucDataSpace[6] = (uint8)st_MTGData.st_CommTime.ucMonth;
	ptData->ucDataSpace[7] = (uint8)st_MTGData.st_CommTime.ucDay;
	ptData->ucDataSpace[8] = (uint8)st_MTGData.st_CommTime.ucHour;
	ptData->ucDataSpace[9] = (uint8)st_MTGData.st_CommTime.ucMin;
	ptData->ucDataSpace[10] = (uint8)st_MTGData.st_CommTime.ucSec;
	//memcpy(&ptData->ucDataSpace[6],&(st_MTGData.st_CommTime+2),(sizeof(Comm_Time)-2));
	for(i = 0; i < 4; i++)
	{
		ptData->ucDataSpace[11 + i] = (uint8)(st_MTGData.iElectric>> (3 - i) * 8); 
	}
	return true;
}

bool MT_StartEndChargeSetData(void *pData,void * temp)
{//说明:开始加电 控制读卡器对用户卡和充值卡锁卡操作 读卡器上传命令格式
	MT318_FramePara *ptData = (MT318_FramePara *)pData;

	st_MTData[1].iRcvResult = ptData->ucState;
	memcpy(st_MTData[1].st_CardData.byBalance, ptData->ucDataSpace, 4); 
	return true;
}


/*******************************************************************************************/

void MT_SetCardExitingFlag(uint8 flag)
{
	st_MTData[1].ucReState = flag;	
}

uint8 MT_GetCardExitingFlag(void)
{
	return(st_MTData[1].ucReState);
}

bool MT_GetRecResult(void)
{
	if(st_MTData[1].iRcvResult == P_MT318_Seccess)
	{
		return true;
	}
	return false;
}

bool MT_ClearRecResult(void)
{
	st_MTData[1].iRcvResult = 0;
	return true;
}

bool MT_GetCardLock(void)
{
	if(st_MTData[1].st_CardData.iCardState == 0x31)       
	{//说明:卡片为灰卡请进行结束加电操作  读卡器回复值:0x31 
		return true;
	}
	
	return false;
}

bool MT_ReadCardRemain(uint32 *remain)
{
	*remain = st_MTData[0].st_CardData.byBalance[0];
	*remain <<= 8;
	*remain += st_MTData[0].st_CardData.byBalance[1];
	*remain <<= 8;
	*remain += st_MTData[0].st_CardData.byBalance[2];
	*remain <<= 8;
	*remain += st_MTData[0].st_CardData.byBalance[3];
	
	return true;
}

bool MT_SetCardConsume(uint32 consume)
{
	uint32 cardRemain;
	
	MT_ReadCardRemain(&cardRemain);
	if(consume >= cardRemain)
	{
		st_MTGData.iConsume = cardRemain;
	}
	else
	{
		st_MTGData.iConsume = consume;
	}
	return true;
}
#endif

