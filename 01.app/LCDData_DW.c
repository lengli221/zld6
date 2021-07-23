#include "includes.h"

static LCD_SCREENID lcdScreenIdSet;
static LCD_SCREENID lcdScreenGetCurId;

static LCDTime lcdTime;
static LCDMainPage lcdMainPage;
static LCDAdminPage lcdAdminPage;
static LCDSysParam lcdSysParamShow;
static LCDSysParam lcdSysParamSet;
static LCDRecordData lcdRecordData;
static LCDRecordStateData lcdRecordStateData;
static uint16 lcdReserved;
static LCD_ClearRecord lcdClearRecord;
static LCD_OneAppointPage lcdOneAppoint;
static LCD_SoftVersion lcdSoftVersion;

#if Modify_PW_190822 == 1
static LCD_ModifyPw lcdModifyPwAscii;
static LCD_ModifyPw lcdModifyPwShow;
#endif

#if Modify_Key_190826 == 1
static LCD_SysModifyKey lcdSysModifyKey;
static LCD_SysModifyKey lcdSysModifyKeyBase;
static LCD_ModifyCard lcdModifyCard;
static LCD_ModifyCard lcdModifyCardBase;
static LCD_ModifyCardChange lcdModifyCardChange;
static LCD_CancelCardKey lcdCancelCardKey;
static LCD_KeyModeChoice lcdKeyModeChoice;
#endif

LCD_CTRLFRAME LCD_CtrlAddrSheets[]=
{
	{0x03,&lcdScreenGetCurId.id[0]},
	{0x04,&lcdScreenGetCurId.id[1]},
};
uint8 const  LCD_CtrlAddrSheetsNum = sizeof(LCD_CtrlAddrSheets) / sizeof(LCD_CTRLFRAME);

uint16 lcdPageID = 0;/*迪文二代屏获取页码ID*/

