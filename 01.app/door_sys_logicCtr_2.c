#include "includes.h"
static int32 logicCtr_TaskSTK[1024] = {0};

ProcessSysCtr processSysCtr[ALLDOORNUM] = {0};
ProcessSysCtr thisProcessSysCtr = {0};
DoorSysParam doorSysParam;
SysParam sysParam;
ChargerTime *doorSysChargerTim;

AbnDealLogic abnDealLogic;

/*HMI Vocie Info*/
extern HMI_VocieInfo hmi_VocieInfo;

ProcessSysCtr *GetProcessSysCtr(void)
{
	return(&processSysCtr[0]);
}

DoorSysParam *GetDoorSysParam(void)
{
	return(&doorSysParam);
}

SysParam *GetSysParam(void)
{
	return(&sysParam);
}

void DoorSysCtrParam_Init(void)
{
	memset((uint8*)&sysParam.optCnt,0x00,sizeof(SysParam));
	doorSysChargerTim = GetChargerTime();
}

void DoorSysClearProcSysParam(void)
{
	memset((uint8*)&thisProcessSysCtr.flag,0x00,sizeof(ProcessSysCtr));
}

uint8 DoorSysGetIdleDoorNum(void)
{
	uint8 i=0;
	uint8 idleNum = 0;
	for(i=0;i<ALLDOORNUM;i++)
	{
		if(doorSysParam.doorState[i] == doorIdle)
		{
			idleNum++;
		}
	}
	return(idleNum);
}

uint8 DoorSysGetFullBatNum(void)
{
	uint8 i=0;
	uint8 fullNum = 0;
	for(i=0;i<ALLDOORNUM;i++)
	{
		if(doorSysParam.doorState[i] == BatFull)
		{
			fullNum++;
		}
	}
	return(fullNum);
}

uint8 DoorSysGetUsingBatNum(void)
{
	uint8 i=0;
	uint8 usingNum = 0;
	for(i=0;i<ALLDOORNUM;i++)
	{
		if(doorSysParam.doorState[i] == BatCharging || doorSysParam.doorState[i] == BatFull)
		{
			usingNum++;
		}
	}
	return(usingNum);
}


uint8 DoorSysGetExchangeableBatNum(void)
{
	uint8 i=0;
	uint16 chgTimBase = param_GetBatingConversionBatFull();
	uint8 exchangeNum = DoorSysGetFullBatNum();
	
	for(i=0;i<ALLDOORNUM;i++)
	{
		if(doorSysParam.doorState[i] == BatCharging)
		{
			if(doorSysChargerTim->min[i] >= chgTimBase)
			{
				exchangeNum++;
			}
		}
	}
	return(exchangeNum);
}

/*
** take Bat Before Cloes Charger 
*/
bool take_BatBeforeCloseCharger(uint8 doorId){
	uint8 ret = false;
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};
	static uint32 closeChargerStep[ALLDOORNUM] = {0}; 
	static uint32 closeChargerTickOut[ALLDOORNUM] = {0};
	static uint32 closeChargerRepTick[ALLDOORNUM] = {0};

	if(thisProcessSysCtr.process == Process_TakeFullBat){
		switch(closeChargerStep[doorId]){
			case 0:
				TickOut((uint32 *)&closeChargerTickOut[doorId], 0);
				closeChargerStep[doorId]++;
				break;
			case 1:
				if(TickOut((uint32 *)&closeChargerTickOut[doorId], 2000) == false){/*2S内连续关闭模块,超时重发机制*/
					TickOut((uint32 *)&closeChargerRepTick[doorId], 0);
					set_ZY_ChargerOnOff(false,doorId);
					iParam[1] = D_ZY_CMD_Set_OnOff;
					iParam[2] = doorId;
					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
					closeChargerStep[doorId]++;
				}else{/*超时未关闭充电模块*/
					/*形成"关闭充电模块",无法关机异常记录*/
					closeChargerStep[doorId] = 0;
					ret = true;
				}
				break;
			case 2:
				if(TickOut((uint32 *)&closeChargerRepTick[doorId], 100) == true){
					TickOut((uint32 *)&closeChargerRepTick[doorId], 0);
// 					iParam[1] = D_ZY_CMD_GetSetOnOff;
// 					iParam[2] = doorId;
// 					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
					closeChargerStep[doorId]++;	
				}
				break;
			case 3:
				if(get_ZYChargerOnOffCmdExeState(doorId) == true){/*模块已关机*/
					closeChargerStep[doorId] = 0;
					ret = true;
				}else{
					closeChargerStep[doorId] = 1;
				}
				break;
		}
	}else{/*存电池无此操作流程*/
		ret = true;
	}
	
	return ret;
}

