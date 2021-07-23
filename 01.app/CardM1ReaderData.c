#include "includes.h"
#if MT_CardVersion == MT_Card_M1

#define M1_KEY_NUM			2
/*static*/uint8 st_key[M1_KEY_NUM][2][6] = 
{
	{{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}/*{0x02,0x00,0x00,0x00,0xFF,0xFF}*/,{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},		// ÉÈÇø0,keyA,keyB
	{{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},		// ÉÈÇø1,keyA,keyB

};

#if Modify_Key_190826 == 1
#define M1_Key_Sector		2
uint8 Card_Key[M1_Key_Sector][6] = 
{
	{0x00,0x00,0x00,0x00,0xFF,0XFF},
	{0x00,0x00,0x00,0x00,0xFF,0XFF},
};
#endif

static M1_COMDATA st_m1Com;
static M1_CARDDATA st_m1Card[2];

#if Modify_Key_190826 == 1
void M1_UpdateKeyData(uint8* data,uint8 sector)
{
	memcpy((uint8*)&st_key[sector][0][0],(uint8*)&data[0],sizeof(ParamSysCabinet));
}

void M1_UpdateCardKeyData(uint8* data,uint8 sector)
{
	memcpy((uint8*)&Card_Key[sector][0],(uint8*)&data[0],sizeof(ParamSysCabinet));
}

M1_COMDATA Get_M1_ComData(void)
{
	return(st_m1Com);
}
#endif

bool M1_FindCard(void * pData,uint8 dev)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	MT318_FramePara bCitizenCard;
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0002;
	ptData->ucCmdCode = P_M1_FIND;
	memcpy(&bCitizenCard , ptData , 50);
	return true; 
}

bool M1_FindCardResult(void *pData,void *temp)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	MT318_FramePara bCitizenCardResult;
	st_m1Com.result = ptData->ucState;
	memcpy(&bCitizenCardResult , ptData , 50);
	return true;	
}

bool M1_VerifyKeyA(void * pData,uint8 dev)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	MT318_FramePara bCitizenCard;
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0009;
	ptData->ucCmdCode = P_M1_KEYA;
	ptData->ucDataSpace[0] = st_m1Com.sector;
	if(st_m1Com.sector < M1_KEY_NUM)
	{
		memcpy(ptData->ucDataSpace+1,st_key[st_m1Com.sector][0],0x06);
	}
	else
	{
		memcpy(ptData->ucDataSpace+1,st_key[M1_KEY_NUM-1][0],0x06);
	}

	memcpy(&bCitizenCard , ptData , 50);
	return true; 
}

bool M1_VerifyKeyB(void * pData,uint8 dev)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0009;
	ptData->ucCmdCode = P_M1_KEYB;
	ptData->ucDataSpace[0] = st_m1Com.sector;
	if(st_m1Com.sector < M1_KEY_NUM)
	{
		memcpy(ptData->ucDataSpace+1,st_key[st_m1Com.sector][1],0x06);
	}
	return true;
}

bool M1_VerifyResult(void *pData,void *temp)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	MT318_FramePara bCitizenCardResult;
	st_m1Com.result = ptData->ucDataSpace[0];
	memcpy(&bCitizenCardResult , ptData , 50);
	return true;
}

#if Modify_Key_190826 == 1
bool M1_ModifyKeyOpt(void * pData,uint8 dev)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	if(ptData == null)
		return false;
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x0009;
	ptData->ucCmdCode = P_M1_MODIFYKEY;
	ptData->ucDataSpace[0] = st_m1Com.sector;
	if(st_m1Com.sector < M1_KEY_NUM)
	{
		memcpy(ptData->ucDataSpace+1,(uint8*)&Card_Key[st_m1Com.sector][0],0x06);
	}
	return true;
}
#endif

bool M1_ReadBlockData(void *pData,uint8 dev)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
  	MT318_FramePara bCitizenCardData;
	if(ptData == null)
	{
		return false;
	}
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x04;
	ptData->ucCmdCode = P_M1_READ;
	ptData->ucDataSpace[0] = st_m1Com.sector;
	ptData->ucDataSpace[1] = st_m1Com.block;	
	memcpy(&bCitizenCardData, ptData , 50);
	return true;
}

bool M1_ReadResult(void *pData, void *temp)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	MT318_FramePara bCitizenCardResultData;
	st_m1Com.result = ptData->ucDataSpace[1];
	if(st_m1Com.result == P_M1_ACK_OK)
	{
		if((ptData->ucState== st_m1Com.sector)&&
		   (ptData->ucDataSpace[0] == st_m1Com.block))
		{
			memcpy(st_m1Com.bdata,ptData->ucDataSpace+2,16);
		}
	}	
	memcpy(&bCitizenCardResultData , ptData , 50);
	return(true);
}

bool M1_ImpairmentOpt(void * pData,uint8 dev)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	if(ptData == null)
	{
		return false;
	}
	ptData->ucSTX = P_MT318_FrameStart;
	ptData->ucDataLen = 0x08;
	ptData->ucCmdCode = P_M1_IMPAIRMENT;
	ptData->ucDataSpace[0] = st_m1Com.sector;
	ptData->ucDataSpace[1] = st_m1Com.block;
	memcpy(&(ptData->ucDataSpace[2]),st_m1Com.bdata,4);
	return true;
}