LCD_DATAFRAME LCD_DataAddrSheets[]=
{
	/*迪文二代屏获取页码ID*/
	{LCD_ADDR_ReadId,&lcdPageID},
		
	{LCD_WriteTimeAddr,&lcdTime.year[0]},
	{LCD_WriteTimeAddr+1,&lcdTime.year[1]},
	{LCD_WriteTimeAddr+2,&lcdTime.month},
	{LCD_WriteTimeAddr+3,&lcdTime.day},
	{LCD_WriteTimeAddr+4,&lcdTime.hour},
	{LCD_WriteTimeAddr+5,&lcdTime.min},
	{LCD_WriteTimeAddr+6,&lcdTime.sec},

	{LCD_WriteStateIconAddr,&lcdMainPage.stateIcon[0]},
	{LCD_WriteStateIconAddr+1,&lcdMainPage.stateIcon[1]},
	{LCD_WriteStateIconAddr+2,&lcdMainPage.stateIcon[2]},
	{LCD_WriteStateIconAddr+3,&lcdMainPage.stateIcon[3]},
	{LCD_WriteStateIconAddr+4,&lcdMainPage.stateIcon[4]},
	{LCD_WriteStateIconAddr+5,&lcdMainPage.stateIcon[5]},
	{LCD_WriteStateIconAddr+6,&lcdMainPage.stateIcon[6]},
	{LCD_WriteStateIconAddr+7,&lcdMainPage.stateIcon[7]},
	{LCD_WriteStateIconAddr+8,&lcdMainPage.stateIcon[8]},
	{LCD_WriteStateIconAddr+9,&lcdMainPage.stateIcon[9]},
	{LCD_WriteStateIconAddr+10,&lcdMainPage.stateIcon[10]},
	{LCD_WriteStateIconAddr+11,&lcdMainPage.stateIcon[11]},

	{LCD_WriteNumAddr,&lcdMainPage.idleNum},
	{LCD_WriteNumAddr+1,&lcdMainPage.ingNum},
	{LCD_WriteNumAddr+2,&lcdMainPage.WaterChkIcon},
	{LCD_WriteNumAddr+3,&lcdMainPage.cardComErr},

	{LCD_WriteAdminPwShowAddr,&lcdAdminPage.show[0]},
	{LCD_WriteAdminPwShowAddr+1,&lcdAdminPage.show[1]},
	{LCD_WriteAdminPwShowAddr+2,&lcdAdminPage.show[2]},
	{LCD_WriteAdminPwShowAddr+3,&lcdAdminPage.show[3]},

	{LCD_WriteAdminPwAsciiAddr,&lcdAdminPage.Ascii[0]},
	{LCD_WriteAdminPwAsciiAddr+1,&lcdAdminPage.Ascii[1]},
	{LCD_WriteAdminPwAsciiAddr+2,&lcdAdminPage.Ascii[2]},
	{LCD_WriteAdminPwAsciiAddr+3,&lcdAdminPage.Ascii[3]},

	{LCD_WriteAdminPwIconAddr,&lcdAdminPage.icon},
	{LCD_WriteSysSetTimeAddr,&lcdSysParamShow.time.year[0]},
	{LCD_WriteSysSetTimeAddr+1,&lcdSysParamShow.time.year[1]},
	{LCD_WriteSysSetTimeAddr+2,&lcdReserved},
	{LCD_WriteSysSetTimeAddr+3,&lcdSysParamShow.time.month},
	{LCD_WriteSysSetTimeAddr+4,&lcdReserved},
	{LCD_WriteSysSetTimeAddr+5,&lcdSysParamShow.time.day},
	{LCD_WriteSysSetTimeAddr+6,&lcdReserved},
	{LCD_WriteSysSetTimeAddr+7,&lcdSysParamShow.time.hour},
	{LCD_WriteSysSetTimeAddr+8,&lcdReserved},
	{LCD_WriteSysSetTimeAddr+9,&lcdSysParamShow.time.min},
	{LCD_WriteSysSetTimeAddr+10,&lcdReserved},
	{LCD_WriteSysSetTimeAddr+11,&lcdSysParamShow.time.sec},
	{LCD_WriteSysSetTimeAddr+12,&lcdReserved},
	
	{LCD_WriteConsumeWayAddr,&lcdSysParamShow.consumeWay},
	{LCD_WriteConsumeWayAddr+1,&lcdSysParamShow.smokeCtr},

	{LCD_WriteConsumeIconAddr,&lcdSysParamShow.consumeIcon[0]},
	{LCD_WriteConsumeIconAddr+1,&lcdSysParamShow.consumeIcon[1]},
	{LCD_WriteConsumeIconAddr+2,&lcdSysParamShow.smokeIcon[0]},
	{LCD_WriteConsumeIconAddr+3,&lcdSysParamShow.smokeIcon[1]},

	{LCD_WriteSingleConsumeAddr,&lcdSysParamShow.singleConsum},
	
	{LCD_WriteSingleConsumeAddr+1,&lcdSysParamShow.deviceId[0]},
	{LCD_WriteSingleConsumeAddr+2,&lcdSysParamShow.deviceId[1]},
	{LCD_WriteSingleConsumeAddr+3,&lcdSysParamShow.deviceId[2]},
	{LCD_WriteSingleConsumeAddr+4,&lcdSysParamShow.deviceId[3]},
	{LCD_WriteSingleConsumeAddr+5,&lcdSysParamShow.deviceId[4]},
	{LCD_WriteSingleConsumeAddr+6,&lcdSysParamShow.deviceId[5]},
	{LCD_WriteSingleConsumeAddr+7,&lcdSysParamShow.deviceId[6]},

	{LCD_WriteFullSetTimeAddr,&lcdSysParamShow.fullSetTime},
	#if Modify_Key_190826 == 1
	{LCD_WriteFullSetTimeAddr+1,&lcdReserved},
	{LCD_WriteFullSetTimeAddr+2,&lcdSysModifyKey.cabinet[0]},
	{LCD_WriteFullSetTimeAddr+3,&lcdSysModifyKey.cabinet[1]},
	{LCD_WriteFullSetTimeAddr+4,&lcdSysModifyKey.cabinet[2]},
	{LCD_WriteFullSetTimeAddr+5,&lcdSysModifyKey.cabinet[3]},
	{LCD_WriteFullSetTimeAddr+6,&lcdSysModifyKey.cabinet[4]},
	#endif

	{LCD_WriteRecordStateAddr,&lcdRecordStateData.curpage[0]},
	{LCD_WriteRecordStateAddr+1,&lcdRecordStateData.curpage[1]},
	{LCD_WriteRecordStateAddr+2,&lcdRecordStateData.curpage[2]},
	{LCD_WriteRecordStateAddr+3,&lcdRecordStateData.curpage[3]},
	{LCD_WriteRecordStateAddr+4,&lcdRecordStateData.totalpage[0]},
	{LCD_WriteRecordStateAddr+5,&lcdRecordStateData.totalpage[1]},
	{LCD_WriteRecordStateAddr+6,&lcdRecordStateData.totalpage[2]},
	{LCD_WriteRecordStateAddr+7,&lcdRecordStateData.totalpage[3]},
	{LCD_WriteRecordStateAddr+8,&lcdRecordStateData.totalpage[4]},
	{LCD_WriteRecordStateAddr+9,&lcdRecordStateData.totalpage[5]},
	{LCD_WriteRecordStateAddr+10,&lcdRecordStateData.lastbtn},
	{LCD_WriteRecordStateAddr+11,&lcdRecordStateData.nextbtn},

	{LCD_WriteClearRecordAddr,&lcdClearRecord.btnOk},

	{LCD_WriteClearOneBtnOpenAddr,&lcdSysParamShow.oneBtnOpenDoor},

	{LCD_WriteAppointDoorIdAddr,&lcdOneAppoint.appointId},
	{LCD_WriteAppointDoorIdAddr+1,&lcdOneAppoint.ok},

	{LCD_WriteSoftVersionAddr,&lcdSoftVersion.ctr},
	
	#if Modify_PW_190822 == 1
	{LCD_WriteModifyPwAsciiAddr,&lcdModifyPwAscii.newPw[0]},
	{LCD_WriteModifyPwAsciiAddr+1,&lcdModifyPwAscii.newPw[1]},
	{LCD_WriteModifyPwAsciiAddr+2,&lcdModifyPwAscii.newPw[2]},
	{LCD_WriteModifyPwAsciiAddr+3,&lcdModifyPwAscii.newPw[3]},
	{LCD_WriteModifyPwAsciiAddr+4,&lcdModifyPwAscii.rePw[0]},
	{LCD_WriteModifyPwAsciiAddr+5,&lcdModifyPwAscii.rePw[1]},
	{LCD_WriteModifyPwAsciiAddr+6,&lcdModifyPwAscii.rePw[2]},
	{LCD_WriteModifyPwAsciiAddr+7,&lcdModifyPwAscii.rePw[3]},
	{LCD_WriteModifyPwAsciiAddr+8,&lcdModifyPwAscii.ok},
	{LCD_WriteModifyPwAsciiAddr+9,&lcdReserved},
	#endif

	#if Modify_Key_190826 == 1
	{LCD_WriteModifyKeyAddr,&lcdModifyCard.initKey[0]},
	{LCD_WriteModifyKeyAddr+1,&lcdModifyCard.initKey[1]},
	{LCD_WriteModifyKeyAddr+2,&lcdModifyCard.initKey[2]},
	{LCD_WriteModifyKeyAddr+3,&lcdModifyCard.initKey[3]},
	{LCD_WriteModifyKeyAddr+4,&lcdModifyCard.initKey[4]},
	{LCD_WriteModifyKeyAddr+5,&lcdModifyCard.newKey[0]},
	{LCD_WriteModifyKeyAddr+6,&lcdModifyCard.newKey[1]},
	{LCD_WriteModifyKeyAddr+7,&lcdModifyCard.newKey[2]},
	{LCD_WriteModifyKeyAddr+8,&lcdModifyCard.newKey[3]},
	{LCD_WriteModifyKeyAddr+9,&lcdModifyCard.newKey[4]},
	{LCD_WriteModifyKeyAddr+10,&lcdModifyCard.way},
	{LCD_WriteModifyKeyAddr+11,&lcdModifyCard.ok},
	{LCD_WriteModifyKeyAddr+12,&lcdModifyCard.icon},

	{LCD_WriteCancelModifyKeyAddr,&lcdCancelCardKey.step},

	{LCD_WriteKeyModeChoiceAddr,&lcdKeyModeChoice.init},
	{LCD_WriteKeyModeChoiceAddr+1,&lcdKeyModeChoice.re},
	#endif
};
uint16 const LCD_DataAddrSheetsNum = sizeof(LCD_DataAddrSheets) / sizeof(LCD_DATAFRAME);

uint16 LCD_GetScreenID(void)
{
	uint16 id = 0;
	id = lcdScreenGetCurId.id[0];
	id <<= 8;
	id |= lcdScreenGetCurId.id[1];
	return lcdPageID;
}

bool LCD_SetScreenID(uint16 screenId)
{
	lcdScreenIdSet.id[0] = screenId>>8;
	lcdScreenIdSet.id[1] = screenId&0xFF;
	return true;
}