/*
** get Charger State Tick
*/
void get_ChargerStateTcik(uint8 doorId){
	static uint32 getChargerStateTick[ALLDOORNUM] = {0};
	uint32 iParam[3] = {0x01,D_ZY_CMD_Get_State,0};

	if(TickOut((uint32 *)&getChargerStateTick[doorId], 200) == true){
		TickOut((uint32 *)&getChargerStateTick[doorId], 0);
		iParam[2] = doorId;
		SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
	}
}

/*
** close doorIdle	after Open Charger
*/
bool close_DoorIdleAfterOpenCharger(uint8 doorId){
	uint8 ret = false;
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};
	static uint32 openChargerStep[ALLDOORNUM] = {0}; 
	static uint32 openChargerTickOut[ALLDOORNUM] = {0};
	static uint32 openChargerRepTick[ALLDOORNUM] = {0};

	switch(openChargerStep[doorId]){
		case 0:
			TickOut((uint32 *)&openChargerTickOut[doorId], 0);
			openChargerStep[doorId]++;
			break;
		case 1:
			if(TickOut((uint32 *)&openChargerTickOut[doorId], 2000) == false){/*2S内连续开模块,超时重发机制*/
				TickOut((uint32 *)&openChargerRepTick[doorId], 0);
				set_ZY_ChargerOnOff(true,doorId);
				iParam[1] = D_ZY_CMD_Set_OnOff;
				iParam[2] = doorId;
				SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
				openChargerStep[doorId]++;
			}else{/*超时模块未开启*/
				/*形成"开充电模块",无法开机异常记录*/
				openChargerStep[doorId] = 0;
				ret = true;
			}
			break;
		case 2:
			if(TickOut((uint32 *)&openChargerRepTick[doorId], 100) == true){
				TickOut((uint32 *)&openChargerRepTick[doorId], 0);
// 				iParam[1] = D_ZY_CMD_GetSetOnOff;
// 				iParam[2] = doorId;
// 				SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
				openChargerStep[doorId]++;	
			}
			break;
		case 3:
			if(get_ZYChargerOnOffCmdExeState(doorId) == true){/*模块已关机*/
				openChargerStep[doorId] = 0;
				ret = true;
			}else{
				openChargerStep[doorId] = 1;
			}
			break;
	}	
	return ret;
}

/*
** normal Logic Idle Fun
*/
void normalLogic_IdleFun(void){
	if(abnDealLogic.Flag != 0){/*存在异常需要处理*/
		abnDealLogic.handle = save_AbnBatRepOpenDoor;
	}
}

/*
** save Abn Bat Rep Open Door 
*/
void save_AbnBatRepOpenDoor(void){
	uint8 i = 0;
	static uint8 step[ALLDOORNUM] = {0};
	static uint32 itick[ALLDOORNUM] = {0};
	static uint32 itickOut[ALLDOORNUM] = {0};
	static uint8 openDoorCnt[ALLDOORNUM] = {0};
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};

	for(i=0;i<ALLDOORNUM;i++){
		if((abnDealLogic.Flag & (1<<i)) != 0){
			switch(step[i]){
				case 0:
					set_ZY_ChargerOnOff(false, i);
					iParam[2] = i;
					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);	
					hmi_VocieInfo.triggerFlag.bits.swFail = true;/*换电失败*/
					TurnOnKC(i);
					TickOut((uint32 *)&itick[i], 0x00);
					step[i]++;
					break;
				case 1:
					if(TickOut((uint32 *)&itick[i], 200)){
						TurnOffKC(i);
						if(YXState(i)){
							abnDealLogic.Flag &= ~(1<<i);
						}else{
							if(TickOut((uint32 *)&itickOut[i], 505)){
								if(openDoorCnt[i]++>=3){
									openDoorCnt[i] = 0;
									abnDealLogic.Flag &= ~(1<<i);
								}
								step[i] = 0;
							}
						}
					}
					break;
			}
		}else{
			step[i] = 0;
		}
	}

	if(abnDealLogic.Flag == 0){
		abnDealLogic.handle = normalLogic_IdleFun;
	}
}

