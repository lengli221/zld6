#include "includes.h"

ParamSys paramSys;

#if Modify_PW_190822 == 1
ParamSysPw paramSysPw;
#endif

#if Modify_Key_190826 == 1
ParamSysCabinet paramSysCabinet;
#endif

ParamHS paramHS;

/*
** HS Volt Cur Limit Init
*/
void init_HSVolCurLimit(void){
	paramHS.volLimit = 54.6;
	paramHS.curLimit =10;
}

/*
** get HS Volt Cur Limit
*/
ParamHS get_HSVolCurLimit(void){
	return paramHS;
}

void param_GetCardPassword(uint8 *cardPw)
{//说明:获取系统主板的PIN码

}

uint16 param_GetConsumeWay(void)
{//说明:获取计费方式
	return(paramSys.consumWay);
}

void param_SetConsumeWay(uint16 consumeWay)
{//说明:更新计费方式
	paramSys.consumWay = consumeWay;
}

void param_SaveConsumeWay(void)
{
	uint8 buf[LEN_CONSUMEWAY];
	memcpy((uint8*)&buf[0],(uint8*)&paramSys.consumWay,sizeof(uint16));
	buf[2] = calcSum((uint8*)&buf[0],LEN_CONSUMEWAY-1);
	eeprom_write(EADDR_CONSUMEWAY,(uint8*)&buf[0],LEN_CONSUMEWAY);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_CONSUMEWAY, (uint8*)&buf[0], LEN_CONSUMEWAY);
	#endif	
}

void param_ReadConsumeWay(void)
{
	uint8 buf[LEN_CONSUMEWAY],buf_bkp[LEN_CONSUMEWAY];
	eeprom_read(EADDR_CONSUMEWAY,(uint8*)&buf[0],LEN_CONSUMEWAY);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_CONSUMEWAY, (uint8*)&buf_bkp[0], LEN_CONSUMEWAY);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_CONSUMEWAY-1) == buf[LEN_CONSUMEWAY-1])
	{
		paramSys.consumWay = (((uint16)buf[1])<<8) + buf[0];
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_CONSUMEWAY, (uint8*)&buf[0], LEN_CONSUMEWAY);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_CONSUMEWAY-1) == buf_bkp[2])
	{
		paramSys.consumWay = (((uint16)buf_bkp[1])<<8) + buf_bkp[0];
		eeprom_write(EADDR_CONSUMEWAY,(uint8*)&buf_bkp[0],LEN_CONSUMEWAY);
	}
	else
	{
		paramSys.consumWay = Month_Card;
		param_SaveConsumeWay();
	}
}

uint16 param_GetSmokeCtr(void)
{//说明:获取计费方式
	return(paramSys.smokeCtr);
}

void param_SetSmokeCtr(uint16 smokeCtr)
{//说明:更新计费方式
	paramSys.smokeCtr = smokeCtr;
}

void param_SaveSmokeCtr(void)
{
	uint8 buf[LEN_SmokeCtr];
	memcpy((uint8*)&buf[0],(uint8*)&paramSys.smokeCtr,sizeof(uint16));
	buf[2] = calcSum((uint8*)&buf[0],LEN_SmokeCtr-1);
	eeprom_write(EADDR_SmokeCtr,(uint8*)&buf[0],LEN_SmokeCtr);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_SmokeCtr, (uint8*)&buf[0], LEN_SmokeCtr);
	#endif	
}

void param_ReadSmokeCtr(void)
{
	uint8 buf[LEN_SmokeCtr],buf_bkp[LEN_SmokeCtr];
	eeprom_read(EADDR_SmokeCtr,(uint8*)&buf[0],LEN_SmokeCtr);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_SmokeCtr, (uint8*)&buf_bkp[0], LEN_SmokeCtr);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_SmokeCtr-1) == buf[LEN_SmokeCtr-1])
	{
		paramSys.smokeCtr = (((uint16)buf[1])<<8) + buf[0];
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_SmokeCtr, (uint8*)&buf[0], LEN_SmokeCtr);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_SmokeCtr-1) == buf_bkp[2])
	{
		paramSys.smokeCtr = (((uint16)buf_bkp[1])<<8) + buf_bkp[0];
		eeprom_write(EADDR_SmokeCtr,(uint8*)&buf_bkp[0],LEN_SmokeCtr);
	}
	else
	{
		paramSys.smokeCtr = Close_SmokeCtr;
		param_SaveSmokeCtr();
	}
}


