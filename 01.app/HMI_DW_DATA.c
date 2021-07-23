#include "includes.h"

static int32 HMI_DW_TaskSTK[1000] = {0};  //任务堆栈

HMI_DW_PAGE thispage;
HMI_DW_PAGE thislastpage;
static HMISwipeCtr hmiSwipeCtr;
ProcessSysCtr *hmiProcessSysCtr;
SysParam *hmiSysParam;
HMI_MultiDeepPage hmiMulitDeepPage;

#if Modify_Key_190826 == 1
void HMI_DW_ClearModifyKey(void);
#endif

/*HMI Vocie Info*/
HMI_VocieInfo hmi_VocieInfo;

#if Modify_Key_190826 == 1
HMI_DW_PAGE Get_HmiDwPageParam(void)
{
	return(thispage);
}
#endif

/*
** HMI DW Voice Init  
*/
void HMI_DW_VoiceDeal(void){
	static uint32 itick = 0;
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteVoiceInfo,0};

	if(TickOut((uint32 *)&itick, 2000) == true){
		TickOut((uint32 *)&itick, 0);
		iParam[2] = 36;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
}

/*
** HMI DW Vocie Info Deal
*/
void HMI_DW_VocieInfoDeal(void){
	static uint8 state = 0;
	static uint32 itick = 0;
	static uint32 timePara = 3200;
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteVoiceInfo,0};
	bool triggerFlag = false;

	switch(state){
		case 0:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.idleDoorOpen == true
					&& hmi_VocieInfo.triggerFlag.bits.cardFinsh == false /*优化语音播报顺序,在刷卡和开空仓顺序控制*/
					){
					hmi_VocieInfo.triggerFlag.bits.idleDoorOpen = false;
					triggerFlag = true;
					iParam[2] = 1;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}
			break;
		case 1:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.fullDoorOpen == true){
					hmi_VocieInfo.triggerFlag.bits.fullDoorOpen = false;
					triggerFlag = true;
					iParam[2] = 4;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 2:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.carding == true){
					hmi_VocieInfo.triggerFlag.bits.carding = false;
					triggerFlag = true;
					iParam[2] = 6;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 3:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.cardFinsh == true){
					hmi_VocieInfo.triggerFlag.bits.cardFinsh = false;
					triggerFlag = true;
					iParam[2] = 8;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 4:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.comErr == true){
					hmi_VocieInfo.triggerFlag.bits.comErr = false;
					triggerFlag = true;
					iParam[2] = 10;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 5:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.doorNoClose == true){
					hmi_VocieInfo.triggerFlag.bits.doorNoClose = false;
					triggerFlag = true;
					iParam[2] = 12;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 6:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.batNoSave == true){
					hmi_VocieInfo.triggerFlag.bits.batNoSave = false;
					triggerFlag = true;
					iParam[2] = 15;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 7:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.openIdleDoorFail == true){
					hmi_VocieInfo.triggerFlag.bits.openIdleDoorFail = false;
					triggerFlag = true;
					iParam[2] = 17;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 8:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.noIdleDoor == true){
					hmi_VocieInfo.triggerFlag.bits.noIdleDoor = false;
					triggerFlag = true;
					iParam[2] = 19;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 9:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.noExceBat == true){
					hmi_VocieInfo.triggerFlag.bits.noExceBat = false;
					triggerFlag = true;
					iParam[2] = 21;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 10:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.openFullDoorFail == true){
					hmi_VocieInfo.triggerFlag.bits.openFullDoorFail = false;
					triggerFlag = true;
					iParam[2] = 23;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 11:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.optBusy == true){
					hmi_VocieInfo.triggerFlag.bits.optBusy = false;
					triggerFlag = true;
					iParam[2] = 25;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 12:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.saveErrBat == true){
					hmi_VocieInfo.triggerFlag.bits.saveErrBat = false;
					triggerFlag = true;
					iParam[2] = 27;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;	
		case 13:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.fullNoTake == true){
					hmi_VocieInfo.triggerFlag.bits.fullNoTake = false;
					triggerFlag = true;
					iParam[2] = 29;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 14:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.saveBatOutTime == true){
					hmi_VocieInfo.triggerFlag.bits.saveBatOutTime = false;
					triggerFlag = true;
					iParam[2] = 31;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 15:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.swing == true){
					hmi_VocieInfo.triggerFlag.bits.swing = false;
					triggerFlag = true;
					iParam[2] = 38;
					TickOut((uint32 *)&itick, 0);
				}
				state++;
			}			
			break;
		case 16:
			if(TickOut((uint32 *)&itick, timePara) == true){
				if(hmi_VocieInfo.triggerFlag.bits.swFail == true){
					hmi_VocieInfo.triggerFlag.bits.swFail = false;
					triggerFlag = true;
					iParam[2] = 40;
					TickOut((uint32 *)&itick, 0);
				}
				state = 0;
			}			
			break;			
	}

	if(triggerFlag == true){
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
}

static void HMI_DW_MainPage(void)
{
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteStateIcon,0};
	#if MainPageVer_190823 == 1
		LCD_SoftVersion hmiLcdSoftVer;
		static uint32 tick = 0/*OSTimeGet()*/;//说明:增加(static + OSTimeGet())编译无法通过--function call is not allowed in a constant expression
	#endif
		
	switch(thispage.state)
	{
		case 0:
			LCD_UpdateStateIcon();
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			LCD_UpdateStateNum();
			iParam[1] = D_LCD_CMD_WriteStateNum;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		#if MainPageVer_190823 == 1
		case 2:
			if(TickOut((uint32 *)&tick, 5000))
			{
				TickOut((uint32 *)&tick, 0x00);
				hmiLcdSoftVer.ctr = SysSoftCtrVersion;
				LCD_SetSoftVersion(hmiLcdSoftVer);
				iParam[1] = D_LCD_CMD_WriteSoftVersion;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			}
			thispage.state = 0;
			break;
		#endif
		default:
			thispage.state = 0;
			break;
	}

	if(hmiSwipeCtr.stepFlag == swiping)
	{
		hmiSwipeCtr.stepFlag = swipeIdle;
	}
}