/*开仓门与存电池流程*/
void DoorSysSaveTakeBatProcessCtr(uint8 doorId)
{
	static uint8 step[ALLDOORNUM] = {0};
	static uint32 itick[ALLDOORNUM] = {0};
	static uint32 itickOut[ALLDOORNUM] = {0};
	static uint8 openDoorCnt[ALLDOORNUM] = {0};
	static uint32 saveBatTick[ALLDOORNUM] = {0};
	static uint32 closeTick[ALLDOORNUM] = {0};
	static uint32 closeChkBatTick[ALLDOORNUM] = {0};
	static uint32 takeBatTick[ALLDOORNUM] = {0};
	uint8 state = 0;
	static uint32 swingTick = 0;

	switch(step[doorId])
	{
		case 0:
			if(take_BatBeforeCloseCharger(doorId) == true){/*存电池直接开仓,取电池需先关机模块再开仓门*/
				TurnOnKC(doorId);
				TickOut((uint32 *)&itick[doorId], 0x00);
				TickOut((uint32 *)&itickOut[doorId], 0x00);
				step[doorId]++;
			}
			break;
		case 1:
			if(TickOut((uint32 *)&itick[doorId], 200))
			{
				TurnOffKC(doorId);		
				if(YXState(doorId))
				{
					if(thisProcessSysCtr.process == Process_SaveBat)
					{
						step[doorId]++;
						TickOut((uint32 *)&saveBatTick[doorId], 0x00);
						hmi_VocieInfo.triggerFlag.bits.idleDoorOpen = true;
					}
					else
					{
						step[doorId] = 5;
						TickOut((uint32 *)&takeBatTick[doorId], 0x00);
						hmi_VocieInfo.triggerFlag.bits.fullDoorOpen = true;
					}
				}
				else
				{
					if(TickOut((uint32 *)&itickOut[doorId], 505))
					{
						if(openDoorCnt[doorId]++>=3)
						{
							openDoorCnt[doorId] = 0;
							doorSysParam.doorState[doorId] = doorErr;
							/*业务逻辑变更:开舱异常,直接挂起(说明:回检与锁可能出现问题,异常不明确),存储异常记录,做退费处理*/
							if(thisProcessSysCtr.process == Process_SaveBat)
							{
								state = OpenDoorIdErr;
								HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_OpenIdleErr);
								hmi_VocieInfo.triggerFlag.bits.openIdleDoorFail = true;
							}
							else
							{
								state = OpenFullErr;
								HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_OpenFullErr);
								hmi_VocieInfo.triggerFlag.bits.openFullDoorFail = true;
							}
							ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);
							DoorSysClearProcSysParam();
							sysParam.optCnt--;
						}
						step[doorId] = 0;
					}
				}
			}
			break;
		/*step2-4:save bat*/
		case 2:
			if(TickOut((uint32 *)&saveBatTick[doorId], 60000) == false)
			{
				/*get Charger State Tick*/
				get_ChargerStateTcik(doorId);
				if(doorSysParam.doorState[doorId] != doorIdle)
				{
					TickOut((uint32 *)&closeTick[doorId], 0x00);
					step[doorId]++;
				}

				if(!YXState(doorId)&&doorSysParam.doorState[doorId] == doorIdle)
				{/*说明:增加未放入电池直接关门*/
					TickOut((uint32 *)&closeTick[doorId], 0x00);
					step[doorId]++;
				}
			}
			else
			{
				doorSysParam.doorState[doorId] = doorOptOverTime;
				thisProcessSysCtr.process = Process_SaveBatOverTime;
				sysParam.optCnt--;
				step[doorId] = 0;
				hmi_VocieInfo.triggerFlag.bits.saveBatOutTime = true;
			}
			break;
		case 3:
			if(TickOut((uint32 *)&closeTick[doorId], 22000) == false)
			{
				/*get Charger State Tick*/
				get_ChargerStateTcik(doorId);

				if(!YXState(doorId))
				{
					TickOut((uint32 *)&closeTick[doorId], 0x00);
					TickOut((uint32 *)&closeChkBatTick[doorId], 0x00);
					/*未接入电池,空载开机模块,模块会默认为电池在满电情况
						故修改关仓之后,检测存在电池再开机*/
					if(doorSysParam.doorState[doorId] == BatOnline){
						/*close doorIdle	after Open Charger*/
						if(close_DoorIdleAfterOpenCharger(doorId) == true){
							step[doorId]++;
						}
					}else{/*空闲状态直接等待五秒结束流程*/
						step[doorId]++;
					}
					hmi_VocieInfo.triggerFlag.bits.swing = true;/*换电中*/
					TickOut((uint32 *)&swingTick, 0);
				}
			}
			else
			{
				doorSysParam.doorState[doorId] = doorOptOverTime;
				thisProcessSysCtr.process = Process_SaveBatOverTime;
				sysParam.optCnt--;
				step[doorId] = 0;
				hmi_VocieInfo.triggerFlag.bits.doorNoClose = true;
			}
			break;
		case 4:
			if(TickOut((uint32 *)&closeChkBatTick[doorId], DoorSys_CloseChkBatTick))
			{
				if(doorSysParam.doorState[doorId] == BatCharging || doorSysParam.doorState[doorId] == BatFull)
				{
					state = Normal_SaveBat;
					ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);
					thisProcessSysCtr.process = Process_ChkExchangeBat;
					thisProcessSysCtr.doorData.doorId = 0x00;
				}
				else
				{/*放入电池异常或电池被取走:生成异常记录--卡号+仓门号+电池异常描述*/
					if(doorSysParam.doorState[doorId] == doorIdle)
					{
						state = No_SaveBat;
						ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);						
						HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_NoSaveBat);
						hmi_VocieInfo.triggerFlag.bits.batNoSave = true;
					}
					else
					{
						if(doorSysParam.doorState[doorId] == BatTurnAround || doorSysParam.doorState[doorId] == BatOverTempP 
							|| doorSysParam.doorState[doorId] == BatOverVolP || doorSysParam.doorState[doorId] == batErr)
						{
							state = SaveAbnormalBat;
							ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);		
							HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_SaveAbnBat);
							hmi_VocieInfo.triggerFlag.bits.saveErrBat = true;
						}
					}
					DoorSysClearProcSysParam();
					sysParam.optCnt--;
					/*异常:重新开启存电仓门*/
					abnDealLogic.Flag |= (uint32)(1<<doorId);
				}
				step[doorId] = 0;
			}else{
				/*get Charger State Tick*/
				get_ChargerStateTcik(doorId);
				/*换电中语言重复播报*/
				if(TickOut((uint32 *)&swingTick, 3200) == true){
					TickOut((uint32 *)&swingTick, 0);
					hmi_VocieInfo.triggerFlag.bits.swing = true;/*换电中*/
				}
			}
			break;
		/*step5-8,take bat*/
		case 5:
			if(TickOut((uint32 *)&takeBatTick[doorId], 40000) == false)
			{
				if(!YXState(doorId))
				{
					TickOut((uint32 *)&takeBatTick[doorId], 0x00);
					TickOut((uint32 *)&closeChkBatTick[doorId], 0x00);
					step[doorId]++;
				}
			}
			else
			{
				step[doorId] += 2;
			}
			break;
		case 6:
			if(TickOut((uint32 *)&closeChkBatTick[doorId], 3000))
			{
				if(doorSysParam.doorState[doorId] != doorIdle)
				{/*生成异常记录:卡号+仓门+未取满电*/
					state = CloseDoorNoTakeBat;
					ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);														
					HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_NoFullBat);
					step[doorId] +=2;
					hmi_VocieInfo.triggerFlag.bits.fullNoTake = true;
				}
				else
				{
					step[doorId]++;
				}
			}
			break;
		case 7:
			state = CloseDoorTakeBat;
			ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);																				
			step[doorId]++;
			break;
		case 8:
			DoorSysClearProcSysParam();
			sysParam.optCnt--;
			step[doorId] = 0;
			break;
		default:
			
			break;
	}
}