void param_GetDeviceId(uint8 *deviceId)
{
	memcpy((uint8*)&deviceId[0],(uint8*)&paramSys.deviceId[0],DeviceAddr_MAX*sizeof(uint16));
}

void param_SetDeviceId(uint8*deviceId)
{
	memcpy((uint8*)&paramSys.deviceId[0],(uint8*)&deviceId[0],DeviceAddr_MAX*sizeof(uint16));
}

void param_SaveDeviceId(void)
{
	uint8 buf[LEN_DeviceId];
	memcpy((uint8*)&buf[0],(uint8*)&paramSys.deviceId[0],DeviceAddr_MAX*sizeof(uint16));
	buf[LEN_DeviceId-1] = calcSum((uint8*)&buf[0],LEN_DeviceId-1);
	eeprom_write(EADDR_DeviceId,(uint8*)&buf[0],LEN_DeviceId);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_DeviceId, (uint8*)&buf[0], LEN_DeviceId);
	#endif	
}

void param_ReadDeviceId(void)
{
	uint8 buf[LEN_DeviceId],buf_bkp[LEN_DeviceId];
	eeprom_read(EADDR_DeviceId,(uint8*)&buf[0],LEN_DeviceId);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_DeviceId, (uint8*)&buf_bkp[0], LEN_DeviceId);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_DeviceId-1) == buf[LEN_DeviceId-1])
	{
		memcpy((uint8*)&paramSys.deviceId[0],(uint8*)&buf[0],DeviceAddr_MAX*sizeof(uint16));
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_DeviceId, (uint8*)&buf[0], LEN_DeviceId);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_DeviceId-1) == buf_bkp[LEN_DeviceId-1])
	{
		memcpy((uint8*)&paramSys.deviceId[0],(uint8*)&buf_bkp[0],DeviceAddr_MAX*sizeof(uint16));
		eeprom_write(EADDR_DeviceId,(uint8*)&buf_bkp[0],LEN_DeviceId);
	}
	else
	{
		memset((uint8*)&paramSys.deviceId[0],' ',DeviceAddr_MAX*sizeof(uint16));
		param_SaveDeviceId();
	}
}

uint16 param_GetSingleConsume(void)
{//说明:获取单次消费金额
	return(paramSys.singleConsume);
}

void param_SetSingleConsume(uint16 singleConsume)
{//说明:设置单次消费金额
	paramSys.singleConsume = singleConsume;
}

void param_SaveSingleConsume(void)
{
	uint8 buf[LEN_SINGLECONSUME];
	memcpy((uint8*)&buf[0],(uint8*)&paramSys.singleConsume,sizeof(uint16));
	buf[2] = calcSum((uint8*)&buf[0],LEN_SINGLECONSUME-1);
	eeprom_write(EADDR_SINGLECONSUME,(uint8*)&buf[0],LEN_SINGLECONSUME);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_SINGLECONSUME, (uint8*)&buf[0], LEN_SINGLECONSUME);
	#endif	
}

void param_ReadSingleConsume(void)
{
	uint8 buf[LEN_SINGLECONSUME],buf_bkp[LEN_SINGLECONSUME];
	eeprom_read(EADDR_SINGLECONSUME,(uint8*)&buf[0],LEN_SINGLECONSUME);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_SINGLECONSUME, (uint8*)&buf_bkp[0], LEN_SINGLECONSUME);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_SINGLECONSUME-1) == buf[LEN_SINGLECONSUME-1])
	{
		paramSys.singleConsume = (((uint16)buf[1])<<8) + buf[0];
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_SINGLECONSUME, (uint8*)&buf[0], LEN_SINGLECONSUME);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_SINGLECONSUME-1) == buf_bkp[2])
	{
		paramSys.singleConsume = (((uint16)buf_bkp[1])<<8) + buf_bkp[0];
		eeprom_write(EADDR_SINGLECONSUME,(uint8*)&buf_bkp[0],LEN_SINGLECONSUME);
	}
	else
	{
		paramSys.singleConsume = 100;//默认参数1元
		param_SaveSingleConsume();
	}
}

uint16 param_GetBatingConversionBatFull(void)
{//说明:获取满电转换参数时间值
	return(paramSys.fullBatSetTime);
}

void param_SetBatingConversionBatFull(uint16 fullSetTime)
{//说明:设置满电转换参数时间
	paramSys.fullBatSetTime = fullSetTime;
}