void HMI_DW_SwipingPage(void)
{/*说明:关于卡片扇区号,块号(函数:M1_SetBlockReq)设计待处理,目前仅参数定义最大范围3个区域数据*/
	static uint32 swipingTick = 0;
	uint32 iParam[3] = {0x01,D_LCD_CMD_JumpScreen,0};
	uint8 comFlag = 0;
	#if Modify_Key_190826 == 1
	LCD_ModifyCard hLcdModifyCard = {0};
	#endif
	
	switch(thispage.state)
	{
		case 0:
			TickOut((uint32 *)&swipingTick, 0x00);
			hmiSwipeCtr.stepFlag = swiping; 
			M1_CardReadCom(0);//初始化数据
			#if Modify_Key_190826 == 1
			hLcdModifyCard.icon = No_CardKeyIcon;
			LCD_SetModifyCardKey(hLcdModifyCard);
			iParam[1] = D_LCD_CMD_WriteModifyKey;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			#endif
			thispage.state++;
			break;
		case 1:
			M1_SetBlockReq(0, 0);//读卡号
			comFlag = M1_CardReadCom(1);
			if(comFlag == M1_COM_STATE_STATER || comFlag == M1_COM_STATE_ACKERR)
			{
				M1_CardReadCom(0);
			}
			#if Modify_Key_190826 == 1
			else if(comFlag == M1_COM_STATE_PWERR)
			{
				M1_CardReadCom(0);
				hLcdModifyCard.icon = CardKey_Cabinet_NoMatch;
				LCD_SetModifyCardKey(hLcdModifyCard);
				iParam[1] = D_LCD_CMD_WriteModifyKey;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			}
			#endif
			else
			{
				if(comFlag == M1_COM_STATE_DONE)
				{
					M1_SetCardData(0);
					M1_CardReadCom(0);
					/*说明:量必达科技协议--关于减值操作--不允许为0,否则操作失败,故通过月卡,扣费两种模式区分*/
					if(param_GetConsumeWay() == Month_Card)
					{
						thispage.state = 4;
					}
					else
					{
						thispage.state++;
					}
				}	
			}
			break;
		case 2:
			M1_SetBlockReq(0, 1);//读卡内余额
			comFlag = M1_CardReadCom(1);
			if(comFlag == M1_COM_STATE_STATER || comFlag == M1_COM_STATE_ACKERR)
			{
				M1_CardReadCom(0);
			}
			else
			{
				if(comFlag == M1_COM_STATE_DONE)
				{
					M1_SetCardData(1);
					M1_CardReadCom(0);
					M1_CardImpairmentCom(0);
					thispage.state++;
				}
			}			
			break;
		case 3:
			M1_SetBlockReq(0, 1);//减值操作--扣除卡内余额
			comFlag = M1_CardImpairmentCom(1);
			if(comFlag == M1_COM_STATE_STATER || comFlag == M1_COM_STATE_ACKERR)
			{
				M1_CardReadCom(0);
			}
			else
			{
				if(comFlag == M1_COM_STATE_DONE)
				{
					M1_SetCardData(1);
					M1_CardImpairmentCom(0);
					thispage.state++;
				}
			}
			break;
		case 4:
			iParam[1] = D_M1_CMD_FindCar;
			SM_SetCmd(D_SM_TASKID_CARD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 5://确认卡片未完成刷卡流程并将卡号绑定至线程
			if(MT_GetRecResult())
			{
				HMI_DW_OptBindingProc();
				iParam[1]  = D_LCD_CMD_JumpScreen;
				LCD_SetScreenID(HMI_DW_PAGE_Tasking);
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				thispage.state++;
				hmi_VocieInfo.triggerFlag.bits.cardFinsh = true;
			}
			else
			{
				thispage.state = 4;
			}
			break;
		default:
			
			break;
	}

	if(TickOut((uint32 *)&swipingTick, 5000))
	{
		hmiSwipeCtr.stepFlag = swipeIdle;
		LCD_SetScreenID(HMI_GetMultiDeepPageId());
		iParam[1] = D_LCD_CMD_JumpScreen;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
		/*说明:优化处理*/
		M1_CardReadCom(0);
		MT_ClearRecResult();
	}
}

void HMI_DW_TaskingPage(void)
{
	uint32 iParam[3] = {0x01,D_M1_CMD_FindCar,0};
	uint8 setPageId = 0;
	
	switch(thispage.state)
	{
		case 0:
			SM_SetCmd(D_SM_TASKID_CARD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;	
		case 1://确认卡片取走
			if(!MT_GetRecResult())
			{
				MT_ClearRecResult();
				hmiSwipeCtr.stepFlag = swipeIdle;
				setPageId = HMI_GetMultiDeepPageId();
				LCD_SetScreenID(setPageId);
				iParam[1] = D_LCD_CMD_JumpScreen;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				thispage.state++;
			}
			else
			{
				thispage.state = 0;
			}
			break;
		default:
			
			break;
	}
}

static void HMI_DW_AdminPwPage(void)
{
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteAdminPwShow,0};
	static uint16 pwIcon = CorrectPw;
	LCDAdminPage hmiLCDAdminPage = {0};
	static LCDAdminPage hmiLCDAdminPage_Back = {0};
	static uint16 highPw[4] = {0x3136,0x3838,0xFFFF,0x2020};
	static uint16 pwSwBat[4] = {0x3136,0x3838,0x3132,0x3334};//修改成:绑卡,解卡页面
	uint8 setPageId = 0;
	#if Modify_PW_190822 == 1
	ParamSysPw hmiParamSysPw = {0};
	#endif
	
	switch(thispage.state)
	{
		case 0:
			pwIcon = CorrectPw;
			LCD_ClearAdminPwShow();
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			LCD_SetAdminPwIcon(pwIcon);
			iParam[1] = D_LCD_CMD_WriteAdminPwIcon;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 2:
			LCD_ClearAdminPwAscii();
			iParam[1] = D_LCD_CMD_WriteAdminPwAscii;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			LCD_GetAdminPw((LCDAdminPage *)&hmiLCDAdminPage_Back);
			thispage.state++;
			break;
		case 3:
			iParam[1] = D_LCD_CMD_ReadAdminPwAscii;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 4:
			LCD_GetAdminPw((LCDAdminPage *)&hmiLCDAdminPage);
			if(0 != memcmp((uint8*)&hmiLCDAdminPage_Back.Ascii[0],(uint8*)&hmiLCDAdminPage.Ascii[0],sizeof(highPw)))
			{
				LCD_SetAdminPwShowXX();
				memcpy((uint8*)&hmiLCDAdminPage_Back.Ascii[0],(uint8*)&hmiLCDAdminPage.Ascii[0],sizeof(highPw));
				iParam[1] = D_LCD_CMD_WriteAdminPwShow;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				while(SM_GetCmd(D_SM_TASKID_LCD,null)) 				
				{
					Sleep(3);
				}			
			}
			
			if(LCD_GetBtnOk())
			{
				while(SM_GetCmd(D_SM_TASKID_LCD,null))
				{
					Sleep(10);
				}
				#if Modify_PW_190822 == 1
				param_GetParamSysPw((ParamSysPw *)&hmiParamSysPw);
				#endif
				if(0==memcmp((uint8*)&hmiLCDAdminPage.Ascii[0],(uint8*)&highPw[0],sizeof(highPw))
					#if Modify_PW_190822 == 1
					|| 0==memcmp((uint8*)&hmiLCDAdminPage.Ascii[0],(uint8*)&hmiParamSysPw.sysAdmin[0],4*sizeof(uint16))
					#endif
				)
				{
					LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
					iParam[1] = D_LCD_CMD_JumpScreen;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					pwIcon = CorrectPw;
					thispage.state++;
				}
				else if(0==memcmp((uint8*)&hmiLCDAdminPage.Ascii[0],(uint8*)&pwSwBat[0],sizeof(pwSwBat))
					#if Modify_PW_190822 == 1
					|| 0 == memcmp((uint8*)&hmiLCDAdminPage.Ascii[0],(uint8*)&hmiParamSysPw.sysFun[0],4*sizeof(uint16))
					#endif
				)
				{/*说明:增加密码取电*//*增加绑卡解卡页面*/
					switch(DoorSysRealOptCntFlag())
					{
						case OptCnt_Succ:
							setPageId = HMI_DW_PAGE_MAIN;
							M1_ClearM1CardData();//用于清卡号数据
							HMI_DW_OptBindingProc();
							break;
						case No_OptDoor:
							setPageId = HMI_DW_PAGE_NoSwOpt;
							break;
						case Exiting_SaveBatOpt:
							setPageId = HMI_DW_PAGE_ExitSaveBatOpt;
							break;
					}
					LCD_SetScreenID(setPageId);
					iParam[1] = D_LCD_CMD_JumpScreen;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					thispage.state++;
				}
				else
				{
					pwIcon = ErrPw;
					thispage.state = 1;
				}
			}
			else
			{
				thispage.state = 3;			
			}
			break;
		default:
			
			break;
	}
}

static void HMI_DW_SetSysParamPage(void)
{
	uint32 iParam[3] = {0x01,D_LCD_CMD_ReadSysParamTime,0x00};
	LCDTime sysSetTime = {0};
	Common_Time time = {0};
	uint8 month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8 tmp[DeviceAddr_MAX*sizeof(uint16)] = {0};
	#if Modify_Key_190826 == 1
	LCD_SysModifyKey himLcdSysModifyKey = {0};
	ParamSysCabinet hmiparamSysCabinet = {0};
	#endif
	
	switch(thispage.state)
	{
		case 0:
			#if Modify_Key_190826 == 1
			 hmiparamSysCabinet = param_GetSysCabinetKey();
			M1_UpdateKeyData((uint8 *)&hmiparamSysCabinet.key[0],0);
			#endif
			param_UpdateSysParamToLCD();
			iParam[1] = D_LCD_CMD_WriteSysParamTime;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_WriteConsumeWay;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;		
			break;
		case 2:
			iParam[1] = D_LCD_CMD_WriteConsumeIcon;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 3:
			iParam[1] = D_LCD_CMD_WriteSingleConsume;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;	
			break;
		case 4:
			iParam[1] = D_LCD_CMD_WriteFullSetTime;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 5:
			iParam[1] = D_LCD_CMD_ReadConsumeWay;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;
		case 6:
			iParam[1] = D_LCD_CMD_ReadSingleConsume;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;				
			break;	
		case 7:
			iParam[1] = D_LCD_CMD_ReadFullSetTime;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 8:
			iParam[1] = D_LCD_CMD_ReadSysParamTime;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 9:
			if(LCD_IsUpdateConsumeWay() || Lcd_IsUpdateSmokeCtr())
			{
				iParam[1] = D_LCD_CMD_WriteConsumeWay;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);

				while(SM_GetCmd(D_SM_TASKID_LCD,null))
				{
					Sleep(10);
				}
				
				iParam[1] = D_LCD_CMD_WriteConsumeIcon;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			}

			if(LCD_IsSingleConsumeChange() || LCD_IsDeviceIdChange())
			{
				if(LCD_GetSingleConsume()>5000 || LCD_IsDeviceIdLegality())//说明:单笔消费max:50元
				{
					if(LCD_IsDeviceIdLegality())
					{
						LCD_SetScreenID(HMI_DW_PAGE_DeviceIdNoLegality);
						iParam[1] = D_LCD_CMD_JumpScreen;
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					}

					LCD_SetSingleConsume(param_GetSingleConsume());
					param_GetDeviceId((uint8 *)&tmp[0]);
					LCD_SetDeviceId((uint8 *)&tmp[0]);
					while(SM_GetCmd(D_SM_TASKID_LCD,null))
					{
						Sleep(10);
					}	
					iParam[1] = D_LCD_CMD_WriteSingleConsume;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				}
				else
				{
					param_SetSingleConsume(LCD_GetSingleConsume());
					param_SaveSingleConsume();
					
					LCD_GetDeviceId((uint8 *)&tmp[0]);
					param_SetDeviceId((uint8 *)&tmp[0]);
					param_SaveDeviceId();
				}
			}

			if(LCD_IsFullSetTimeChange() )
			{
				if(LCD_GetFullSetTime()>600)//说明:设置时间不能超过600min
				{
					LCD_SetFullSetTime(param_GetBatingConversionBatFull());
					#if Modify_Key_190826 == 1
					LCD_SetCabinetKey(param_GetSysCabinetKey());
					#endif
					while(SM_GetCmd(D_SM_TASKID_LCD,null))
					{
						Sleep(10);
					}	
					iParam[1] = D_LCD_CMD_WriteFullSetTime;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				}
				else
				{
					param_SetBatingConversionBatFull(LCD_GetFullSetTime());
					param_SaveFullBatSetTime();
				}
			}

			#if Modify_Key_190826 == 1
			if(LCD_IsKeyChange())
			{
				if(!LCD_IsKeyLegality())
				{
					LCD_SetFullSetTime(param_GetBatingConversionBatFull());
					LCD_SetCabinetKey(param_GetSysCabinetKey());
					while(SM_GetCmd(D_SM_TASKID_LCD,null))
					{
						Sleep(10);
					}	
					iParam[1] = D_LCD_CMD_WriteFullSetTime;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				}
				else
				{
					#if Modify_Key_190826 == 1
					himLcdSysModifyKey = LCD_GetSysModifyKey();
					AsciiConversionByte((uint8 *)&himLcdSysModifyKey.cabinet[0], (uint8 *)&hmiparamSysCabinet.key[0], sizeof(LCD_SysModifyKey)-sizeof(uint16));
					param_SetSysCabinetKey(hmiparamSysCabinet);
					param_SaveParamSysCabinetKey();
					M1_UpdateKeyData((uint8 *)&hmiparamSysCabinet.key[0],0);
					#endif
				}
			}
			#endif
			
			if(LCD_IsUpdateTime())
			{
				LCD_UpdateSysSet();
				LCD_GetSysSetShow((LCDTime *)&sysSetTime);
				time.iYear= (((sysSetTime.year[1]>>8)-'0')*10) + ((sysSetTime.year[1]&0xff)-'0');
				time.ucMonth = (((sysSetTime.month>>8)-'0')*10) + ((sysSetTime.month&0xff)-'0');
				time.ucDay = (((sysSetTime.day>>8)-'0')*10) + ((sysSetTime.day&0xff)-'0');
				time.ucHour = (((sysSetTime.hour>>8)-'0')*10) + ((sysSetTime.hour&0xff)-'0');
				time.ucMin = (((sysSetTime.min>>8)-'0')*10) + ((sysSetTime.min&0xff)-'0');
				time.ucSec = (((sysSetTime.sec>>8)-'0')*10) + ((sysSetTime.sec&0xff)-'0');
				if(((time.ucHour >=24)||(time.ucMin >= 60)||(time.ucSec >= 60)||(time.ucMonth == 0)||
					   (time.ucMonth > 12)||(time.ucDay == 0)||(time.ucDay > month[time.ucMonth-1])))
				{
					LCD_UpdateSysTimeShow();
					
					while(SM_GetCmd(D_SM_TASKID_LCD,null))
					{
						Sleep(10);
					}
					
					iParam[1] = D_LCD_CMD_WriteSysParamTime;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				}
				else
				{
					RTC_Set_time(time);
					/*关于是否增加系统时间保存至EEPROM待分析*/
				}
			}
			thispage.state =5;
			break;
		default:
			
			break;
	}

	#if Modify_Key_190826 == 1
		HMI_DW_ClearModifyKey();
	#endif
}

