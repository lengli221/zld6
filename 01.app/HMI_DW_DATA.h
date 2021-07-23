#ifndef HMI_DW_DATA_H
#define HMI_DW_DATA_H

#define HMI_PAGEDEEP_NUM								20//设计页面最大深度20

#define HMI_DW_RECORD_NUM							10//一页显示的记录数

#define HMI_DW_PAGE_MAIN								(uint8)0x00//主页面
#define HMI_DW_PAGE_Swiping							(uint8)0x18//刷卡页面
#define HMI_DW_PAGE_Tasking							(uint8)0x19//取卡页面
#define HMI_DW_PAGE_NoSwOpt							(uint8)0x14//无法换电页面
#define HMI_DW_PAGE_ExitSaveBatOpt					(uint8)0x15//存在存电池操作
#define HMI_DW_PAGE_AdminPwPage					(uint8)0x06//管理员密码页面
#define HMI_DW_PAGE_SysSetPage						(uint8)0x13//系统设置
#define HMI_DW_PAGE_chargeRecordPage				(uint8)0x02//充电记录页面
#define HMI_DW_PAGE_OpenIdleErr						(uint8)0x0F//开空仓故障
#define HMI_DW_PAGE_OpenFullErr						(uint8)0x10//开满仓故障
#define HMI_DW_PAGE_NoSaveBat						(uint8)0x17//未存入电池
#define HMI_DW_PAGE_SaveAbnBat						(uint8)0x1B//存入异常电池
#define HMI_DW_PAGE_OTSaveNorBat					(uint8)0x1C//超时存入正常电池
#define HMI_DW_PAGE_OTSaveAbnBat					(uint8)0x1E//超时存入异常电池
#define HMI_DW_PAGE_NoFullBat							(uint8)0x1D//未取满电
#define HMI_DW_PAGE_chargeRecordChkPage			(uint8)0x1F//客户查询充电记录
#define HMI_DW_PAGE_ClearHistoryRecord				(uint8)0x0B//清除充电记录
#define HMI_DW_PAGE_OneBtnOpenDoor				(uint8)0x12//开所有仓门
#define HMI_DW_PAGE_AppointDoorId					(uint8)0x1A//开指定门
#define HMI_DW_PAGE_SoftVersion						(uint8)0x04//软件版本号

#define HMI_DW_PAGE_DeviceIdNoLegality				(uint8)0x2B//设备ID非法性

#if Modify_PW_190822 == 1
#define HMI_DW_PAGE_ModifyPw_Sys					(uint8)0x07//修改系统密码页面
#define HMI_DW_PAGE_ModifyPw_Fun					(uint8)0x2C//修改功能密码页面
#endif

#if Modify_Key_190826 == 1
#define HMI_DW_PAGE_Modify_Key						(uint8)0x2D//修改卡片密钥
#endif

enum
{
	cardCom_Normal = (uint16)0x0000,//刷卡板正常
	cardCom_Abnormal = (uint16)0x0001,//刷卡板异常
};

enum
{
	swipeIdle = (uint8)0x00,
	idleToSwipe = (uint8)0x01,
	swiping = (uint8)0x02,
};

typedef struct
{
	uint8 stepFlag;
}HMISwipeCtr;

typedef struct
{
	uint8 id;
	uint8 state;
	void (*handle)(void);
}HMI_DW_PAGE;

typedef struct
{
	uint8 id[HMI_PAGEDEEP_NUM];
	void *handle[HMI_PAGEDEEP_NUM];
	uint8 deepCnt;
}HMI_MultiDeepPage;

/*---------------------------------语音标志------------------------------------------------*/
typedef union{
	uint32 flag;
	struct{
		uint32 idleDoorOpen:1;/*空闲仓门已打开*/
		uint32 fullDoorOpen:1;/*满仓仓门已打开*/
		uint32 carding:1;/*刷卡中*/
		uint32 cardFinsh:1;/*刷卡完成*/
		uint32 comErr:1;/*通讯异常*/
		uint32 doorNoClose:1;/*仓门未关*/
		uint32 batNoSave:1;/*电池未放入*/
		uint32 openIdleDoorFail:1;/*开仓失败*/
		uint32 noIdleDoor:1;/*无空闲仓*/
		uint32 noExceBat:1;/*无可用电池*/
		uint32 openFullDoorFail:1;/*开满仓失败*/
		uint32 optBusy:1;/*操作繁忙中*/
		uint32 saveErrBat:1;/*存入异常电池*/
		uint32 fullNoTake:1;/*满电未取走*/
		uint32 saveBatOutTime:1;/*存电池异常*/
		uint32 swing:1;/*换电中*/
		uint32 swFail:1;/*换电失败*/
		uint32 res:15;/*保留*/
	}bits;
}VoiceTriggerFlag;

/*
** HMI Vocie Info
*/
typedef struct{
	VoiceTriggerFlag triggerFlag;/*触发标志*/
}HMI_VocieInfo;
/*----------------------------------------------------------------------------------------------*/

void HMI_Init(void);
void HMI_DW_PageDataTableFun(void);
void HMI_DW_PageSwAccordDoorState(uint8 setPageId);
void HMI_DW_OptBindingProc(void);
uint8 HMI_GetMultiDeepPageId(void);

#if Modify_Key_190826 == 1
HMI_DW_PAGE Get_HmiDwPageParam(void);
#endif

/*
** HMI DW Voice Init  
*/
void HMI_DW_VoiceDeal(void);

#endif