void LCD_GetScreenIdSet(uint8*pdata)
{
	/*二代迪文屏增加识别字段:0x5A 0x01*/
	*pdata++ = 0x5A;
	*pdata++ = 0x01; 
	*pdata++ = lcdScreenIdSet.id[0];
	*pdata++ = lcdScreenIdSet.id[1];
}

bool LCD_JumpScreen(void *pData, uint8 dev)
{
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if (pData == null)
		return false;
	
	st_FramePara->iAddress = LCD_ADDR_WriteId;										
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_ADDR_Len_WriteId;
	
	LCD_GetScreenIdSet(st_FramePara->ucDataSpace);

	return true;
}

bool LCD_ReadScreenID(void *pData, uint8 dev)
{
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	
	if(pData == null)
		return false;
	
	st_FramePara->iAddress = LCD_ADDR_ReadId;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_ADDR_Len_ReadId;

	return true;
}

bool LCD_SetTime(Common_Time time)
{
	lcdTime.year[0] = 0x3230;
	lcdTime.year[1] = (((time.iYear/10)+'0')<<8) + ((time.iYear%10)+'0');
	lcdTime.month = (((time.ucMonth/10)+'0')<<8) + ((time.ucMonth%10)+'0');
	lcdTime.day = (((time.ucDay/10)+'0')<<8) + ((time.ucDay%10)+'0');
	lcdTime.hour = (((time.ucHour/10)+'0')<<8) + ((time.ucHour%10)+'0');
	lcdTime.min = (((time.ucMin/10)+'0')<<8) + ((time.ucMin%10)+'0');
	lcdTime.sec = (((time.ucSec/10)+'0')<<8) + ((time.ucSec%10)+'0');
	
	return true;
}

bool LCD_WriteTime(void *pData,uint8 dev)
{//说明:写迪文屏上系统时间 地址:0x0100
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteTimeAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_TimeAddr;
	LCD_GetData(st_FramePara);
	return true;
}

void LCD_UpdateStateIcon(void)
{
	uint8 i=0;
	uint8 ret=0;
	DoorSysParam *lcdDoorSysParam = GetDoorSysParam();

	for(i=0;i<ALLDOORNUM;i++)
	{
		switch(lcdDoorSysParam->doorState[i])
		{
			case doorIdle:
				ret = IdleIcon;
				break;
			case BatFull:
				ret = FullIcon;
				break;
			case BatOnline:/*电池接入*/
				ret = BatOnlineIcon;
				break;
			case BatCharging:
				ret = IngIcon;
				break;
			case BatTurnAround:
			case BatOverTempP:
			case BatOverVolP:
			case batErr:/*电池故障--除电池反接,过温保护,过压保护*/
			case chargerModuleComErr:/*充电器模块通讯异常*/
			case doorErr:
				ret = ErrIcon;
				break;
			case doorOptOverTime:
			case doorOptOverTimeMid:
				ret = OverTimeIcon;
				break;
			default:
				
				break;
		}
		lcdMainPage.stateIcon[i] = ret;
	}
}

bool LCD_WriteStateIcon(void *pData,uint8 dev)
{//说明:写状态图标 地址:0x0110
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteStateIconAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_StateIconAddr;
	LCD_GetData(st_FramePara);
	return true;
}

void LCD_UpdateStateNum(void)
{
	lcdMainPage.idleNum = DoorSysGetIdleDoorNum();
	lcdMainPage.ingNum = DoorSysGetUsingBatNum();
	/*说明:增加水位检测Icon*/
	lcdMainPage.WaterChkIcon = GetDoorSysWaterChk();
	/*说明:增加烟感,急停使用同一图标显示,急停显示优先级最高*/
	if(GetSysSmokeFlag() == true)
	{
		lcdMainPage.WaterChkIcon = 0x0003;//烟感异常
	}

	if(GetSysEPOFlag() == true)
	{
		lcdMainPage.WaterChkIcon = 0x0002;//急停异常
	}

	lcdMainPage.cardComErr = GetDoorSysCardComErr();
}

bool LCD_WriteStateNum(void *pData,uint8 dev)
{//说明:写状态数 地址:0x0120
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteNumAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_NumAddr;
	LCD_GetData(st_FramePara);
	return true;
}

void LCD_ClearAdminPwShow(void)
{
	uint8 tmp0 = ' ';
	uint8 i=0;
	for(i=0;i<4;i++)
	{
		/*lcdAdminPage.show[i] = (uint16)tmp0<<8 +tmp0;*/
		lcdAdminPage.show[i] = ((uint16)tmp0<<8) +tmp0;
	}
}

void LCD_SetAdminPwShowXX(void)
{
	uint8  i=0;
	uint8 *piont=(uint8*)&lcdAdminPage.Ascii[0];
	uint8 *Dest= (uint8*)&lcdAdminPage.show[0];
	uint8  num[10];
	uint8  nu;

	for(i=0;i<8;i++)
	{
		*(Dest+i)=' ';
	}

	for(i=0;i<7;i++)
	{
		num[i]= *(piont+i);
	}

	for(i=0;i<4;i++)
	{
		nu=num[i*2+1];    
		num[i*2+1]=num[i*2];
		num[i*2]=nu;
	}

	for(i=0;i<8;i++)
	{
		if(num[i]==0xFF)
		{
			break;
		}
		else
		{
			*(Dest+i)='*';
		}
	}
	
	if(i<8)
	{
		for(;i<8;i++)
		{
			*(Dest+i)=' ';
		}
	}

	for(i=0;i<4;i++)
	{
		nu=*(Dest+i*2+1);
		*(Dest+i*2+1)= *(Dest+i*2);
		*(Dest+i*2)=nu;
	}
}