static void HMI_DW_ChargeRecordPage(void)
{
	static uint16 recordno = 0;
	uint8 i = 0;
	Charge_Record record = {0};
	uint32 iParam[3] = {0x01, D_LCD_CMD_WriteRecord, 0};
	uint16 pagenum=0;
	LCDRecordStateData state;

	switch(thispage.state)
	{
		case 0:
			recordno = 0;
			thispage.state++;
			break;
		case 1:
			for(i=0;i<HMI_DW_RECORD_NUM;i++)
			{
				while(SM_GetCmd(D_SM_TASKID_LCD,null))
				{
					Sleep(10);
				}
				if(ChargeRecordRead(recordno+i, (Charge_Record *)&record))
				{
					LCD_SetRecordData((Charge_Record *)&record, recordno+i+1);
				}
				else
				{
					LCD_ClearRecordData();
				}
				iParam[2] = i;
				SM_SetCmd(D_SM_TASKID_LCD,iParam);
			}
			pagenum = (ChargeGetRecordNo()==0?1:(((ChargeGetRecordNo()-1)/HMI_DW_RECORD_NUM)+1));
			LCD_SetRecordState((recordno/HMI_DW_RECORD_NUM)+1, pagenum, 0, 0);
			iParam[1] = D_LCD_CMD_WriteRecordState;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD,iParam);
			thispage.state++;
			break;
		case 2:
			iParam[1] = D_LCD_CMD_ReadRecordState;
			iParam[2] = 0;
			SM_SetCmd(D_SM_TASKID_LCD,iParam);
			thispage.state++;
			break;
		case 3:
			LCD_GetRecordBtn((uint16 *)&state.lastbtn, (uint16 *)&state.nextbtn);
			if(state.lastbtn)
			{
				if(recordno > HMI_DW_RECORD_NUM)
				{
					recordno -= HMI_DW_RECORD_NUM;
				}
				else
				{
					recordno = 0;
				}
				thispage.state = 1;
			}
			else if(state.nextbtn)
			{
				if((recordno + HMI_DW_RECORD_NUM) < ChargeGetRecordNo())
				{
					recordno += HMI_DW_RECORD_NUM;
					thispage.state = 1;
				}
				else
				{
					thispage.state = 2;
				}
			}
			else
			{
				thispage.state = 2;
			}
			break;
		default:
			thispage.state = 0;
			break;
	}
}