/*存电池超时挂起操作*/
void DoorSysSaveBatOverTimeOpt(uint8 doorId)
{
	static uint8 step[ALLDOORNUM] = {0};	
	uint8 state = 0;
	static uint32 itick[ALLDOORNUM] ={0};
	
	switch(step[doorId])
	{
		case 0:
			if(!YXState(doorId))
			{
				doorSysParam.doorState[doorId] = doorOptOverTimeMid;
				step[doorId]++;
				TickOut((uint32 *)&itick, 0);
			}
			break;
		case 1://等待更新电池状态
			if(TickOut((uint32 *)&itick, 1000) == true){
				TickOut((uint32 *)&itick, 0);
				if(doorSysParam.doorState[doorId] != doorOptOverTimeMid)
				{
					if(doorSysParam.doorState[doorId] == BatCharging || doorSysParam.doorState[doorId] == BatFull)
					{/*生成异常记录:仓门+超时电池记录*/
						state = OverTimeSaveNormalBat;
						ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);														
						HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_OTSaveNorBat);
					}

					if(doorSysParam.doorState[doorId] == BatTurnAround || doorSysParam.doorState[doorId] == BatOverTempP 
						|| doorSysParam.doorState[doorId] == BatOverVolP || doorSysParam.doorState[doorId] == batErr)
					{/*生成异常记录:仓门+异常电池*/
						state = OverTimeSaveAbnormalBat;
						ChargeRecordWrite(thisProcessSysCtr.procId, state, (uint8 *)&thisProcessSysCtr.cradData.card[0], thisProcessSysCtr.doorData.doorId);																			
						HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_OTSaveAbnBat);
						hmi_VocieInfo.triggerFlag.bits.saveErrBat = true;
					}
					DoorSysClearProcSysParam();
					step[doorId] = 0;
					if(doorSysParam.doorState[doorId] != doorIdle ){
						/*存电超时:开存电仓门*/
						abnDealLogic.Flag |= (1<<doorId);
					}
				}
			}else{
				/*get Charger State Tick*/
				get_ChargerStateTcik(doorId);
			}
			break;
		default:
			
			break;
	}
}

