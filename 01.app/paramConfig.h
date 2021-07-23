#ifndef PARAMCONFIG_H
#define PARAMCONFIG_H

/*eeprom地址分配*/
#define EADDR_RECORDNO										0x10
#define ELEN_RECORDNO										(sizeof(uint16)+1)

#define EADDR_CONSUMEWAY									(EADDR_RECORDNO + ELEN_RECORDNO)
#define LEN_CONSUMEWAY										(sizeof(uint16)+1)

#define EADDR_SINGLECONSUME								(EADDR_CONSUMEWAY + LEN_CONSUMEWAY)
#define LEN_SINGLECONSUME									(sizeof(uint16)+1)

#define EADDR_FULLSETTIME									(EADDR_SINGLECONSUME + LEN_SINGLECONSUME)
#define LEN_FULLSETTIME										(sizeof(uint16)+1)

#define EADDR_SmokeCtr										(EADDR_FULLSETTIME + LEN_FULLSETTIME)
#define LEN_SmokeCtr											(sizeof(uint16)+1)

#define EADDR_DeviceId											(EADDR_SmokeCtr+LEN_SmokeCtr)
#define LEN_DeviceId												(DeviceAddr_MAX*sizeof(uint16)+1)

#if Modify_PW_190822 == 1
#define EADDR_SysPwParam									(EADDR_DeviceId + LEN_DeviceId)
#define LEN_SysPwParam										(8*sizeof(uint16)+1)
#endif

#if Modify_Key_190826 == 1
#define EAADR_KeySysParam									(EADDR_SysPwParam+LEN_SysPwParam)
#define LEN_KeySysParam										(sizeof(ParamSysCabinet)+1)
#endif

/*备份区地址直接增加偏移量*/
#define EADDR_BKPDATOFFSET								1024//备份地址偏移量


/*flash地址说明*/
#define FADDR_RECORD_BASE		0	
#define FLEN_RECORD				sFLASH_SPI_PAGESIZE

enum
{
	Open_SmokeCtr = (uint16)0x01,//烟感开启控制
	Close_SmokeCtr = (uint16)0x02,//关闭烟感控制
};

typedef struct
{
	uint16 consumWay;//消费方式
	uint16 singleConsume;//单次消费金额 倍率--100
	uint16 fullBatSetTime;//满电设定时间值
	uint16 smokeCtr;//修改:使用屏幕控制烟感逻辑是否执行
	uint16 deviceId[DeviceAddr_MAX];//设备ID
}ParamSys;

#if Modify_PW_190822 == 1
typedef struct
{
	uint16 sysAdmin[4];
	uint16 sysFun[4];
}ParamSysPw;
#endif

#if Modify_Key_190826 == 1
typedef struct
{
	uint8 key[4];
}ParamSysCabinet;
#endif

/*
** 恒晟--电压/电流阈值
*/
typedef struct{
	float volLimit;/*电压限定阈值*/
	float curLimit;/*电流设定阈值*/
}ParamHS;

void param_GetCardPassword(uint8 *cardPw);
void param_UpdateSysParamToLCD(void);
uint16 param_GetConsumeWay(void);
void param_SetConsumeWay(uint16 consumeWay);
void param_SaveConsumeWay(void);
void param_ReadConsumeWay(void);
uint16 param_GetSingleConsume(void);
void param_SetSingleConsume(uint16 singleConsume);
void param_SaveSingleConsume(void);
void param_ReadSingleConsume(void);
uint16 param_GetBatingConversionBatFull(void);
void param_SetBatingConversionBatFull(uint16 fullSetTime);
void param_SaveFullBatSetTime(void);
void param_ReadFullBatSetTime(void);
uint16 param_GetSmokeCtr(void);
void param_SetSmokeCtr(uint16 smokeCtr);
void param_SaveSmokeCtr(void);
void param_ReadSmokeCtr(void);
void param_GetDeviceId(uint8 *deviceId);
void param_SetDeviceId(uint8*deviceId);
void param_SaveDeviceId(void);
void param_ReadDeviceId(void);

void param_Init(void);

#if Modify_PW_190822 == 1
void param_GetParamSysPw(ParamSysPw *parSysPw);
void param_SetParamSysPw(ParamSysPw parSysPw);
void param_SaveParamSysPw(void);
#endif

#if Modify_Key_190826 == 1
ParamSysCabinet param_GetSysCabinetKey(void);
void param_SetSysCabinetKey(ParamSysCabinet pSysCabinet);
void param_SaveParamSysCabinetKey(void);
#endif

/*
** get HS Volt Cur Limit
*/
ParamHS get_HSVolCurLimit(void);

#endif