void HMI_DW_NoSwOpt(void)
{
	uint32 iParam[3] = {0x01, D_LCD_CMD_JumpScreen, 0};
	static uint32 itick = 0;

	switch(thispage.state)
	{
		case 0:
			TickOut((uint32 *)&itick, 0x00);
			thispage.state++;
			break;
		default:
			
			break;
	}

	if(TickOut((uint32 *)&itick, 2000))
	{
		TickOut((uint32 *)&itick, 0x00);
		LCD_SetScreenID(HMI_GetMultiDeepPageId());
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
}

static void HMI_DW_PAGE_ClearHistoryRecordPage(void)
{
	uint32 iParam[3] = {0x01, D_LCD_CMD_WriteClearRecordBtn, 0};
	static uint32 itick = 0;

	
	switch(thispage.state)
	{
		case 0:
			TickOut((uint32 *)&itick, 0x00);
			LCD_ClearClearRecordBtnOk();
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadClearRecordBtn;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 2:
			if(LCD_GetClearRecordBtnOk())
			{
				LCD_ClearClearRecordBtnOk();
				chargeClearRecordNo();
				ChargeRecordNoWrite(0);//清充电记录
				iParam[1] = D_LCD_CMD_JumpScreen;
				LCD_SetScreenID(HMI_DW_PAGE_chargeRecordPage);
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				thispage.state++;
			}
			else
			{
				thispage.state = 1;
			}
			break;
		default:
			
			break;
	}
	
	if(TickOut((uint32 *)&itick, 5000))
	{
		TickOut((uint32 *)&itick, 0x00);
		LCD_SetScreenID(HMI_DW_PAGE_chargeRecordPage);
		iParam[1] = D_LCD_CMD_JumpScreen;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
}

static void HMI_DW_PAGE_OneBtnOpenDoorPage(void)
{
	uint32 iParam[3] = {0x01, D_LCD_CMD_WriteOneBtnOpenDoor, 0};
	static uint32 itick = 0;

	
	switch(thispage.state)
	{
		case 0:
			TickOut((uint32 *)&itick, 0x00);
			LCD_ClearClearRecordBtnOk();
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadOneBtnOpenDoor;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 2:
			if(LCD_GetClearOneBtnOpenDoor())
			{
				LCD_ClearClearOneBtnOpenDoor();
				hmiSysParam->oneBtnOpenFlag = 0x01;
				iParam[1] = D_LCD_CMD_JumpScreen;
				LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				thispage.state++;
			}
			else
			{
				thispage.state = 1;
			}
			break;
		default:
			
			break;
	}
	
	if(TickOut((uint32 *)&itick, 15000))
	{
		TickOut((uint32 *)&itick, 0x00);
		LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
		iParam[1] = D_LCD_CMD_JumpScreen;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
}

static void HMI_DW_AppointDoorIdPage(void)
{
	uint32 iParam[3] = {0x01, D_LCD_CMD_WriteAppointDoorId, 0};
	static uint32 itick = 0;
	LCD_OneAppointPage hmiOneAppoint = {0};
	
	switch(thispage.state)
	{
		case 0:
			TickOut((uint32 *)&itick, 0x00);
			LCD_ClearAppointDoorId();
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadAppointDoorId;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 2:
			LCD_GetAppointDoorId(&hmiOneAppoint);
			if(hmiOneAppoint.appointId>ALLDOORNUM || hmiSysParam->appointId.flag/*控制条件:等待前一操作使用指定仓门完成*/)
			{
				thispage.state = 0;							
			}
			else
			{
				if(hmiOneAppoint.ok)
				{
					hmiSysParam->appointId.flag = true;
					hmiSysParam->appointId.doorid = (hmiOneAppoint.appointId-1);
					LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
					iParam[1] = D_LCD_CMD_JumpScreen;
					SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
					thispage.state++;
				}
				else
				{
					thispage.state = 1;
				}
			}	
			break;
		default:
			
			break;
	}
	
	if(TickOut((uint32 *)&itick, 15000))
	{
		TickOut((uint32 *)&itick, 0x00);
		LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
		iParam[1] = D_LCD_CMD_JumpScreen;
		SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
	}
}

void HMI_DW_SoftVersionPage(void)
{
	uint32 iParam[3] = {0x01, D_LCD_CMD_WriteSoftVersion, 0};
	LCD_SoftVersion hmiLcdSoftVer;

	switch(thispage.state)
	{
		case 0:
			hmiLcdSoftVer.ctr = SysSoftCtrVersion;
			LCD_SetSoftVersion(hmiLcdSoftVer);
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		default:
			thispage.state = 0;
			break;
	}
}

#if Modify_PW_190822 == 1
void HMI_DW_ModifyPwPage(void)
{
	uint32  iParam[3] = {0x01, D_LCD_CMD_ReadModifyPWAscii, 0};
	LCD_ModifyPw hmimodifyPwAscii = {0};
	LCD_ModifyPw hmimodifyPwShow = {0};
	ParamSysPw hmiParamSysPw;
	static uint8 iconStatus = CorrectPw;

	switch(thispage.state)
	{
		case 0:
			iconStatus = CorrectPw;
			thispage.state++;
			break;
		case 1:
			LCD_ClearModifyPwData((LCD_ModifyPw *)&hmimodifyPwShow);
			hmimodifyPwShow.icon = iconStatus/*CorrectPw*/;
			LCD_SetModifyPwShowData(hmimodifyPwShow);
			iParam[1] = D_LCD_CMD_WriteModifyPWShow;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 2:
			LCD_ClearModifyPwData((LCD_ModifyPw *)&hmimodifyPwAscii);
			hmimodifyPwAscii.ok = 0;
			LCD_SetModifyPwAsciiData(hmimodifyPwAscii);
			iParam[1] = D_LCD_CMD_WriteModifyPWAscii;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 3:
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 4:
			LCD_ChkPwAsciiModify();
			/*
			LCD_SetModifyPwShowData(hmimodifyPwShow);
			*/
			iParam[1] = D_LCD_CMD_WriteModifyPWShow;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 5:
			LCD_GetModifyPwAsciiData((LCD_ModifyPw *)&hmimodifyPwAscii);
			if(hmimodifyPwAscii.ok == 0x00001)
			{
				if(hmimodifyPwAscii.newPw[0] == hmimodifyPwAscii.rePw[0]
					&& hmimodifyPwAscii.newPw[1] == hmimodifyPwAscii.rePw[1]
					&& hmimodifyPwAscii.newPw[2] == hmimodifyPwAscii.rePw[2]
					&& hmimodifyPwAscii.newPw[3] == hmimodifyPwAscii.rePw[3]
					&& hmimodifyPwAscii.newPw[0]>=0x3000&&hmimodifyPwAscii.rePw[0]>=0x3000)
				{
					param_GetParamSysPw((ParamSysPw *)&hmiParamSysPw);

					if(thispage.id == HMI_DW_PAGE_ModifyPw_Sys)
					{
						memcpy((uint8*)&hmiParamSysPw.sysAdmin[0],(uint8*)&hmimodifyPwAscii.newPw[0],4*sizeof(uint16));
						param_SetParamSysPw(hmiParamSysPw);
						param_SaveParamSysPw();
						LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
						iParam[1] = D_LCD_CMD_JumpScreen;
						SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
						thispage.state++;
					}
					else
					{	
						if(memcmp((uint8*)&hmimodifyPwAscii.newPw[0],(uint8*)&hmiParamSysPw.sysAdmin[0],4*sizeof(uint16)) != 0)
						{
							memcpy((uint8*)&hmiParamSysPw.sysFun[0],(uint8*)&hmimodifyPwAscii.newPw[0],4*sizeof(uint16));	
							param_SetParamSysPw(hmiParamSysPw);
							param_SaveParamSysPw();
							LCD_SetScreenID(HMI_DW_PAGE_SysSetPage);
							iParam[1] = D_LCD_CMD_JumpScreen;
							SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
							thispage.state++;
						}
						else
						{
							iconStatus = TwoPwErr;
							thispage.state = 1;
						}
					}
				}
				else
				{
					iconStatus = TwoPwErr;
					thispage.state = 1;
				}
			}
			else
			{
				thispage.state = 3;
			}
			break;
		default :
			
			break;
	}
}
#endif

#if Modify_Key_190826 == 1
void HMI_DW_ClearModifyKey(void)
{
	static uint8 clearStep = 0;
	uint32  iParam[3] = {0x01, D_LCD_CMD_ReadClearStepBtn, 0};
	LCD_CancelCardKey hmilcdCancelCardKey = {0};	
	ParamSysCabinet pSysCabinet = {0};
	uint8 tmp[4] = {0xFF,0xFF,0xFF,0xFF};

	switch(clearStep)
	{
		case 0:
			LCD_SetCancelModifyCardKey(hmilcdCancelCardKey);
			iParam[1] = D_LCD_CMD_WriteClearStepBtn;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			clearStep++;
			break;
		case 1:
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			clearStep++;
			break;
		case 2:
			hmilcdCancelCardKey = LCD_GetCancelModifyCardKey();
			if(hmilcdCancelCardKey.step == 0x01)
			{
				clearStep = 0;
				thispage.state = 0;
				if(thispage.id == HMI_DW_PAGE_SysSetPage)
				{
					memcpy((uint8*)&pSysCabinet.key[0],(uint8*)&tmp[0],sizeof(ParamSysCabinet));
					param_SetSysCabinetKey(pSysCabinet);
					param_SaveParamSysCabinetKey();
					M1_UpdateKeyData((uint8 *)&tmp[0], 0);
				}
			}
			else
			{
				clearStep = 1;
			}
			break;
		default:
			
			break;
	}
}

void HMI_DW_ModifyCardKey(void)
{
	uint32  iParam[3] = {0x01, D_LCD_CMD_ReadModifyKey, 0};
	static LCD_ModifyCard hmiLcdModifyCard = {0};
	uint8 tmp[4] = {0xFF,0xFF,0xFF,0xFF};
	uint8 tmpp[4] = {0xFF,0xFF,0xFF,0xFF/*0x00,0x00,0x00,0x00*/};
	ParamSysCabinet hmiPSysCabinet = {0};
	static LCD_ModifyCard hmiLcdModifyCard_tmp = {0};
	static uint32 itick = 0;
	uint8 comFlag = 0;
	static uint8 wayOld = 0;
	LCD_ModifyCardChange hmilModifyCardChange = {0};
	LCD_ModifyCardChange noLegality = {0};
	M1_COMDATA hM1ComData = {0};
	static LCD_KeyModeChoice hLcdKeyModeChoice = {0};
	
	switch(thispage.state)
	{
		case 0:	
			memset((uint8*)&hLcdKeyModeChoice.init,0x00,sizeof(LCD_KeyModeChoice));
			LCD_SetKeyModeChoice(hLcdKeyModeChoice);
			LCD_SetModifyCardChange(hmilModifyCardChange);
			wayOld = 0;
			memset((uint8*)&hmiLcdModifyCard.initKey[0],0x00,sizeof(LCD_ModifyCard));
			hmiLcdModifyCard.icon = CardKeyIcon_Choice;
			LCD_SetModifyCardKey(hmiLcdModifyCard);
			thispage.state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_WriteModifyKey;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 2:
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 3:
			hmiLcdModifyCard = LCD_GetModifyCardKey();
			hLcdKeyModeChoice = LCD_GetKeyModeChoice();
			if(LCD_IsCardInitKeyChange() || LCD_IsCardNewKeyChange() || ((hmiLcdModifyCard.way != Card_InitKey) && (wayOld != hmiLcdModifyCard.way)))
			{
				/*说明:异常处理 防止ASCII输入大量数据*/
				if(hmiLcdModifyCard.way > Card_NewInitKey)
				{
					hmiLcdModifyCard.way = Card_InitKey;
				}

				if(hmiLcdModifyCard.ok > 1)
				{
					hmiLcdModifyCard.ok = 0;
				}

				wayOld = hmiLcdModifyCard.way;

				switch(wayOld)
				{
					case Card_OriginalKey:
						hLcdKeyModeChoice.init = Cardkey_Mode_1;
						hLcdKeyModeChoice.re = CardKey_NoMode;
						break;
					case Card_CabinrtKey:
						hLcdKeyModeChoice.init = Cardkey_Mode_2;
						hLcdKeyModeChoice.re = CardKey_NoMode;						
						break;				
					case Card_NewInitKey:
						hLcdKeyModeChoice.init = CardKey_NoMode;
						hLcdKeyModeChoice.re = Cardkey_Mode_2;	
						break;
				}

				hmilModifyCardChange = LCD_GetModifyCardChange();

				if(LCD_IsCardInitKeyLegality())
				{
					hLcdKeyModeChoice.init = Cardkey_Mode_3;

					if(LCD_IsCardNewKeyLegality())
					{
						hLcdKeyModeChoice.re = Cardkey_Mode_1;
					}

					if(hmiLcdModifyCard.way == Card_NewInitKey || LCD_IsCardNewKeyLegality())
					{
						thispage.state = 4;
						break;
					}
					else
					{
						hmiLcdModifyCard.icon = CardKeyIcon_Choice;
						LCD_SetModifyCardKey(hmiLcdModifyCard);
						thispage.state = 1;
					}
				}
				else
				{
					memset((uint8*)&hmiLcdModifyCard.initKey[0],0x00,sizeof(hmiLcdModifyCard.initKey));
					hmiLcdModifyCard.icon = CardKeyIcon_Choice;
					LCD_SetModifyCardKey(hmiLcdModifyCard);
					thispage.state = 1;
					noLegality.initFlag = true;
				}
				
				if(LCD_IsCardNewKeyLegality())
				{
					hLcdKeyModeChoice.re = Cardkey_Mode_1;	

					if(LCD_IsCardInitKeyLegality())
					{
						hLcdKeyModeChoice.init = Cardkey_Mode_3;
					}

					if(hmiLcdModifyCard.way == Card_OriginalKey || hmiLcdModifyCard.way == Card_CabinrtKey || LCD_IsCardInitKeyLegality() )
					{
						thispage.state = 4;
						break;
					}
					else
					{
						hmiLcdModifyCard.icon = CardKeyIcon_Choice;
						LCD_SetModifyCardKey(hmiLcdModifyCard);
						thispage.state = 1;
					}
				}
				else
				{
					memset((uint8*)&hmiLcdModifyCard.newKey[0],0x00,sizeof(hmiLcdModifyCard.newKey));
					hmiLcdModifyCard.icon = CardKeyIcon_Choice;
					LCD_SetModifyCardKey(hmiLcdModifyCard);	
					thispage.state = 1;
					noLegality.newFlag = true;
				}

				if(((noLegality.initFlag == true)&&(hmilModifyCardChange.initFlag == true)) 
					|| ((hmilModifyCardChange.newFlag == true)&& (noLegality.newFlag == true)))
				{
					noLegality.initFlag = false;
					noLegality.newFlag = false;
					LCD_SetModifyCardChange(hmilModifyCardChange);
					hmiLcdModifyCard.icon = CardKeyIcon_NoLegality;
					LCD_SetModifyCardKey(hmiLcdModifyCard);
				}
			}
			else
			{
				thispage.state = 2;
			}
			break;
		case 4:
			hmiLcdModifyCard_tmp = hmiLcdModifyCard;
			switch(hmiLcdModifyCard.way)
			{
				case Card_InitKey:
				case Card_NewInitKey:
					AsciiConversionByte((uint8 *)&hmiLcdModifyCard.initKey[0],(uint8 *)&tmp[0] , sizeof(hmiLcdModifyCard.initKey)-sizeof(uint16));
					break;
				case Card_OriginalKey:
					
					break;
				case Card_CabinrtKey:
					hmiPSysCabinet = param_GetSysCabinetKey();
					memcpy((uint8*)&tmp[0],(uint8*)&hmiPSysCabinet.key[0],sizeof(ParamSysCabinet));
					break;
				default:
					
					break;
			}
			M1_UpdateKeyData((uint8 *)&tmp[0], 0);
			if(hmiLcdModifyCard.way != Card_NewInitKey)
			{
				memset((uint8*)&tmpp[0],0x00,sizeof(tmpp));
				AsciiConversionByte((uint8 *)&hmiLcdModifyCard.newKey[0],(uint8 *)&tmpp[0], sizeof(hmiLcdModifyCard.newKey)-sizeof(uint16));
			}
			M1_UpdateCardKeyData((uint8 *)&tmpp[0], 0);
			thispage.state++;
			break;
		case 5:
			hmiLcdModifyCard_tmp.icon = CardKeyIcon_Opt;
			LCD_SetModifyCardKey(hmiLcdModifyCard_tmp);
			iParam[1] = D_LCD_CMD_WriteModifyKey;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			MT_ClearRecResult();
			thispage.state++;
			break;
		case 6:
			iParam[1] = D_M1_CMD_FindCar;
			SM_SetCmd(D_SM_TASKID_CARD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 7:
			if(MT_GetRecResult())
			{
				M1_CardReadCom(0);
				TickOut((uint32 *)&itick, 0x00);
				hmiLcdModifyCard_tmp.icon = CardKeyModifying;
				LCD_SetModifyCardKey(hmiLcdModifyCard_tmp);
				iParam[1] = D_LCD_CMD_WriteModifyKey;
				SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
				thispage.state++;
			}
			else
			{
				thispage.state = 6;
			}
			break;
		case 8:
			if(TickOut((uint32 *)&itick, 5000) == false)
			{
				M1_SetBlockReq(0, 3);
				comFlag = M1_CardReadCom(1);
				if(comFlag == M1_COM_ModifyCardKey_Ok)
				{
					hmiLcdModifyCard_tmp.icon = CardKeyModifySucc;
					thispage.state++;
				}
				else if(comFlag == M1_COM_STATE_PWERR)
				{
					hmiLcdModifyCard_tmp.icon = CardKeyModifyFail;
					thispage.state++;
				}
			}
			else
			{
				hmiLcdModifyCard_tmp.icon = CardKeyModifyFail;
				thispage.state++;
			}
			break;
		case 9:
			M1_CardReadCom(0);
			LCD_SetModifyCardKey(hmiLcdModifyCard_tmp);
			iParam[1] = D_LCD_CMD_WriteModifyKey;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 10:
			iParam[1] = D_M1_CMD_FindCar;
			SM_SetCmd(D_SM_TASKID_CARD, (uint32 *)&iParam[0]);
			thispage.state++;			
			break;
		case 11:
			hM1ComData = Get_M1_ComData();/*说明:增加此判断条件的目标在于寻卡回复帧慢与从10->11,缘由是Step已清除关于卡片其他操作*/
			if(!MT_GetRecResult() && hM1ComData.result != 0)
			{
				MT_ClearRecResult();
				thispage.state = 5;
			}
			else
			{
				thispage.state = 10;
			}
			break;
		default:
			
			break;
	}

	HMI_DW_ClearModifyKey();
	
	LCD_SetKeyModeChoice(hLcdKeyModeChoice);
	iParam[1] = D_LCD_CMD_WriteKeyModeChoice;
	SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
		
}
#endif

void HMI_DW_OptBindingProc(void)
{
	M1_CARDDATA myM1Card = {0};
	uint8 proc = 0;

	proc = DoorSysGetSetProcessFlagProc();//获取进程ID
	hmiProcessSysCtr[proc].flag = true;
	hmiProcessSysCtr[proc].procId = proc;//用于进程与充电记录绑定
	M1_GetM1CardData((M1_CARDDATA *)&myM1Card);
	//绑定卡号,暂绑8位
	memcpy((uint8*)&hmiProcessSysCtr[proc].cradData.card[0],(uint8*)&myM1Card.block[0].bdata[0],sizeof(hmiProcessSysCtr[proc].cradData.card));
	//绑定充电记录初始值
	ChargeRecordWritePreData(proc, Record_Init, param_GetConsumeWay());
	hmiSysParam->optCnt++;
}

/*
** 20210723
** 备注:比耐配置电池电压
*/
void HMI_DW_Module_VolCur(void){
	uint32 iParam[3] = {0x01,D_LCD_CMD_WriteCfgVolCur,0x00};
	ParamHS pHs = get_HSVolCurLimit();
	int8 setResult = -1;
	
	switch(thispage.state){
		case 0:
			set_ModuleVolCurToLcd(pHs.volLimit,pHs.curLimit);
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 1:
			iParam[1] = D_LCD_CMD_ReadCfgVolCur;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			thispage.state++;
			break;
		case 2:
			setResult = chk_ModuleVolCurCfgIsLegal();
			if(setResult != -1){
				if(setResult == 1){
					thispage.state = 0;
				}else{/*参数配置成功,复位主控,配置模块*/
					Sleep(200);
					NVIC_SystemReset();
				}
			}else{
				thispage.state = 1;
			}
			break;
	}
}

const HMI_DW_PAGE hmi_DW_PageTable[] =
{
	{HMI_DW_PAGE_MAIN,0,HMI_DW_MainPage},
	{HMI_DW_PAGE_Swiping,0,HMI_DW_SwipingPage},
	{HMI_DW_PAGE_Tasking,0,HMI_DW_TaskingPage},
	{HMI_DW_PAGE_AdminPwPage,0,HMI_DW_AdminPwPage},
	{HMI_DW_PAGE_SysSetPage,0,HMI_DW_SetSysParamPage},
	{HMI_DW_PAGE_chargeRecordPage,0,HMI_DW_ChargeRecordPage},
	{HMI_DW_PAGE_NoSwOpt,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_ExitSaveBatOpt,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_OpenIdleErr,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_OpenFullErr,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_NoSaveBat,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_SaveAbnBat,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_OTSaveNorBat,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_OTSaveAbnBat,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_NoFullBat,0,HMI_DW_NoSwOpt},
	{HMI_DW_PAGE_chargeRecordChkPage,0,HMI_DW_ChargeRecordPage},
	{HMI_DW_PAGE_ClearHistoryRecord,0,HMI_DW_PAGE_ClearHistoryRecordPage},
	{HMI_DW_PAGE_OneBtnOpenDoor,0,HMI_DW_PAGE_OneBtnOpenDoorPage},
	{HMI_DW_PAGE_AppointDoorId,0,HMI_DW_AppointDoorIdPage},
	{HMI_DW_PAGE_SoftVersion,0,HMI_DW_SoftVersionPage},
	{HMI_DW_PAGE_DeviceIdNoLegality,0,HMI_DW_NoSwOpt},
	
	#if Modify_PW_190822 == 1
	{HMI_DW_PAGE_ModifyPw_Sys,0,HMI_DW_ModifyPwPage},
	{HMI_DW_PAGE_ModifyPw_Fun,0,HMI_DW_ModifyPwPage},
	#endif

	#if Modify_Key_190826 == 1
	{HMI_DW_PAGE_Modify_Key,0,HMI_DW_ModifyCardKey},
	#endif
	
	{HMI_DW_PAGE_CFG_ModuleVolCur,0,HMI_DW_Module_VolCur}
};
uint8 hmi_DW_PageTableNum = sizeof(hmi_DW_PageTable)/sizeof(HMI_DW_PAGE);

static void HMI_DW_DATA_Init(void)
{
	thispage.id = HMI_DW_PAGE_MAIN;
	thispage.state = 0;
	thispage.handle = hmi_DW_PageTable[0].handle;

	memset((uint8*)&hmiSwipeCtr.stepFlag,0x00,sizeof(HMISwipeCtr));
	hmiProcessSysCtr = GetProcessSysCtr();	
	hmiSysParam = GetSysParam();
	memset((uint8*)&hmi_VocieInfo.triggerFlag.flag,0,sizeof(HMI_VocieInfo));
}

static void HMI_SwipeStepCtr(void)
{
	static uint32 tick = 0;
	uint32 iParam[3] = {0x01,D_M1_CMD_FindCar , 0};
	uint8 setPageId = 0;

	if(hmiSwipeCtr.stepFlag == swipeIdle)
	{

		if(!SM_GetCardComState())
		{
			if(TickOut((uint32 *)&tick, 100))
			{
				TickOut((uint32 *)&tick, 0x00);
				#if Modify_Key_190826 == 1
				if(thispage.id != HMI_DW_PAGE_Modify_Key)	
				#endif
				{
					SM_SetCmd(D_SM_TASKID_CARD, (uint32 *)&iParam[0]);
				}
			}
			hmiSysParam->cardComErr = cardCom_Normal;
		}
		else
		{
			hmiSysParam->cardComErr = cardCom_Abnormal;/*说明:通讯板故障,换板之后需要重启设备*/
		}

		#if Modify_Key_190826 == 1
		if(MT_GetRecResult()&&thispage.id != HMI_DW_PAGE_Modify_Key)
		#else
		if(MT_GetRecResult())
		#endif
		{
			MT_ClearRecResult();
			if(!hmiSysParam->sysDeviceErrFlag)
			{				
				switch(DoorSysRealOptCntFlag())
				{
					case OptCnt_Succ:
						hmiSwipeCtr.stepFlag = idleToSwipe;
						setPageId = HMI_DW_PAGE_Swiping;
						//hmi_VocieInfo.triggerFlag.bits.carding = true;
						break;
					case No_OptDoor:
						setPageId = HMI_DW_PAGE_NoSwOpt;
						hmi_VocieInfo.triggerFlag.bits.noIdleDoor = true;
						break;
					case Exiting_SaveBatOpt:
						setPageId = HMI_DW_PAGE_ExitSaveBatOpt;
						hmi_VocieInfo.triggerFlag.bits.optBusy = true;
						break;
				}
			}
			LCD_SetScreenID(setPageId);			
			iParam[1] = D_LCD_CMD_JumpScreen;
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
		}
	}	
}

static void HMI_DW_UpdateSystemTime(void)
{
	uint32 iParam[3]  = {0x01, D_LCD_CMD_WriteTime, 0}; 
	Common_Time time;

	GetCurrentTime(&time);
	LCD_SetTime(time);
	SM_SetCmd(D_SM_TASKID_LCD,iParam);
}

void HMI_DW_PageSwAccordDoorState(uint8 setPageId)
{
	uint32 iParam[3] = {0x01,D_LCD_CMD_JumpScreen , 0};	

	LCD_SetScreenID(setPageId);			
	SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);	
}

static void HMI_DW_Thread(void *p_arg)
{
	static uint32 btick = 0;
	uint32 iParam[3] = {0x01,D_LCD_CMD_ReadScreenId,0};

	HMI_DW_DATA_Init();
	
	Sleep(3200);/*保证开机空闲查询zy充电模块轮询次数超过3次*/

	/*HMI DW Voice Init  */
	HMI_DW_VoiceDeal();
	
	for(;;)
	{

		HMI_DW_PageDataTableFun();
		
		if(reset_CfgChargerLogic() == true){/*充电器初始化配置完成,才允许刷卡换电*/
			HMI_SwipeStepCtr();
			/*
			** idle chk is exit bat open close module 
			*/			
			idleChk_IsExitBatCtrChgModule();
		}

		/*
		** HMI DW Vocie Info Deal
		*/
		HMI_DW_VocieInfoDeal();
		
		if(TickOut((uint32 *)&btick, 300))
		{
			SM_SetCmd(D_SM_TASKID_LCD, (uint32 *)&iParam[0]);
			TickOut((uint32 *)&btick, 0x00);
			HMI_DW_UpdateSystemTime();
		}
	
		Sleep(20);
	}
}

void HMI_Init(void)
{
    Thread_create(HMI_DW_Thread, 0, &HMI_DW_TaskSTK[1000 - 1], D_HMI_TASK_PRO);
}

