#include "includes.h"

/*
** zy CRC16
*/
uint16 modBus_CRC16(uint8* data,uint8 size){
	uint16 m_Crc = 0;
	uint16 m_InitCrc = 0xFFFF;
	uint8 i = 0;
	uint8 j = 0;

	for(i=0;i<size;i++){
		m_InitCrc ^= data[i];
		for(j=0;j<8;j++){
			m_Crc = m_InitCrc;
			m_InitCrc >>= 1;
			if(m_Crc & 0x0001){
				m_InitCrc ^= 0xA001;
			}
		}
	}

	return m_InitCrc;
}


/*
** zy Package Get Info
*/
static bool zy_PackageFrame_GetInfo(ZY_Charger* pPara,uint8* pBuf,uint16* ucDataLen){
	uint8 iCount = 0;
	uint16 crc = 0;
	uint16 temp = 0;

	if(pPara == null || pBuf == null || ucDataLen == null){
		return false;
	}

	/*packect*/
	pBuf[iCount++] = pPara->addr;
	pBuf[iCount++] = pPara->funCode;
	temp = little_Big_Endian__Exchange(pPara->registerAddr);
	memcpy((uint8*)&pBuf[iCount],(uint8*)&temp,sizeof(uint16));
	iCount += sizeof(uint16);
	temp = little_Big_Endian__Exchange(pPara->registerNum);
	memcpy((uint8*)&pBuf[iCount],(uint8*)&temp,sizeof(uint16));
	iCount += sizeof(uint16);
	crc = modBus_CRC16(pBuf,iCount);
	memcpy((uint8*)&pBuf[iCount],(uint8*)&crc,sizeof(uint16));
	iCount += sizeof(uint16);

	/*data Len*/
	*ucDataLen = iCount;

	return true;
}

/*
** zy Package Set Info
*/
static bool zy_PackageFrame_SetInfo(ZY_Charger* pPara,uint8* pBuf,uint16* ucDataLen){
	uint8 iCount = 0;
	uint16 crc = 0;
	uint16 temp = 0;

	if(pPara == null || pBuf == null || ucDataLen == null){
		return false;
	}

	/*packect*/
	pBuf[iCount++] = pPara->addr;
	pBuf[iCount++] = pPara->funCode;
	temp = little_Big_Endian__Exchange(pPara->registerAddr);
	memcpy((uint8*)&pBuf[iCount],(uint8*)&temp,sizeof(uint16));
	iCount += sizeof(uint16);
	temp = little_Big_Endian__Exchange(pPara->registerNum);
	memcpy((uint8*)&pBuf[iCount],(uint8*)&temp,sizeof(uint16));
	iCount += sizeof(uint16);
	pBuf[iCount++] = pPara->dataLen;
	memcpy((uint8*)&pBuf[iCount],(uint8*)&pPara->data[0],pPara->dataLen);
	iCount += pPara->dataLen;
	crc = modBus_CRC16(pBuf,iCount);
	memcpy((uint8*)&pBuf[iCount],(uint8*)&crc,sizeof(uint16));
	iCount += sizeof(uint16);

	/*data Len*/
	*ucDataLen = iCount;	

	/*分析:zy协议回复数据无地址分析,故仅根据数据长度区分*/
	memset((uint8*)&pPara->data[0],0,pPara->dataLen);
	pPara->dataLen = 0;

	return true;
}

/*
** zy PackageFrame
*/
bool zy_PackageFrame(void* pPara,void* pBuf1,uint16* dataLen){
	ZY_Charger* zy_Charger = (ZY_Charger*)pPara;
	uint8* pBuf = pBuf1;

	if(pPara == null || pBuf1 == null || dataLen == null){
		return false;
	}

	switch(zy_Charger->funCode){
		case get_ZY_Charger_FunCode:/*功能:允许用户获得系统信息*/
			return zy_PackageFrame_GetInfo(pPara, pBuf, dataLen);
		case set_ZY_Charger_FunCode:/*功能:允许用户发送参数给系统*/
			return zy_PackageFrame_SetInfo(pPara, pBuf, dataLen);
	}

	return false;
}

/*
** zy Parse Frame Get Info
*/
bool zy_ParseFrame_GetInfo(ZY_Charger* pPara,uint8* pBuf,uint16 ucDataLen){
	uint8 iCount = 0;
	uint16 crc = 0;
	uint16 get_Crc = 0;
	uint8 dataItemLen = 0;

	/*parse*/
	if(pBuf[iCount++] != pPara->addr){
		return false;
	}
	
	if(pBuf[iCount++] != get_ZY_Charger_FunCode){
		return false;
	}
	dataItemLen = pBuf[iCount++];
	/*chk frame len*/
	if((dataItemLen + iCount + sizeof(uint16)) > ucDataLen){
		return false;
	}
	/*chk crc*/
	crc = modBus_CRC16(pBuf, sizeof(uint8) + sizeof(uint8) + sizeof(uint8) + dataItemLen);
	memcpy((uint8*)&get_Crc,(uint8*)&pBuf[sizeof(uint8) + sizeof(uint8) + sizeof(uint8) + dataItemLen],sizeof(uint16));
	if(crc != get_Crc){
		return false;
	}

	/*copy data len*/
	pPara->dataLen = dataItemLen;
	/*copy data item*/
	memcpy((uint8*)&pPara->data[0],(uint8*)&pBuf[iCount],dataItemLen);

	return true;
}

/*
** zy Parse Frame Set Info
*/
bool zy_PasreFrame_SetInfo(ZY_Charger* pPara,uint8* pBuf,uint16 ucDataLen){
	uint8 iCount = 0;
	uint16 crc = 0;
	uint16 get_Crc = 0;
	uint16 regAddr = 0;
	uint16 temp = 0;
	uint16 regNum = 0;

	/*parse*/
	if(pBuf[iCount++] != pPara->addr){
		return false;
	}

	if(pBuf[iCount++] != set_ZY_Charger_FunCode){
		return false;
	}	

	memcpy((uint8*)&temp,(uint8*)&pBuf[iCount],sizeof(uint16));
	regAddr = little_Big_Endian__Exchange(temp);
	iCount += sizeof(uint16);
	if(regAddr != pPara->registerAddr){
		return false;
	}

	memcpy((uint8*)&temp,(uint8*)&pBuf[iCount],sizeof(uint16));
	regNum = little_Big_Endian__Exchange(temp);
	iCount += sizeof(uint16);
	if(regNum != pPara->registerNum){
		return false;
	}

	/*chk crc*/
	crc = modBus_CRC16(pBuf, sizeof(uint8) + sizeof(uint8) + sizeof(uint16) + sizeof(uint16));
	memcpy((uint8*)&get_Crc,(uint8*)&pBuf[sizeof(uint8) + sizeof(uint8) +  sizeof(uint16) + sizeof(uint16)],sizeof(uint16));
	if(crc != get_Crc){
		return false;
	}

	return true;
}

/*
** zy ParseFrame
*/
bool zy_ParseFrame(void* pPara,void* pBuf1,uint16 dataLen){
	ZY_Charger* zy_Charger = (ZY_Charger*)pPara;
	uint8* pBuf = pBuf1;

	if(pPara == null || pBuf1 == null || dataLen == 0){
		return false;
	}

	switch(zy_Charger->funCode){
		case get_ZY_Charger_FunCode:
			return zy_ParseFrame_GetInfo(pPara, pBuf, dataLen);
		case set_ZY_Charger_FunCode:		
			return zy_PasreFrame_SetInfo(pPara, pBuf, dataLen);
	}

	return false;
}