void param_SaveFullBatSetTime(void)
{
	uint8 buf[LEN_FULLSETTIME];
	memcpy((uint8*)&buf[0],(uint8*)&paramSys.fullBatSetTime,sizeof(uint16));
	buf[2] = calcSum((uint8*)&buf[0],LEN_FULLSETTIME-1);
	eeprom_write(EADDR_FULLSETTIME,(uint8*)&buf[0],LEN_FULLSETTIME);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_FULLSETTIME, (uint8*)&buf[0], LEN_FULLSETTIME);
	#endif	
}

void param_ReadFullBatSetTime(void)
{
	uint8 buf[LEN_FULLSETTIME],buf_bkp[LEN_FULLSETTIME];
	eeprom_read(EADDR_FULLSETTIME,(uint8*)&buf[0],LEN_FULLSETTIME);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_FULLSETTIME, (uint8*)&buf_bkp[0], LEN_FULLSETTIME);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_FULLSETTIME-1) == buf[LEN_FULLSETTIME-1])
	{
		paramSys.fullBatSetTime = (((uint16)buf[1])<<8) + buf[0];
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_FULLSETTIME, (uint8*)&buf[0], LEN_FULLSETTIME);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_FULLSETTIME-1) == buf_bkp[2])
	{
		paramSys.fullBatSetTime = (((uint16)buf_bkp[1])<<8) + buf_bkp[0];
		eeprom_write(EADDR_FULLSETTIME,(uint8*)&buf_bkp[0],LEN_FULLSETTIME);
	}
	else
	{
		paramSys.fullBatSetTime = 150;//默认150Min
		param_SaveFullBatSetTime();
	}
}

#if Modify_PW_190822 == 1
void param_GetParamSysPw(ParamSysPw *parSysPw)
{
	memcpy((uint8*)&parSysPw->sysAdmin[0],(uint8*)&paramSysPw.sysAdmin[0],sizeof(ParamSysPw));
}

void param_SetParamSysPw(ParamSysPw parSysPw)
{
	memcpy((uint8*)&paramSysPw.sysAdmin[0],(uint8*)&parSysPw.sysAdmin[0],sizeof(ParamSysPw));
}

void param_SaveParamSysPw(void)
{
	uint8 buf[LEN_SysPwParam];
	memcpy((uint8*)&buf[0],(uint8*)&paramSysPw.sysAdmin[0],sizeof(ParamSysPw));
	buf[LEN_SysPwParam-1] = calcSum((uint8*)&buf[0],LEN_SysPwParam-1);
	eeprom_write(EADDR_SysPwParam,(uint8*)&buf[0],LEN_SysPwParam);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_SysPwParam, (uint8*)&buf[0], LEN_SysPwParam);
	#endif	
}

void param_ReadParamSysPw(void)
{
	uint8 buf[LEN_SysPwParam],buf_bkp[LEN_SysPwParam];
	uint16 sysAdmin[4] = {0x3733,0x3531,0xFFFF,0x2020};
	uint16 sysFun[4] = {0x3733,0x3531,0x3136,0x3839};
	
	eeprom_read(EADDR_SysPwParam,(uint8*)&buf[0],LEN_SysPwParam);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EADDR_SysPwParam, (uint8*)&buf_bkp[0], LEN_SysPwParam);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_SysPwParam-1) == buf[LEN_SysPwParam-1])
	{
		memcpy((uint8*)&paramSysPw.sysAdmin[0],(uint8*)&buf[0],4*sizeof(uint16));
		memcpy((uint8*)&paramSysPw.sysFun[0],(uint8*)&buf[8],4*sizeof(uint16));
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EADDR_SysPwParam, (uint8*)&buf[0], LEN_SysPwParam);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_SysPwParam-1) == buf_bkp[LEN_SysPwParam-1])
	{
		memcpy((uint8*)&paramSysPw.sysAdmin[0],(uint8*)&buf_bkp[0],4*sizeof(uint16));
		memcpy((uint8*)&paramSysPw.sysFun[0],(uint8*)&buf_bkp[8],4*sizeof(uint16));
		eeprom_write(EADDR_SysPwParam,(uint8*)&buf_bkp[0],LEN_SysPwParam);
	}
	else
	{
		memcpy((uint8*)&paramSysPw.sysAdmin[0],(uint8*)&sysAdmin[0],4*sizeof(uint16));
		memcpy((uint8*)&paramSysPw.sysFun[0],(uint8*)&sysFun[0],4*sizeof(uint16));
		param_SaveParamSysPw();
	}
}
#endif

#if Modify_Key_190826 == 1
ParamSysCabinet param_GetSysCabinetKey(void)
{
	return(paramSysCabinet);
}