bool LCD_WriteAdminPwShow(void *pData,uint8 dev)
{//说明:写管理员密码界面显示 地址:0x0130
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteAdminPwShowAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_AdminPwShowAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

void LCD_ClearAdminPwAscii(void)
{
	/*
	memset((uint8*)&lcdAdminPage.Ascii[0],0x00,sizeof(lcdAdminPage.Ascii));
	*/
	uint8 tmp0 = ' ';
	uint8 i=0;
	for(i=0;i<4;i++)
	{
		/*lcdAdminPage.Ascii[i] = (uint16)tmp0<<8 +tmp0;*/
		lcdAdminPage.Ascii[i] = ((uint16)tmp0<<8) +tmp0;
	}
}

bool LCD_WriteAdminPwAscii(void *pData,uint8 dev)
{//说明:写管理员密码页面ASCII 地址:0x0140
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteAdminPwAsciiAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_AdminPwAsciiAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

void LCD_GetAdminPw(LCDAdminPage * adminPage)
{
	memcpy((uint8*)&adminPage->show[0],(uint8*)&lcdAdminPage.show[0],sizeof(LCDAdminPage));
}

bool LCD_GetBtnOk(void)
{
	uint8 i=0;
	uint8 ret = false;
	uint16 temp[4] = {0};
	for(i=0;i<4;i++)
	{
		if((uint8)((lcdAdminPage.Ascii[i]>>8)&0xFF) == 0xFF || (uint8)(lcdAdminPage.Ascii[i]&0xFF) == 0xFF)
		{
			ret = true;
			break;
		}
	}

	memset((uint8*)&temp[0],' '/*0x00*/,sizeof(temp));
	if(i==4)
	{
		if(0 != memcmp((uint8*)&lcdAdminPage.Ascii[0],(uint8*)&temp[0],sizeof(temp)))
		{
			ret = true;
		}
	}
	return(ret);
}

bool LCD_ReadAdminPwAscii(void *pData,uint8 dev)
{//说明:读管理员密码页面ASCII 地址:0x0140
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteAdminPwAsciiAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_AdminPwAsciiAddr;
	return(true);
}

void LCD_SetAdminPwIcon(uint16 icon)
{
	lcdAdminPage.icon = icon;
}

bool LCD_WriteAdminPwIcon(void *pData,uint8 dev)
{//说明:写管理员密码页面图片 地址:0x0150
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteAdminPwIconAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_AdminPwIconAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

void LCD_UpdateSysTimeShow(void)
{
	lcdSysParamShow.time.year[0] = lcdTime.year[0];
	lcdSysParamShow.time.year[1] = lcdTime.year[1];
	lcdSysParamShow.time.month = lcdTime.month;
	lcdSysParamShow.time.day = lcdTime.day;
	lcdSysParamShow.time.hour = lcdTime.hour;
	lcdSysParamShow.time.min = lcdTime.min;
	lcdSysParamShow.time.sec = lcdTime.sec;

	memcpy((uint8*)&lcdSysParamSet.time.hour,(uint8*)&lcdSysParamShow.time.hour,sizeof(LCDTime));
}

void LCD_UpdateSysSet(void)
{
	memcpy((uint8*)&lcdSysParamSet.time.hour,(uint8*)&lcdSysParamShow.time.hour,sizeof(LCDTime));
}

void LCD_GetSysSetShow(LCDTime *time)
{
	memcpy((uint8*)&time->hour,(uint8*)&lcdSysParamShow.time.hour,sizeof(LCDTime));
}

bool LCD_IsUpdateTime(void)
{
	if(0 != memcmp((uint8*)&lcdSysParamSet.time.hour,(uint8*)&lcdSysParamShow.time.hour,sizeof(LCDTime)))
	{
		return(true);
	}
	return(false);
}

bool LCD_WriteSysParamTime(void *pData,uint8 dev)
{//说明:写系统页面参数时间 地址:0x0160
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteSysSetTimeAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_SysSetTimeAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadSysParamTime(void *pData,uint8 dev)
{//说明:写系统页面参数时间 地址:0x0160
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteSysSetTimeAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_SysSetTimeAddr;
	return true;	
}

void LCD_ClearConsumeWay(void)
{
	lcdSysParamShow.consumeWay = 0x00;
	/*增加烟感逻辑控制*/
	lcdSysParamShow.smokeCtr = 0x00;
}

void Lcd_SetSmokeCtr(uint16 smokeCtr)
{
	lcdSysParamSet.smokeCtr = smokeCtr;
}

bool Lcd_IsUpdateSmokeCtr(void)
{
	bool ret = false;
	if(lcdSysParamShow.smokeCtr == Open_SmokeCtr || lcdSysParamShow.smokeCtr == Close_SmokeCtr)
	{
		lcdSysParamSet.smokeCtr = lcdSysParamShow.smokeCtr;
		if(lcdSysParamSet.smokeCtr != param_GetSmokeCtr())
		{/*修改烟感控制方式*/
			param_SetSmokeCtr(lcdSysParamSet.smokeCtr);
			param_SaveSmokeCtr();
			LCD_UpdateSmokeCtrIcon(lcdSysParamSet.smokeCtr);
			ret = true;
		}
	}
	return(ret);
}

void LCD_SetConsumeWaySet(uint16 consumeWay)
{
	lcdSysParamSet.consumeWay = consumeWay;
}

bool LCD_IsUpdateConsumeWay(void)
{
	bool ret = false;
	if(lcdSysParamShow.consumeWay == Month_Card || lcdSysParamShow.consumeWay == Consumer_Card)
	{
		lcdSysParamSet.consumeWay = lcdSysParamShow.consumeWay;
		if(lcdSysParamSet.consumeWay != param_GetConsumeWay())
		{/*说明:存储消费方式*/
			/*相关说明:190829--目前暂未开发扣费卡,故暂时屏蔽消费卡选项*/
			lcdSysParamShow.consumeWay = Month_Card;
			param_SetConsumeWay(lcdSysParamSet.consumeWay);
			param_SaveConsumeWay();
			LCD_UpdateConsumeIcon(lcdSysParamSet.consumeWay);
			ret = true;
		}
	}
	return(ret);
}

bool LCD_WriteConsumeWay(void *pData,uint8 dev)
{
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteConsumeWayAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ConsumeWayAddr;
	LCD_GetData(st_FramePara);
	return true;
}

bool LCD_ReadConsumeWay(void *pData,uint8 dev)
{//说明:读消费方式 月卡--消费卡 地址:0x0170
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteConsumeWayAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_ConsumeWayAddr;
	return true;		
}

void LCD_UpdateSmokeCtrIcon(uint16 smokeCtr)
{
	switch(smokeCtr)
	{
		case Open_SmokeCtr:
			lcdSysParamShow.smokeIcon[0] = SelectedIcon;
			lcdSysParamShow.smokeIcon[1] = NoSelectedIcon;
			break;
		case Close_SmokeCtr:
			lcdSysParamShow.smokeIcon[0] = NoSelectedIcon;
			lcdSysParamShow.smokeIcon[1] = SelectedIcon;			
			break;
		default:
			
			break;
	}
}

void LCD_UpdateConsumeIcon(uint16 consumeWay)
{
	switch(consumeWay)
	{
		case Month_Card:
			lcdSysParamShow.consumeIcon[0] = SelectedIcon;
			lcdSysParamShow.consumeIcon[1] = NoSelectedIcon;
			break;
		case Consumer_Card:
			lcdSysParamShow.consumeIcon[0] = NoSelectedIcon;
			lcdSysParamShow.consumeIcon[1] = SelectedIcon;			
			break;
		default:
			
			break;
	}
}

bool LCD_WriteConsumeWayIcon(void *pData,uint8 dev)
{//说明:写消费图标 地址:0x0180
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteConsumeIconAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ConsumeIconAddr;
	LCD_GetData(st_FramePara);
	return true;		
}

void LCD_SetDeviceId(uint8*deviceId)
{
	memcpy((uint8*)&lcdSysParamShow.deviceId[0],(uint8*)&deviceId[0],DeviceAddr_MAX*sizeof(uint16));
}

void LCD_GetDeviceId(uint8*deviceId)
{
	memcpy((uint8*)&deviceId[0],(uint8*)&lcdSysParamShow.deviceId[0],DeviceAddr_MAX*sizeof(uint16));
}

bool LCD_IsDeviceIdChange(void)
{
	bool ret=false;
	
	uint8 tmp[DeviceAddr_MAX*sizeof(uint16)] = {0};
	
	param_GetDeviceId((uint8 *)&tmp[0]);
	
	if(0!= memcmp((uint8*)&lcdSysParamShow.deviceId[0],(uint8*)&tmp[0],DeviceAddr_MAX*sizeof(uint16)))
	{
		ret = true;
	}
	return(ret);
}

bool LCD_IsDeviceIdLegality(void)
{
	uint8 i=0;
	bool ret = false;
	uint8 tmp = 0xFF;
	uint8 *ptrDeviceId = (uint8*)&lcdSysParamShow.deviceId[0]; 

	for(i=0;i<DeviceAddr_MAX*sizeof(uint16);i++)
	{
		if(Compare_Data((uint8 *)&ptrDeviceId[0], (uint8 *)&tmp, sizeof(uint8)))
		{/*非法设备ID*/
			ret = true;
			break;
		}
		ptrDeviceId++;
		tmp = 0xFF;
	}
	return(ret);
}

void LCD_SetSingleConsume(uint16 singleConsume)
{
	lcdSysParamShow.singleConsum = singleConsume;
}

uint16 LCD_GetSingleConsume(void)
{
	return(lcdSysParamShow.singleConsum);
}

bool LCD_IsSingleConsumeChange(void)
{
	bool ret = false;
	if(lcdSysParamShow.singleConsum != param_GetSingleConsume())
	{
		ret = true;
	}
	return(ret);
}

bool LCD_ReadSingleConsume(void *pData,uint8 dev)
{//说明:写单次消费费用 地址:0x0190
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteSingleConsumeAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_SingleConsumeAddr;
	return true;		
}

bool LCD_WriteSingleConsume(void *pData,uint8 dev)
{//说明:写单次消费费用 地址:0x0190
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteSingleConsumeAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_SingleConsumeAddr;
	LCD_GetData(st_FramePara);
	return true;	
}


#if Modify_Key_190826 == 1
LCD_SysModifyKey LCD_GetSysModifyKey(void)
{
	return(lcdSysModifyKey);
}

void LCD_SetCabinetKey(ParamSysCabinet parSysCabinet)
{
	ByteConversionAscii((uint8 *)&parSysCabinet.key[0], (uint8 *)&lcdSysModifyKey.cabinet[0], sizeof(ParamSysCabinet));
	lcdSysModifyKey.cabinet[4] = 0x2020;
	memcpy((uint8*)&lcdSysModifyKeyBase.cabinet[0],(uint8 *)&lcdSysModifyKey.cabinet[0],sizeof(LCD_SysModifyKey));
}

bool LCD_IsKeyChange(void)
{
	bool ret = false;
	if(memcmp((uint8*)&lcdSysModifyKeyBase.cabinet[0],(uint8*)&lcdSysModifyKey.cabinet[0],sizeof(LCD_SysModifyKey)) != 0)
	{
		lcdSysModifyKeyBase = lcdSysModifyKey;
		ret=true;
	}
	return(ret);
}

bool LCD_IsKeyLegality(void)
{
	bool ret=false;
	uint8 i=0;

	for(i=0;i<4;i++)
	{
		if((((lcdSysModifyKey.cabinet[i]&0xFF00)>>8) == 0xFF) || (((uint8)(lcdSysModifyKey.cabinet[i]&0x00FF)) == 0xFF) )
		{
			ret = false;
			goto endCabinetFlag;
		}
	}
	
	if((((lcdSysModifyKey.cabinet[3]&0xFF00)>>8) != 0xFF) &&(((uint8)(lcdSysModifyKey.cabinet[3]&0x00FF)) != 0xFF) && lcdSysModifyKey.cabinet[4] == 0xFFFF)
	{
		ret=true;
	}
endCabinetFlag:
	return(ret);
}
#endif

void LCD_SetFullSetTime(uint16 fullSetTime)
{
	lcdSysParamShow.fullSetTime = fullSetTime;
}

uint16 LCD_GetFullSetTime(void)
{
	return(lcdSysParamShow.fullSetTime);
}

bool LCD_IsFullSetTimeChange(void)
{
	bool ret = false;
	if(lcdSysParamShow.fullSetTime != param_GetBatingConversionBatFull())
	{
		ret = true;
	}
	return(ret);
}

bool LCD_ReadFullSetTime(void *pData,uint8 dev)
{//说明:读满电时间 地址:0x01A0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteFullSetTimeAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_FullSetTimeAddr;
	return true;	
}

bool LCD_WriteFullSetTime(void *pData,uint8 dev)
{//说明:写满电时间 地址:0x01A0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteFullSetTimeAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_FullSetTimeAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

void LCD_ClearRecordData(void)
{
	memset((uint8*)&lcdRecordData.no[0],0x00,sizeof(LCDRecordData));
}

void LCD_SetRecordData(Charge_Record *record,uint16 no)
{
	uint8 i=0;
	uint8 tmp0 = 0;
	static char consumeDescribe[2][6] = {{"月卡"},{"扣费"}};	
	static char detailsDescribe[11][20] = {{"开空仓门故障"},{"开满仓门故障"},{"正常存电池"},{"未存入电池"},
											{"存入异常电池"},{"超时存入正常电池"},{"超时存入异常电池"},{"未取满电"},{"正常取电池"},{"无空仓"},{"无满仓"}};
	/*static*/char detailsDescribeTmp[20] = {"  "};
	uint8 doorId = 0;
	
	memset((uint8*)&lcdRecordData.no[0],0x00,sizeof(LCDRecordData));
	uint32ToAscii(no, 0, (uint16 *)&lcdRecordData.no[0]);

	for(i=0;i<sizeof(record->cardNo)/sizeof(uint16)/*说明:目前量必达卡号只有8位4字节*/;i++)
	{
		tmp0 = record->cardNo[i]>>4;
		A_FConvertToAscii((uint8 *)&tmp0);
		lcdRecordData.cardNo[i] = (tmp0+'0')<<8;
		tmp0 = record->cardNo[i]&0x0F;
		A_FConvertToAscii((uint8 *)&tmp0);
		lcdRecordData.cardNo[i] += tmp0 + '0';
	}

	lcdRecordData.sTime[0] = 0x3230;//说明:将数字转换成相应的字符传送至迪文屏
	lcdRecordData.sTime[1] = (((record->startTime.iYear/10)+'0')<<8) + ((record->startTime.iYear%10)+'0');
	lcdRecordData.sTime[2] = (('/')<<8) + ((record->startTime.ucMonth/10)+'0');
	lcdRecordData.sTime[3] = (((record->startTime.ucMonth%10)+'0')<<8) + '/';
	lcdRecordData.sTime[4] = (((record->startTime.ucDay/10)+'0')<<8) + ((record->startTime.ucDay%10)+'0');
	lcdRecordData.sTime[5] = ((' ')<<8) + ((record->startTime.ucHour/10)+'0');
	lcdRecordData.sTime[6] = (((record->startTime.ucHour%10)+'0')<<8) + ':';
	lcdRecordData.sTime[7] = (((record->startTime.ucMin/10)+'0')<<8) + ((record->startTime.ucMin%10)+'0');

	lcdRecordData.eTime[0] = 0x3230;
	lcdRecordData.eTime[1] = (((record->stopTime.iYear/10)+'0')<<8) + ((record->stopTime.iYear%10)+'0');
	lcdRecordData.eTime[2] = (('/')<<8) + ((record->startTime.ucMonth/10)+'0');
	lcdRecordData.eTime[3] = (((record->stopTime.ucMonth%10)+'0')<<8) + '/';
	lcdRecordData.eTime[4] = (((record->stopTime.ucDay/10)+'0')<<8) + ((record->stopTime.ucDay%10)+'0');
	lcdRecordData.eTime[5] = ((' ')<<8) + ((record->stopTime.ucHour/10)+'0');
	lcdRecordData.eTime[6] = (((record->stopTime.ucHour%10)+'0')<<8) + ':';
	lcdRecordData.eTime[7] = (((record->stopTime.ucMin/10)+'0')<<8) + ((record->stopTime.ucMin%10)+'0');

	/*仓门ID*/
	/*
	lcdRecordData.doorId = 0x3030;
	*/
	doorId = record->doorId + 0x01;
	uint32ToAscii(doorId/*record->doorId*/, 0, (uint16 *)&lcdRecordData.doorId);

	/*消费类型*/
	switch(record->consumeWay)
	{
		case Month_Card:
			/*
			memcpy((uint8*)&lcdRecordData.consumeWay[0],(uint8*)&consumeDescribe[0][0],sizeof(lcdRecordData.consumeWay));
			*/
			for(i=0;i<sizeof(lcdRecordData.consumeWay)/sizeof(uint16);i++)
			{
				lcdRecordData.consumeWay[i] = (*(&consumeDescribe[0][0]+2*i))<<8;
				lcdRecordData.consumeWay[i] += (*(&consumeDescribe[0][0]+2*i+1));
			}
			break;
		case Consumer_Card:
			/*
			memcpy((uint8*)&lcdRecordData.consumeWay[0],(uint8*)&consumeDescribe[1][0],sizeof(lcdRecordData.consumeWay));
			*/
			for(i=0;i<sizeof(lcdRecordData.consumeWay)/sizeof(uint16);i++)
			{
				lcdRecordData.consumeWay[i] = (*(&consumeDescribe[1][0]+2*i))<<8;
				lcdRecordData.consumeWay[i] += (*(&consumeDescribe[1][0]+2*i+1));
			}
			break;
		default:
			
			break;
	}
	
	/*记录详情*/
	switch(record->state)
	{
		case OpenDoorIdErr:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[0][0],sizeof(detailsDescribeTmp));
			break;
		case OpenFullErr:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[1][0],sizeof(detailsDescribeTmp));
			break;
		case Normal_SaveBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[2][0],sizeof(detailsDescribeTmp));
			break;
		case No_SaveBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[3][0],sizeof(detailsDescribeTmp));
			break;
		case SaveAbnormalBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[4][0],sizeof(detailsDescribeTmp));
			break;
		case OverTimeSaveNormalBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[5][0],sizeof(detailsDescribeTmp));
			break;
		case OverTimeSaveAbnormalBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[6][0],sizeof(detailsDescribeTmp));
			break;	
		case CloseDoorNoTakeBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[7][0],sizeof(detailsDescribeTmp));
			break;
		case CloseDoorTakeBat:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[8][0],sizeof(detailsDescribeTmp));
			break;
		case NoIdleDoor:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[9][0],sizeof(detailsDescribeTmp));
			break;
		case NoFullDoor:
			memcpy((uint8*)&detailsDescribeTmp[0],(uint8*)&detailsDescribe[10][0],sizeof(detailsDescribeTmp));
			break;
	}
	
	for(i=0;i<sizeof(lcdRecordData.analysis)/sizeof(uint16);i++)
	{
		lcdRecordData.analysis[i] = (*(&detailsDescribeTmp[0]+2*i))<<8;
		lcdRecordData.analysis[i] += (*(&detailsDescribeTmp[0]+2*i+1));
	}
}

