#ifndef DOOR_SYS_LOGIC_H
#define DOOR_SYS_LOGIC_H
#define DoorSys_CloseChkBatTick					(uint32)5000

#include "type.h"

/*系统仓门状态*/
enum
{
	doorIdle = 0x00,//门空闲(电池不存在且正常)
	/*电池存在*/
	BatCharging = (uint8)0x01,//充电中 
	BatFull = (uint8)0x02,//满电
	BatTurnAround = (uint8)0x03,//电池反接
	BatOverTempP = (uint8)0x04,//过温保护
	BatOverVolP = (uint8)0x05,//过压保护
	BatOnline= (uint8)0x06,/*电池接入*/
	batErr = (uint8)0x07,/*电池故障*/
	chargerModuleComErr = 0xFC,/*充电器模块通讯异常*/
	doorOptOverTimeMid = (uint8)0xFD,//开空仓超时异常挂起中间量--待通讯板更新数据
	doorOptOverTime = (uint8)0xFE,//开空仓超时暂时异常挂起
	doorErr = 0xFF,//仓门异常挂起
};

enum
{
	Exiting_SaveBatOpt = 0x01,//存电池标志
	No_OptDoor = 0x02,//无仓门可操作标志
	OptCnt_Succ = 0x03,//操作人员增加成功标志
};

enum
{
	Process_Idle = 0x00,//空闲进程
	Process_SaveBat = 0x01,//存电池业务逻辑
	Process_SaveBatOverTime = 0x02,//存电池业务之超时异常
	Process_ChkExchangeBat = 0x03,//查询可换电池仓门ID
	Process_TakeFullBat = 0x04,//取满电池业务逻辑
};

enum
{
	WaterChk_Nomal = (uint16)0x0000,//水位正常
	WaterChk_Abnormal = (uint16)0x0001,//水位异常
};

#pragma pack(1)
typedef struct
{
	uint8 card[8];//卡号
}CardData;

typedef struct
{
	uint8 doorId;//仓门状态
}DoorData;

typedef struct
{
	bool flag;//线程标志
	CardData cradData;//卡相关数据
	uint8 process;//进程
	uint8 procId;//进程ID
	DoorData doorData;//仓门相关数据
}ProcessSysCtr;

typedef struct
{
	uint8 doorState[ALLDOORNUM];//仓门状态
	uint8 closeDoorAbnCnt[ALLDOORNUM];//关舱门连续三次判断仓门异常 
	Common_Time time[ALLDOORNUM];//充电开始时间
}DoorSysParam;

typedef struct
{
	bool flag;
	uint8 doorid;
}SysAppointId;//开指定仓门参数

typedef struct
{
	uint8 optCnt;//操作人数
	uint8 oneBtnOpenFlag;//一键开所有门
	SysAppointId appointId;
	uint16 waterChk;//水位检测
	uint16 cardComErr;//刷卡板通讯异常
	bool sysDeviceErrFlag;//系统设备异常--急停,烟感故障不允许刷卡
	bool sysSmokeFlag;//烟感标志
	bool sysEPOFlag;//急停标志
}SysParam;

/*
** Abn Deal Logic
*/
typedef struct{
	uint32 Flag;/*异常处理标志*/
	void (*handle)(void);
}AbnDealLogic;

#pragma pack()

ProcessSysCtr *GetProcessSysCtr(void);
DoorSysParam *GetDoorSysParam(void);
SysParam *GetSysParam(void);
void DoorSysLogicCtr_Init(void);
uint8 DoorSysGetIdleDoorNum(void);
uint8 DoorSysGetUsingBatNum(void);
uint8 DoorSysRealOptCntFlag(void);
uint8 DoorSysGetSetProcessFlagProc(void);
uint16 GetDoorSysWaterChk(void);
uint16 GetDoorSysCardComErr(void);
void DoorLogicCtr(uint8 proc);
void DoorSysProcessCtrFun(void);
bool GetSysEPOFlag(void);
bool GetSysSmokeFlag(void);
/*
** normal Logic Idle Fun
*/
void normalLogic_IdleFun(void);
/*
** save Abn Bat Rep Open Door 
*/
void save_AbnBatRepOpenDoor(void);
/*
** idle chk is exit bat open close module 
*/
void idleChk_IsExitBatCtrChgModule(void);
#endif

