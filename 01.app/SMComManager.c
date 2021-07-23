#include "includes.h"

#define SIZE_STK 1536

static uint8 OtherComNum=1;

/*通讯任务表*/
SM_TaskPara st_SMTaskPara[D_SM_TaskNum];

/*显示屏类*/
static uint32 stk_LCDTask[SIZE_STK];
DP_FramePara SM_LCD_InitiativeFrame;        
static uint32 uiLCDComState;
static uint16  uiLCDComCnt[32];
static uint32 uiLCDComFlag;
static SM_LogicUnit st_LogicSheetLCD[]=
{
	{D_SM_InitiativeCmd,D_LCD_CMD_JumpScreen	,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_JumpScreen,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadScreenId,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadScreenID,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteTime,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteTime,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteStateIcon,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteStateIcon,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteStateNum,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteStateNum,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteAdminPwShow,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteAdminPwShow,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteAdminPwAscii,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteAdminPwAscii,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadAdminPwAscii,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadAdminPwAscii,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteAdminPwIcon,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteAdminPwIcon,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteSysParamTime,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteSysParamTime,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadSysParamTime,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadSysParamTime,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteConsumeWay,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteConsumeWay,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadConsumeWay,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadConsumeWay,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteConsumeIcon,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteConsumeWayIcon,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadSingleConsume,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadSingleConsume,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteSingleConsume,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteSingleConsume,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteFullSetTime,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteFullSetTime,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadFullSetTime,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadFullSetTime,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteRecord,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteRecordData,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteRecordState,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteRecordState,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadRecordState,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadRecordState,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteClearRecordBtn,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteClearRecord,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadClearRecordBtn,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadClearRecord,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadOneBtnOpenDoor,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadClearOneBtnOpenDoor,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteOneBtnOpenDoor,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteClearOneBtnOpenDoor,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadAppointDoorId,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadAppointDoorId,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteAppointDoorId,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteAppointDoorId,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteSoftVersion,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteSoftVersion,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	
	#if Modify_PW_190822 == 1
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteModifyPWShow,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteModifyPwShow,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadModifyPWAscii,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadModifyPwAscii,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteModifyPWAscii,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteModifyPwAscii,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	#endif
	#if Modify_Key_190826 == 1
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadModifyKey,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadModifyCardKey,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteModifyKey,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteModifyCardKey,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadClearStepBtn,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadCancelModifyCardKeyStep,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteClearStepBtn,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteCancelModifyCardKeyStep,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteKeyModeChoice,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteKeyModeChoice,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	#endif

	{D_SM_InitiativeCmd,D_LCD_CMD_WriteVoiceInfo,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,lcd_WriteVoiceCtr,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	
	{D_SM_InitiativeCmd,D_LCD_CMD_WriteCfgVolCur,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_WriteModuleVolCur,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag},
	{D_SM_InitiativeCmd,D_LCD_CMD_ReadCfgVolCur,&SM_LCD_InitiativeFrame,DP_PackageFrame,DP_ParseFrame,LCD_SetData,LCD_ReadoduleVolCur,null,&OtherComNum,&uiLCDComState,uiLCDComCnt,&uiLCDComFlag}	
};
static const uint16 iLogicSheetLCDUnitNum = sizeof(st_LogicSheetLCD)/sizeof(SM_LogicUnit);

/*读卡器类*/
static uint32 stk_CardReaderTask[SIZE_STK];
static MT318_FramePara SM_CardReader_InitiativeFrame;
static uint32 uiCardReaderComState;
static uint16  uiCardReaderComCnt[32];
static uint32 uiCardReaderComFlag;
#if MT_CardVersion == MT_Card_M1
static SM_LogicUnit st_LogicSheetCard[]=
{
	{D_SM_InitiativeCmd, D_M1_CMD_FindCar,&SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, M1_FindCardResult, M1_FindCard, null, &OtherComNum,  &uiCardReaderComState , uiCardReaderComCnt , &uiCardReaderComFlag},
	{D_SM_InitiativeCmd, D_M1_CMD_VerifyKeyA,&SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, M1_VerifyResult, M1_VerifyKeyA, null, &OtherComNum,  &uiCardReaderComState , uiCardReaderComCnt , &uiCardReaderComFlag},
	{D_SM_InitiativeCmd, D_M1_CMD_VerifyKeyB,&SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, M1_VerifyResult, M1_VerifyKeyB, null, &OtherComNum,  &uiCardReaderComState, uiCardReaderComCnt , &uiCardReaderComFlag},	
	{D_SM_InitiativeCmd, D_M1_CMD_ReadData,&SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, M1_ReadResult, M1_ReadBlockData, null, &OtherComNum,  &uiCardReaderComState, uiCardReaderComCnt , &uiCardReaderComFlag},	
	{D_SM_InitiativeCmd, D_M1_CMD_Impairment,&SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, M1_ImpairmentOptResult, M1_ImpairmentOpt, null, &OtherComNum,  &uiCardReaderComState, uiCardReaderComCnt , &uiCardReaderComFlag},	
	#if Modify_Key_190826 == 1
	{D_SM_InitiativeCmd, D_M1_CMD_Modify_key,&SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, M1_VerifyResult, M1_ModifyKeyOpt, null, &OtherComNum,  &uiCardReaderComState , uiCardReaderComCnt , &uiCardReaderComFlag},
	#endif
};
#else
static SM_LogicUnit st_LogicSheetCard[]=
{
	{D_SM_InitiativeCmd, D_MT318_CMD_Pretereatment, &SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, MT_PreSetData, MT_PreStartGetData, null, &OtherComNum,  &uiCardReaderComState, uiCardReaderComCnt , &uiCardReaderComFlag},
	{D_SM_InitiativeCmd, D_MT318_CMD_StartCharge,   &SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, MT_StartEndChargeSetData, MT_StartGetData, null, &OtherComNum,  &uiCardReaderComState, uiCardReaderComCnt , &uiCardReaderComFlag},
	{D_SM_InitiativeCmd, D_MT318_CMD_EndCharge,     &SM_CardReader_InitiativeFrame,  MT_PackageFrame,  MT_ParseFrame, MT_StartEndChargeSetData, MT_EndGetData, null, &OtherComNum,  &uiCardReaderComState, uiCardReaderComCnt , &uiCardReaderComFlag},
};
#endif
static const uint16 iLogicSheetCardUnitNum = sizeof(st_LogicSheetCard)/sizeof(SM_LogicUnit);

/*
** 充电器类(振源科技)
*/
static uint32 stk_zyChargerTask[SIZE_STK];
static ZY_Charger sm_ZYCharger;
#if Debug_SinglePoint == 1
static uint8 zyChargerNum = 1/*ALLDOORNUM*/;
#else
static uint8 zyChargerNum = ALLDOORNUM;
#endif
static uint32 zyComState;
static uint16  zyComCnt[32];
static uint32 zyComFlag;

static SM_LogicUnit st_LogicSheetZY[] = {
	{D_SM_InitiativeCmd, D_ZY_CMD_Get_State,&sm_ZYCharger,  zy_PackageFrame,  zy_ParseFrame, zy_ParseChargerInfo, zy_State_Charger_GetInfo, null, &zyChargerNum,  &zyComState , zyComCnt , &zyComFlag},
	{D_SM_InitiativeCmd, D_ZY_CMD_Set_VoltCur,&sm_ZYCharger,  zy_PackageFrame,  zy_ParseFrame, zy_ParseChargerInfo, zy_Charger_SetVoltCur, null, &zyChargerNum,  &zyComState , zyComCnt , &zyComFlag},	
	{D_SM_InitiativeCmd, D_ZY_CMD_Set_OnOff,&sm_ZYCharger,  zy_PackageFrame,  zy_ParseFrame, zy_ParseChargerInfo, zy_Charger_SetOnOff, null, &zyChargerNum,  &zyComState , zyComCnt , &zyComFlag},	
	{D_SM_InitiativeCmd, D_ZY_CMD_GetSetValue,&sm_ZYCharger,  zy_PackageFrame,  zy_ParseFrame, zy_ParseChargerInfo, zy_ChargerSetValueGet, null, &zyChargerNum,  &zyComState , zyComCnt , &zyComFlag},	
	{D_SM_InitiativeCmd, D_ZY_CMD_GetSetOnOff,&sm_ZYCharger,  zy_PackageFrame,  zy_ParseFrame, zy_ParseChargerInfo, zy_ChargerSetOnOffGet, null, &zyChargerNum,  &zyComState , zyComCnt , &zyComFlag},	
	#if Test_FunModule_ZY == 0 /*在正常模式下启动循环检测状态信息*/
	{D_SM_CycleCmd, D_ZY_CMD_Get_State,&sm_ZYCharger,  zy_PackageFrame,  zy_ParseFrame, zy_ParseChargerInfo, zy_State_Charger_GetInfo, null, &zyChargerNum,  &zyComState , zyComCnt , &zyComFlag}
	#endif
};
static const uint16 iLogicSheetZYUnitNum = sizeof(st_LogicSheetZY)/sizeof(SM_LogicUnit);

static void SM_InitLCDTask(void)
{
	st_SMTaskPara[D_SM_TASKID_LCD].ucComType = BSP_ComType_USART;
	st_SMTaskPara[D_SM_TASKID_LCD].ucComID = BSP_ComUsart3;
	st_SMTaskPara[D_SM_TASKID_LCD].ucTableLen = iLogicSheetLCDUnitNum;
	st_SMTaskPara[D_SM_TASKID_LCD].pst_LogicTable = st_LogicSheetLCD;
	st_SMTaskPara[D_SM_TASKID_LCD].cmdNum = 0;

	uiLCDComState = 0;    
}

uint32 SM_GetLCDComState(void)
{
	return *(st_LogicSheetLCD[0].ucComFlag);
}

static void SM_InitCardTask(void)
{
	st_SMTaskPara[D_SM_TASKID_CARD].ucComType = BSP_ComType_USART;
	st_SMTaskPara[D_SM_TASKID_CARD].ucComID = BSP_ComUsart1;
	st_SMTaskPara[D_SM_TASKID_CARD].ucTableLen = iLogicSheetCardUnitNum;
	st_SMTaskPara[D_SM_TASKID_CARD].pst_LogicTable = st_LogicSheetCard;
	
	st_SMTaskPara[D_SM_TASKID_CARD].cmdNum = 0;
	uiCardReaderComState = 0;  
}

uint32 SM_GetCardComState(void)
{
	return (*(st_LogicSheetCard[0].ucComFlag));
}

/*
** zy Charger Init Task
*/
void SM_InitZYTask(void){
	st_SMTaskPara[D_SM_ZY_Charger].ucComType = BSP_ComType_USART;
	st_SMTaskPara[D_SM_ZY_Charger].ucComID = BSP_ComUsart2;
	st_SMTaskPara[D_SM_ZY_Charger].ucTableLen = iLogicSheetZYUnitNum;
	st_SMTaskPara[D_SM_ZY_Charger].pst_LogicTable = st_LogicSheetZY;
	
	st_SMTaskPara[D_SM_ZY_Charger].cmdNum = 0;	
}

/*
** get Charger Com State 
*/
uint32 get_ChargerComState(void){
	return (*(st_LogicSheetZY[0].ucComFlag));
}

void SM_InitSmComManagement(void)
{
	SM_InitLCDTask();
	SM_InitCardTask();
	/*
	** zy Charger Init Task
	*/
	 SM_InitZYTask();	
	
	/*读卡器*/
	OSTaskCreate(SM_DisposeSmCom, (void *)(&st_SMTaskPara[D_SM_TASKID_CARD]), &stk_CardReaderTask[SIZE_STK - 1], D_CARD_TASK_PRO);

	/*显示屏*/
  	OSTaskCreate(SM_DisposeSmCom, (void *)(&st_SMTaskPara[D_SM_TASKID_LCD]), &stk_LCDTask[SIZE_STK - 1], D_LCD_TASK_PRO);

	/*zy Charger*/
	OSTaskCreate(SM_DisposeSmCom, (void *)(&st_SMTaskPara[D_SM_ZY_Charger]), &stk_zyChargerTask[SIZE_STK - 1], D_ZY_Charger_Task_PRO);
}