void LCD_GetRecordData(uint8*pdata)
{
	uint8 len=0;
	uint8 i=0;
	uint16 *ptr = (uint16*)&lcdRecordData.no[0];
	len = sizeof(LCDRecordData)/sizeof(uint16);
	for(i=0;i<len;i++)
	{
		*pdata++ = *ptr >> 8;
		*pdata++ = (*ptr++) & 0xff;
	}
}

bool LCD_WriteRecordData(void *pData,uint8 dev)
{//说明:写充电记录 地址:0x3000 
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteRecordAddr+(dev*0x100);
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_RecordAddr;
	LCD_GetRecordData(st_FramePara->ucDataSpace);
	return true;	
}

void LCD_SetRecordState(uint16 cur,uint16 total,uint16 lastbtn,uint16 nextbtn)
{
	memset((uint8*)&lcdRecordStateData.curpage[0],0x00,sizeof(LCDRecordStateData));
	uint32ToAscii(cur,0,(uint16*)&lcdRecordStateData.curpage[0]);
	uint32ToAscii(total,0,(uint16*)&lcdRecordStateData.totalpage[0]);
	lcdRecordStateData.lastbtn = lastbtn;
	lcdRecordStateData.nextbtn = nextbtn;
}

void LCD_GetRecordBtn(uint16 *last,uint16 *next)
{
	*last = lcdRecordStateData.lastbtn;
	*next = lcdRecordStateData.nextbtn;
}