/*控制实际操作人员的个数:根据业务流程--放电池与取电池分两个业务逻辑*//*刷卡完成前扣费条件之一*/
uint8 DoorSysRealOptCntFlag(void)
{
	uint8 ret = 0;
	uint8 i=0;

	for(i=0;i<ALLDOORNUM;i++)
	{
		if(processSysCtr[i].process == Process_SaveBat)
		{
			ret = Exiting_SaveBatOpt;
			return(ret);
		}
	}
	
	if(DoorSysGetIdleDoorNum()>=1 && DoorSysGetExchangeableBatNum()>=(sysParam.optCnt+1))
	{
		ret = OptCnt_Succ;
	}
	else
	{
		ret = No_OptDoor;
	}
	return(ret);/*sysParam.optCnt++加一功能应置于刷卡成功之后取卡的操作*/
}

/*获取设定进程标志的Id:proc*/
uint8 DoorSysGetSetProcessFlagProc(void)
{
	uint8 proc = 0;
	uint8 ret = 0;
	for(proc=0;proc<ALLDOORNUM;proc++)
	{
		if(processSysCtr[proc].process == Process_Idle)
		{
			ret = proc;
			break;
		}
	}
	return(ret);
}

/*获取空仓相应地址并设置相应仓门的进程标志(并确认是否可取走电池)*/
bool DoorSysGetIdleAddrSetProcess(void)
{
	bool ret = true;
	int8 idleAddr = -1;
	static int8 lastIdleAddr = -1;
	uint8 i=0;

	/*防止在刷卡成功之后,电池突然异常或仓门异常*/
	if(DoorSysGetIdleDoorNum() == 0 || DoorSysGetExchangeableBatNum()< sysParam.optCnt)
	{
		ret = false;
		return(ret);
	}

	for(i=(lastIdleAddr+1);i<ALLDOORNUM;i++)
	{
		if(doorSysParam.doorState[i] == doorIdle)
		{
			idleAddr = i;
			break;
		}
	}

	if(idleAddr == -1)
	{
		for(i=0;i<(lastIdleAddr+1);i++)
		{
			if(doorSysParam.doorState[i] == doorIdle)
			{
				idleAddr = i;
				break;
			}
		}
	}
	thisProcessSysCtr.process = Process_SaveBat;
	thisProcessSysCtr.doorData.doorId = idleAddr;
	lastIdleAddr = idleAddr;
	return(ret);
}