bool M1_ImpairmentOptResult(void *pData, void *temp)
{
	MT318_FramePara *ptData = (MT318_FramePara *)pData;
	if(ptData->ucDataSpace[1] == P_M1_ACK_OK)
	{
		if((ptData->ucState== st_m1Com.sector)&&
		   (ptData->ucDataSpace[0] == st_m1Com.block))
		{
			st_m1Com.result = P_M1_ACK_OK;
		}
		else
		{
			st_m1Com.result = P_M1_ACK_FAIL;
		}
	}
	else
	{
		st_m1Com.result = ptData->ucDataSpace[1];		
	}
	return(true);
}

/**************************************************************************************************************/
bool MT_GetRecResult(void)
{
	if(st_m1Com.result == P_M1_ACK_OK)
	{
		return true;
	}
	return false;
}

void MT_ClearRecResult(void)
{
	st_m1Com.result = 0x00;
}

void M1_SetBlockReq(uint8 sector,uint8 block)
{
	st_m1Com.sector = sector;
	st_m1Com.block  = block;
}

void M1_SetCardData(uint8 idx)
{
	st_m1Card[0].block[idx].sector = st_m1Com.sector;
	st_m1Card[0].block[idx].block  = st_m1Com.block;
	memcpy(st_m1Card[0].block[idx].bdata,st_m1Com.bdata,16);
}

void M1_GetM1CardData(M1_CARDDATA* p_stM1Card)
{
	memcpy(p_stM1Card, &st_m1Card[0], sizeof(M1_CARDDATA));
}

void M1_ClearM1CardData(void)
{
	memset((uint8*)&st_m1Card[0].block[0].sector,0x00,sizeof(M1_CARDDATA));
}

uint8 M1_CardReadCom(uint8 state)
{
	uint32 iparam[3] = {0x01,D_M1_CMD_VerifyKeyA,0x00};
	uint8 ret = 0;
	#if Modify_Key_190826 == 1
	HMI_DW_PAGE m1HmiDwPage = {0};
	#endif
	
	if(state == 0)
	{
		memset((uint8*)&st_m1Com.state,0x00,sizeof(M1_COMDATA));
	}
	else
	{
		switch (st_m1Com.state)
		{
			case 0:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_VerifyKeyA)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					st_m1Com.state++;
				}
				#if Modify_Key_190826 == 1
				else if((st_m1Com.cmd == D_M1_CMD_VerifyKeyA)&&(st_m1Com.result == P_M1_ACK_PWFail))
				{
					ret = M1_COM_STATE_PWERR;
				}
				#endif
				else
				{
					iparam[1] = D_M1_CMD_VerifyKeyA;
					st_m1Com.cmd = D_M1_CMD_VerifyKeyA;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;
			case 1:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_VerifyKeyB)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					#if Modify_Key_190826 == 1
						m1HmiDwPage = Get_HmiDwPageParam();
						if(m1HmiDwPage.id == HMI_DW_PAGE_Modify_Key)
						{
							st_m1Com.state += 2;
						}
						else
					#endif
						{
							st_m1Com.state++;
						}
				}
				else
				{
					iparam[1] = D_M1_CMD_VerifyKeyB;
					st_m1Com.cmd = D_M1_CMD_VerifyKeyB;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;
			case 2:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_ReadData)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					ret = M1_COM_STATE_DONE;
				}
				else if((st_m1Com.cmd == D_M1_CMD_ReadData)&&(st_m1Com.result != 0x00))					
				{
					ret = M1_COM_STATE_ACKERR;
				}
				else
				{
					iparam[1] = D_M1_CMD_ReadData;
					st_m1Com.cmd = D_M1_CMD_ReadData;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;
			#if Modify_Key_190826 == 1
			case 3:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_Modify_key)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					ret = M1_COM_ModifyCardKey_Ok;
				}
				else
				{
					iparam[1] = D_M1_CMD_Modify_key;
					st_m1Com.cmd = D_M1_CMD_Modify_key;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;	
			#endif
			default:
				ret = M1_COM_STATE_STATER;
				break;
		}
	}
	return ret;
}

uint8 M1_CardImpairmentCom(uint8 state)
{
	uint32 iparam[3] = {0x01,D_M1_CMD_VerifyKeyA,0x00};
	uint8 ret = 0;
	if(state == 0)
	{
		memset((uint8*)&st_m1Com.state,0x00,sizeof(M1_COMDATA));
	}
	else
	{
		switch (st_m1Com.state)
		{
			case 0:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_VerifyKeyA)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					st_m1Com.state++;
				}
				else
				{
					iparam[1] = D_M1_CMD_VerifyKeyA;
					st_m1Com.cmd = D_M1_CMD_VerifyKeyA;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;
			case 1:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_VerifyKeyB)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					st_m1Com.state++;
				}
				else
				{
					iparam[1] = D_M1_CMD_VerifyKeyB;
					st_m1Com.cmd = D_M1_CMD_VerifyKeyB;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;
			case 2:
				while(SM_GetCmd(D_SM_TASKID_CARD,null))
				{
					Sleep(5);
				}
				if((st_m1Com.cmd == D_M1_CMD_WriteData)&&(st_m1Com.result == P_M1_ACK_OK))
				{
					ret = M1_COM_STATE_DONE;
				}
				else if((st_m1Com.cmd == D_M1_CMD_WriteData)&&(st_m1Com.result != 0x00))					
				{
					ret = M1_COM_STATE_ACKERR;
				}
				else
				{
					iparam[1] = D_M1_CMD_Impairment;
					st_m1Com.cmd = D_M1_CMD_Impairment;
					st_m1Com.result = 0x00;
					SM_SetCmd(D_SM_TASKID_CARD,iparam);
				}
				break;
			
			default:
				ret = M1_COM_STATE_STATER;
				break;
		}
	}
	return ret;
}

#endif