void LCD_GetRecordState(uint8 *pdata)
{
	uint8 len=0;
	uint8 i=0;
	uint16 *ptr = (uint16 *)&lcdRecordStateData.curpage[0];
	len = sizeof(LCDRecordStateData)/sizeof(uint16);
	for(i = 0; i < len ; i++)
	{
		*pdata++ = *ptr >> 8;
		*pdata++ = (*ptr++) & 0xff;
	}
}

bool LCD_WriteRecordState(void *pData,uint8 dev)
{//说明:写充电记录 控件值 地址:0x01B0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteRecordStateAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_RecordStateAddr;
    	LCD_GetRecordState(st_FramePara->ucDataSpace);
	return true;	
}

bool LCD_ReadRecordState(void *pData,uint8 dev)
{//说明:读充电记录 控件值 地址:0x01B0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteRecordStateAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_RecordStateAddr;
	return true;	
}

void LCD_ClearClearRecordBtnOk(void)
{
	lcdClearRecord.btnOk = 0;
}

uint16 LCD_GetClearRecordBtnOk(void)
{
	return(lcdClearRecord.btnOk);
}

bool LCD_WriteClearRecord(void *pData,uint8 dev)
{//说明:写清历史充电记录 地址:0x01C0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteClearRecordAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ClearRecordAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadClearRecord(void *pData,uint8 dev)
{//说明:读清历史充电记录 地址:0x01C0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteClearRecordAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_ClearRecordAddr;
	return true;	
}

void LCD_ClearClearOneBtnOpenDoor(void)
{
	lcdSysParamShow.oneBtnOpenDoor = 0;
}

uint16 LCD_GetClearOneBtnOpenDoor(void)
{
	return(lcdSysParamShow.oneBtnOpenDoor);
}

bool LCD_WriteClearOneBtnOpenDoor(void *pData,uint8 dev)
{//说明:写一键开锁有门 地址:0x01D0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteClearOneBtnOpenAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ClearOneBtnOpenAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadClearOneBtnOpenDoor(void *pData,uint8 dev)
{//说明:读一键开锁有门 地址:0x01D0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteClearOneBtnOpenAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_ClearOneBtnOpenAddr;
	return true;	
}

void LCD_ClearAppointDoorId(void)
{
	memset((uint8*)&lcdOneAppoint.appointId,0x00,sizeof(LCD_OneAppointPage));	
	lcdOneAppoint.appointId = 0x01;
}

void LCD_GetAppointDoorId(LCD_OneAppointPage*  oneAppoint)
{
	memcpy((uint8*)&oneAppoint->appointId,(uint8*)&lcdOneAppoint.appointId,sizeof(LCD_OneAppointPage));	
}

/*
void LCD_SetAppointDoorId(LCD_OneAppointPage  oneAppoint)
{
	memcpy((uint8*)&lcdOneAppoint.appointId,(uint8*)&oneAppoint.appointId,sizeof(LCD_OneAppointPage));	
}
*/

bool LCD_WriteAppointDoorId(void *pData,uint8 dev)
{//说明:写开指定仓门 地址:0x01E0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteAppointDoorIdAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_AppointDoorIdAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadAppointDoorId(void *pData,uint8 dev)
{//说明:读开指定仓门 地址:0x01E0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteAppointDoorIdAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_AppointDoorIdAddr;
	return true;	
}

void LCD_SetSoftVersion(LCD_SoftVersion softVer)
{
	memcpy((uint8*)&lcdSoftVersion.ctr,(uint8*)&softVer.ctr,sizeof(LCD_SoftVersion));
}

bool LCD_WriteSoftVersion(void *pData,uint8 dev)
{//说明:写软件版本号 地址:0x01F0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteSoftVersionAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_SoftVersionAddr;
	LCD_GetData(st_FramePara);
	return true;		
}

#if Modify_PW_190822 == 1
void LCD_ClearModifyPwData(LCD_ModifyPw *modifyPw)
{
	uint8 tmp0 = ' ';
	uint8 i=0;
	uint16 *ptr_Pw = (uint16*)&modifyPw->newPw[0];
 	for(i=0;i<8;i++)
	{

		/**ptr_Pw = (uint16)(tmp0<<8) + tmp0;*/
		*ptr_Pw = ((uint16)(tmp0<<8)) + tmp0;
		ptr_Pw++;
	}
}

void LCD_SetModifyPwShowData(LCD_ModifyPw modifyPw)
{
	memcpy((uint8*)&lcdModifyPwShow.newPw[0],(uint8*)&modifyPw.newPw,sizeof(LCD_ModifyPw));
}

void LCD_GetModifyPwShowData(uint8*pdata,uint8 dev)
{
	uint8 len=0;
	uint8 i=0;
	uint16 *ptr = (uint16*)&lcdModifyPwShow.newPw[0];
	len = sizeof(LCD_ModifyPw)/sizeof(uint16);
	for(i=0;i<len;i++)
	{
		*pdata++ = *ptr >> 8;
		*pdata++ = (*ptr++) & 0xff;
	}
}

bool LCD_WriteModifyPwShow(void *pData,uint8 dev)
{//说明:写修改密码显示以及Icon 地址:0x0420
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteModifyPwShowAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ModifyPwShowAddr;
	LCD_GetModifyPwShowData((uint8 *)&st_FramePara->ucDataSpace[0],dev);
	return true;	
}

void LCD_ChkPwAsciiModify(void)
{
	uint8 i=0;

	for(i=0;i<4;i++)
	{
		if(lcdModifyPwAscii.newPw[i] != (uint16)0x0000 && lcdModifyPwAscii.newPw[0] != (uint16)0xFFFF && lcdModifyPwAscii.newPw[i] != (uint16)0x2020)
		{
			memset((uint8*)&lcdModifyPwShow.newPw[0],'*',4*sizeof(uint16));
			break;
		}
	}

	for(i=0;i<4;i++)
	{
		if(lcdModifyPwAscii.rePw[i] != (uint16)0x0000 && lcdModifyPwAscii.rePw[0] != (uint16)0xFFFF && lcdModifyPwAscii.rePw[i] != (uint16)0x2020)
		{
			memset((uint8*)&lcdModifyPwShow.rePw[0],'*',4*sizeof(uint16));
			break;
		}
	}
}

void LCD_SetModifyPwAsciiData(LCD_ModifyPw modifyPw)
{
	memcpy((uint8*)&lcdModifyPwAscii.newPw[0],(uint8*)&modifyPw.newPw,sizeof(LCD_ModifyPw));
}

void LCD_GetModifyPwAsciiData(LCD_ModifyPw *modifyPw)
{
	memcpy((uint8*)&modifyPw->newPw[0],(uint8*)&lcdModifyPwAscii.newPw[0],sizeof(LCD_ModifyPw));
}

bool LCD_WriteModifyPwAscii(void *pData,uint8 dev)
{//说明:写修改密码ASCII和确认按钮  地址:0x0440
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteModifyPwAsciiAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ModifyPwAsciiAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadModifyPwAscii(void *pData,uint8 dev)
{//说明:读修改密码ASCII和确认按钮  地址:0x0440
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteModifyPwAsciiAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_ModifyPwAsciiAddr;
	return true;	
}
#endif

#if Modify_Key_190826 == 1
LCD_ModifyCardChange LCD_GetModifyCardChange(void)
{
	return(lcdModifyCardChange);
}

void LCD_SetModifyCardChange(LCD_ModifyCardChange lModifyCardChange)
{
	lcdModifyCardChange = lModifyCardChange;
}

void LCD_SetModifyCardKey(LCD_ModifyCard lModifyCardKey)
{
	lcdModifyCard = lModifyCardKey;
	/*
	lcdModifyCard.initKey[4] = 0x2020;
	lcdModifyCard.newKey[4] = 0x2020;
	*/
	lcdModifyCardBase = lcdModifyCard;
}

LCD_ModifyCard LCD_GetModifyCardKey(void)
{
	return(lcdModifyCard);
}

bool LCD_IsCardInitKeyChange(void)
{
	bool ret= false;
	if(0!=memcmp((uint8*)&lcdModifyCard.initKey[0],(uint8*)&lcdModifyCardBase.initKey[0],sizeof(lcdModifyCard.initKey)))
	{
		/*lcdModifyCard.initKey[4] = 0x2020;*/
		memcpy((uint8*)&lcdModifyCardBase.initKey[0],(uint8*)&lcdModifyCard.initKey[0],sizeof(lcdModifyCard.initKey));
		lcdModifyCardChange.initFlag = true;
		ret = true;
	}
	return(ret);
}

bool LCD_IsCardInitKeyLegality(void)
{
	bool ret = false;
	uint8 i=0;

	for(i=0;i<4;i++)
	{
		if((((lcdModifyCard.initKey[i]&0xFF00)>>8) == 0xFF) || (((uint8)(lcdModifyCard.initKey[i]&0x00FF)) == 0xFF))
		{
			ret = false;
			goto endInitFlag;
		}
	}
	
	if((((lcdModifyCard.initKey[3]&0xFF00)>>8) != 0xFF) &&(((uint8)(lcdModifyCard.initKey[3]&0x00FF)) != 0xFF) && lcdModifyCard.initKey[4] == 0xFFFF)
	{
		ret = true;
	}

endInitFlag:
	return(ret);
}

bool LCD_IsCardNewKeyChange(void)
{
	bool ret= false;
	if(0!=memcmp((uint8*)&lcdModifyCard.newKey[0],(uint8*)&lcdModifyCardBase.newKey[0],sizeof(lcdModifyCard.newKey)))
	{
		/*lcdModifyCard.newKey[4] = 0x2020;*/
		memcpy((uint8*)&lcdModifyCardBase.newKey[0],(uint8*)&lcdModifyCard.newKey[0],sizeof(lcdModifyCard.newKey));
		lcdModifyCardChange.newFlag = true;
		ret = true;
	}
	return(ret);
}

bool LCD_IsCardNewKeyLegality(void)
{
	bool ret = false;
	uint8 i=0;

	/*说明:防止初始化Key输入参数输入数据>=10&&<=18*/
	for(i=0;i<4;i++)
	{
		if((((lcdModifyCard.newKey[i]&0xFF00)>>8) == 0xFF) || (((uint8)(lcdModifyCard.newKey[i]&0x00FF)) == 0xFF) )
		{
			ret = false;
			goto endFlag;
		}
	}
	
	if((((lcdModifyCard.newKey[3]&0xFF00)>>8) != 0xFF) &&(((uint8)(lcdModifyCard.newKey[3]&0x00FF)) != 0xFF) && lcdModifyCard.newKey[4] == 0xFFFF)
	{
		ret = true;
	}
endFlag:	
	return(ret);
}

bool LCD_WriteModifyCardKey(void *pData,uint8 dev)
{//说明:写卡片密钥  地址:0x0460
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteModifyKeyAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_ModifyKeyAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadModifyCardKey(void *pData,uint8 dev)
{//说明:读卡片密钥  地址:0x0460
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteModifyKeyAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_ModifyKeyAddr;
	return true;	
}

void LCD_SetCancelModifyCardKey(LCD_CancelCardKey lCanelCardKey)
{
	lcdCancelCardKey = lCanelCardKey;
}

LCD_CancelCardKey LCD_GetCancelModifyCardKey(void)
{
	return(lcdCancelCardKey);
}

bool LCD_WriteCancelModifyCardKeyStep(void *pData,uint8 dev)
{//说明:写卡片密钥取消流程  地址:0x0480
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteCancelModifyKeyAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_CancelModifyKeyAddr;
	LCD_GetData(st_FramePara);
	return true;	
}

bool LCD_ReadCancelModifyCardKeyStep(void *pData,uint8 dev)
{//说明:读卡片密钥取消流程  地址:0x0480
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteCancelModifyKeyAddr;
	st_FramePara->ucFunCode = D_DP_DataRead; 
	st_FramePara->ucRegLen = LCD_Write_Len_CancelModifyKeyAddr;
	return true;	
}

LCD_KeyModeChoice LCD_GetKeyModeChoice(void)
{
	return(lcdKeyModeChoice);
}

void LCD_SetKeyModeChoice(LCD_KeyModeChoice lKeyModeChoice)
{
	lcdKeyModeChoice = lKeyModeChoice;
}

bool LCD_WriteKeyModeChoice(void *pData,uint8 dev)
{//说明:写显示密钥图标  地址:0x04A0
	DP_FramePara *st_FramePara = (DP_FramePara *)pData;
	if (pData == null)
		return false;
	st_FramePara->iAddress = LCD_WriteKeyModeChoiceAddr;
	st_FramePara->ucFunCode = D_DP_DataWirte; 
	st_FramePara->ucRegLen = LCD_Write_Len_KeyModeChoiceAddr;
	LCD_GetData(st_FramePara);
	return true;	
}
#endif