/*查询仓门地址是否绑定开满仓地址*/
bool DoorSysChkDoorIdBindProc(uint8 doorId)
{
	uint8 proc = 0;
	uint8 ret = true;

	for(proc=0;proc<ALLDOORNUM;proc++)
	{
		if((processSysCtr[proc].process == Process_TakeFullBat || processSysCtr[proc].process == Process_SaveBatOverTime)&& processSysCtr[proc].doorData.doorId == doorId)
		{/*开满仓将绑定超时流程,导致超时流程无超时记录生成*/
			ret = false;
			break;
		}
	}
	return(ret);
}

/*查询可换电池的仓门ID*/
void DoorSysGetExchangeBatAddr(void)
{
	uint16 chgTimBase = param_GetBatingConversionBatFull();
	static int8 lastFullAddr = -1;
	int8 fullAddr = -1;
	uint8 i=0;

	/*防止在开空箱之后存入电池之后,开满仓电池异常*/
	if(DoorSysGetExchangeableBatNum() == 0)
	{/*生成异常记录:*/
		ChargeRecordWrite(thisProcessSysCtr.procId, NoFullDoor, (uint8 *)&thisProcessSysCtr.cradData.card[0], 0x00);																			
		DoorSysClearProcSysParam();
		sysParam.optCnt--;
		HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_NoSwOpt);
		goto NoExchangeBat;
	}

	/*1.查询满电电池*/
	if(DoorSysGetFullBatNum()>=1)
	{
		for(i=(lastFullAddr+1);i<ALLDOORNUM;i++)
		{
			if(doorSysParam.doorState[i] == BatFull && DoorSysChkDoorIdBindProc(i))
			{
				fullAddr = i;
				break;
			}		
		}

		if(fullAddr == -1)
		{
			for(i=0;i<(lastFullAddr+1);i++)
			{
				if(doorSysParam.doorState[i] == BatFull && DoorSysChkDoorIdBindProc(i))
				{
					fullAddr = i;
					break;
				}
			}
		}
	}
	else
	{
		for(i=(lastFullAddr+1);i<ALLDOORNUM;i++)
		{
			if(doorSysParam.doorState[i] == BatCharging && doorSysChargerTim->min[i] >= chgTimBase && DoorSysChkDoorIdBindProc(i))
			{
				fullAddr = i;
				break;
			}		
		}

		if(fullAddr == -1)
		{
			for(i=0;i<(lastFullAddr+1);i++)
			{
				if(doorSysParam.doorState[i] == BatCharging && doorSysChargerTim->min[i] >= chgTimBase && DoorSysChkDoorIdBindProc(i))
				{
					fullAddr = i;
					break;
				}
			}
		}		
	}

	thisProcessSysCtr.process = Process_TakeFullBat;
	thisProcessSysCtr.doorData.doorId = fullAddr;
	/*参数待处理*/
	ChargeRecordWritePreData(thisProcessSysCtr.procId, Record_FullInit, param_GetConsumeWay());
	lastFullAddr = fullAddr;