void param_SetSysCabinetKey(ParamSysCabinet pSysCabinet)
{
	paramSysCabinet = pSysCabinet;
}

void param_SaveParamSysCabinetKey(void)
{
	uint8 buf[LEN_KeySysParam];
	memcpy((uint8*)&buf[0],(uint8*)&paramSysCabinet.key[0],sizeof(ParamSysCabinet));
	buf[LEN_KeySysParam-1] = calcSum((uint8*)&buf[0],LEN_KeySysParam-1);
	eeprom_write(EAADR_KeySysParam,(uint8*)&buf[0],LEN_KeySysParam);
	#if EEPROMDATA_REPAIRDAT_EN == 1
		BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EAADR_KeySysParam, (uint8*)&buf[0], LEN_KeySysParam);
	#endif	
}

void param_ReadParamSysCabinetKey(void)
{
	uint8 buf[LEN_KeySysParam],buf_bkp[LEN_KeySysParam];
	uint8 key[sizeof(ParamSysCabinet)] = {0xFF,0xFF,0xFF,0xFF};
	
	eeprom_read(EAADR_KeySysParam,(uint8*)&buf[0],LEN_KeySysParam);
	#if  EEPROMDATA_REPAIRDAT_EN == 1
		BKP_ReadBackupRegDat(EADDR_BKPDATOFFSET + EAADR_KeySysParam, (uint8*)&buf_bkp[0], LEN_KeySysParam);
	#endif
	if(calcSum((uint8*)&buf[0],LEN_KeySysParam-1) == buf[LEN_KeySysParam-1])
	{
		memcpy((uint8*)&paramSysCabinet.key[0],(uint8*)&buf[0],sizeof(ParamSysCabinet));
		#if  EEPROMDATA_REPAIRDAT_EN == 1
			BKP_WriteBackupDat(EADDR_BKPDATOFFSET + EAADR_KeySysParam, (uint8*)&buf[0], LEN_KeySysParam);
		#endif
	}
	else if(calcSum((uint8*)&buf_bkp[0],LEN_KeySysParam-1) == buf_bkp[LEN_KeySysParam-1])
	{
		memcpy((uint8*)&paramSysCabinet.key[0],(uint8*)&buf_bkp[0],sizeof(ParamSysCabinet));
		eeprom_write(EAADR_KeySysParam,(uint8*)&buf_bkp[0],LEN_KeySysParam);
	}
	else
	{
		memcpy((uint8*)&paramSysCabinet.key[0],(uint8*)&key[0],sizeof(ParamSysCabinet));
		param_SaveParamSysCabinetKey();
	}
	M1_UpdateKeyData((uint8 *)&paramSysCabinet.key[0],0);
}

#endif

void param_UpdateSysParamToLCD(void)
{
	uint8 tmp[DeviceAddr_MAX*sizeof(uint16)] = {0};

	/*更新系统页面时间*/
	LCD_UpdateSysTimeShow();

	/*清除迪文屏页面消费按键值*/
	LCD_ClearConsumeWay();

	/*更新系统设置参数值*/
	LCD_SetConsumeWaySet(param_GetConsumeWay());

	/*更新烟感控制参数*/
	Lcd_SetSmokeCtr(param_GetSmokeCtr());
	
	/*更新屏幕按键值*/
	LCD_UpdateConsumeIcon(param_GetConsumeWay());

	/*更新屏幕烟感图标*/
	LCD_UpdateSmokeCtrIcon(param_GetSmokeCtr());

	/*更新单次消费金额*/
	LCD_SetSingleConsume(param_GetSingleConsume());

	/*更新设备地址*/
	param_GetDeviceId((uint8 *)&tmp[0]);
	LCD_SetDeviceId((uint8 *)&tmp[0]);

	/*更新满电设置时间*/
	LCD_SetFullSetTime(param_GetBatingConversionBatFull());

	#if Modify_Key_190826 == 1
	/*更新密钥*/
	LCD_SetCabinetKey(param_GetSysCabinetKey());
	#endif
}

void param_Init(void)
{
	/*
	** HS Volt Cur Limit Init
	*/
	init_HSVolCurLimit();

	param_ReadConsumeWay();
	param_ReadSmokeCtr();
	param_ReadSingleConsume();
	param_ReadDeviceId();
	param_ReadFullBatSetTime();
	ChargeRecordNoRead();

	#if Modify_PW_190822 == 1
	param_ReadParamSysPw();
	#endif

	#if Modify_Key_190826 == 1
	param_ReadParamSysCabinetKey();
	#endif
}