NoExchangeBat:;
}

void DoorLogicCtr(uint8 proc)
{
	switch(proc)
	{
		case Process_SaveBat:
		case Process_TakeFullBat:
			DoorSysSaveTakeBatProcessCtr(thisProcessSysCtr.doorData.doorId);
			break;
		case Process_SaveBatOverTime:
			DoorSysSaveBatOverTimeOpt(thisProcessSysCtr.doorData.doorId);
			break;
		case Process_ChkExchangeBat:
			DoorSysGetExchangeBatAddr();
			break;
		default:
			if(!DoorSysGetIdleAddrSetProcess())
			{/*跳转至标志No_OptDoor控制页面*/
				ChargeRecordWrite(thisProcessSysCtr.procId, NoIdleDoor, (uint8 *)&thisProcessSysCtr.cradData.card[0], 0x00);																			
				DoorSysClearProcSysParam();
				sysParam.optCnt--;
				HMI_DW_PageSwAccordDoorState(HMI_DW_PAGE_NoSwOpt);
			}
			break;
	}
}

bool GetSysEPOFlag(void)
{
	return(sysParam.sysEPOFlag);
}

bool GetSysSmokeFlag(void)
{
	return(sysParam.sysSmokeFlag);
}

static void SysSPDCtr(void)
{
	static int8 cnt = 0;
	static int8 cnt1 = 0;

	if(YXStateSysCtr(sys_EPO))
	{
		cnt++;
	}
	else
	{
		cnt--;
	}

	if(cnt>=5)
	{
		cnt = 5;
		TurnOnBreaker(ac_Breaker);
		TurnOnWaringLed(waring_Led);
		sysParam.sysDeviceErrFlag = true;
		sysParam.sysEPOFlag = true;
	}

	if(cnt<=-5)
	{
		cnt = -5;
		sysParam.sysEPOFlag = false;
	}

	if(YXStateSysCtr(sys_SPD)&&param_GetSmokeCtr() == Open_SmokeCtr)
	{
		cnt1++;
	}
	else
	{
		cnt1--;
	}

	if(cnt1>=5)
	{
		cnt1=5;
		TurnOnBreaker(ac_Breaker);
		TurnOnWaringLed(waring_Led);
		/*sysParam.sysDeviceErrFlag = true;*//*恒晟无烟感检测*/
		sysParam.sysSmokeFlag = true;
	}

	if(cnt1<=-5)
	{
		cnt1= -5;
		sysParam.sysSmokeFlag = false;
	}

	if(cnt<=-5&&cnt1<=-5)
	{
		TurnOffBreaker(ac_Breaker);
		TurnOffWaringLed(waring_Led);
		sysParam.sysDeviceErrFlag = false;
	}
}

static void DoorSysWaterChk(void)
{
	static int8 cnt = 0;

	if(YXStateSysCtr(sys_water))
	{
		cnt++;
		
	}
	else
	{
		cnt--;
	}

	if(cnt>=5)
	{
		cnt = 5;
		sysParam.waterChk = WaterChk_Abnormal;
	}

	if(cnt<=-5)
	{
		cnt = -5;
		sysParam.waterChk = WaterChk_Nomal;
	}
}

uint16 GetDoorSysWaterChk(void)
{
	return(sysParam.waterChk);
}

uint16 GetDoorSysCardComErr(void)
{
	return(sysParam.cardComErr);
}

void DoorSysChkChargerComErr(void)
{
	uint8 i=0;
	uint32 chargerState = get_ChargerComState();
	
	for(i=0;i<ALLDOORNUM;i++)
	{
		if((chargerState & (1<<i)) != 0){
			doorSysParam.doorState[i] = chargerModuleComErr;/*通讯失联即判断仓门异常,更换充电器需重新下电处理*/
		}
	}
}

void DoorSysOneOpenAllDoor(void)
{
	uint8 openAllDoorId  = 0;
	uint8 step=0;
	static uint32 openTimeTick = 0;
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};
	
	if(sysParam.oneBtnOpenFlag == 0x01)
	{
		for(;openAllDoorId<ALLDOORNUM;)
		{
			watchdogUpdate();
			switch(step)
			{
				case 0:
					TickOut((uint32 *)&openTimeTick, 0x00);
					TurnOnKC(openAllDoorId);
					step++;
					break;
				case 1:
					if(TickOut((uint32 *)&openTimeTick, 200))
					{
						TickOut((uint32 *)&openTimeTick, 0x00);
						TurnOffKC(openAllDoorId);
						set_ZY_ChargerOnOff(false, openAllDoorId);
						iParam[2] = openAllDoorId;
						SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
						step++;
					}
					break;
				case 2:
					openAllDoorId++;
					step = 0;
					break;
			}
		}
		sysParam.oneBtnOpenFlag = 0;
	}
}

void DoorSysOneAppointDoorId(void)
{
	static uint32 openTimeTick = 0;
	static uint8 step=0;
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};
	
	if(sysParam.appointId.flag)
	{
		switch(step)
		{
			case 0:
				TurnOnKC(sysParam.appointId.doorid);
				TickOut((uint32 *)&openTimeTick, 0x00);
				step++;
				break;
			case 1:
				if(TickOut((uint32 *)&openTimeTick, 200))
				{
					TickOut((uint32 *)&openTimeTick, 0x00);
					TurnOffKC(sysParam.appointId.doorid);
					set_ZY_ChargerOnOff(false, sysParam.appointId.doorid);
					iParam[2] = sysParam.appointId.doorid;
					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);					
					step++;
				}
				break;
			case 2:
				sysParam.appointId.flag = false;
				sysParam.appointId.doorid = 0;
				step = 0;
				break;
		}
	}
}

/*
** idle chk is exit bat open close module 
*/
void idleChk_IsExitBatCtrChgModule(void){
	static uint8 addr = 0;
	static uint32 itick = 0;
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};
	bool ctrModuleFlag = false;/*默认关机*/

	if(sysParam.optCnt == 0){/*空闲状态*/
		if(TickOut((uint32 *)&itick, 1000) == true){
			TickOut((uint32 *)&itick, 0);
			if(!YXState(addr)){
				if(doorSysParam.doorState[addr] == BatOnline 
					|| doorSysParam.doorState[addr] == BatCharging
					|| doorSysParam.doorState[addr] == BatFull){
					ctrModuleFlag = true;/*模块开机*/
				}
			}
			set_ZY_ChargerOnOff(ctrModuleFlag, addr);
			iParam[2] = addr;
			SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);	
			addr++;
			if(addr == ALLDOORNUM){
				addr = 0;
			}
		}
	}
}

void DoorSysLogicCtr_Task(void *p_arg)
{
	abnDealLogic.handle = normalLogic_IdleFun;

	DoorSysCtrParam_Init();
	
	Sleep(1000);
	for(;;)
	{
		/*Abn Deal Logic*/
		abnDealLogic.handle();

		watchdogUpdate();

		/*计算充电时间*/
		CalcChargerIntervalTime();
		/*充电时间备份*/
		chargerTimeWriteBackupDat();

		/*说明:分析业务逻辑的变更成随意刷卡皆开舱门将导致多单扣费异常(开空箱的异常导致)--扣费提前扣 从目前业务逻辑变更需要在刷卡时指定业务逻辑,并修改目前业务逻辑控制部分*/
		DoorSysProcessCtrFun();
		
		CC_Control_Fan();
		SysSPDCtr();
		DoorSysOneOpenAllDoor();
		DoorSysOneAppointDoorId();
		DoorSysWaterChk();
		DoorSysChkChargerComErr();

		#if Test_FunModule_ZY == 1
			/*
			** Test ZY
			*/
			test_ZY();
		#endif
		
		Sleep(20);
	}
}

void DoorSysLogicCtr_Init(void)
{
	Thread_create(DoorSysLogicCtr_Task, 0, (int32 *)&logicCtr_TaskSTK[1024-1], D_LogicCtr_TASK_PRO);
}

