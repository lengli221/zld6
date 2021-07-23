#include "includes.h"

ZY_Charger_Info zyChargerInfo[ALLDOORNUM] = {0};

DoorSysParam* zy_DoorSysPara = null;
ChargerTime *chgChargerTime = null;

/*
** init ZY Charger Para
*/
void init_ZYChargerPara(void){
	uint8 i = 0;

	zy_DoorSysPara = GetDoorSysParam();
	chgChargerTime = GetChargerTime();
	for(i=0;i<ALLDOORNUM;i++){
		TickOut((uint32 *)&chgChargerTime->itick[i], 0);
	}
}

/*
** update door state info
*/
void update_DoorStateInfo(uint8 dev){
	static bool chgingTimeUpdateFlag[ALLDOORNUM] = {0};
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_OnOff,0};/*�ػ�ģ��*/
	static uint8 closeModuleCnt[ALLDOORNUM] = {0};
	static uint32 closeModuleTick[ALLDOORNUM] = {0};

	if(zy_DoorSysPara->doorState[dev] != doorErr && zy_DoorSysPara->doorState[dev] != doorOptOverTime){
		/*������Ƿ�����*/
		if(zyChargerInfo[dev].zyGetItem.batVol >= 350){/*��ص�ѹ���ڱ���:10*/
			zy_DoorSysPara->doorState[dev] = BatOnline;
			/*�����ϵ縴λ������߱�־*/
			zyChargerInfo[dev].zySetItem.resetBatOnline = true;
		}else{
			zy_DoorSysPara->doorState[dev] = doorIdle;
			/*���õ�ز����߱�־*/
			zyChargerInfo[dev].zySetItem.resetBatOnline = false;
		}

		if(zy_DoorSysPara->doorState[dev] == BatOnline){
			if(zyChargerInfo[dev].zyGetItem.zyErr.flag == 0){/*�޵�ع���*/
				switch(zyChargerInfo[dev].zyGetItem.state){/*״̬��Ϣ*/
					case 1:/*׼�������-- �����Ϊ�������*/
						
						break;
					case 2:/*�����*/
						zy_DoorSysPara->doorState[dev] = BatCharging;
						break;
					case 3:/*����*/
						zy_DoorSysPara->doorState[dev] = BatFull;
						break;
				}	
			}else{
				if(zyChargerInfo[dev].zyGetItem.zyErr.bits.batAT == true){
					zy_DoorSysPara->doorState[dev] = BatTurnAround;
				}else if(zyChargerInfo[dev].zyGetItem.zyErr.bits.chargerH == true){
					zy_DoorSysPara->doorState[dev] = BatOverTempP;
				}else if(zyChargerInfo[dev].zyGetItem.zyErr.bits.outputVolH == true){
					zy_DoorSysPara->doorState[dev] = BatOverVolP;
				}else{
						zy_DoorSysPara->doorState[dev] = batErr;
				}
			}
		}
		
		if((zy_DoorSysPara->doorState[dev] == BatCharging || zy_DoorSysPara->doorState[dev] == BatFull)
			&& chgingTimeUpdateFlag[dev] == false){
			chgingTimeUpdateFlag[dev] = true;
			GetCurrentTime((Common_Time *)&zy_DoorSysPara->time[dev]);/*�������ʼ���ʱ��*/
		}else{
			if(zy_DoorSysPara->doorState[dev] == doorIdle){
				chgingTimeUpdateFlag[dev] = false;
				memset((uint8*)&zy_DoorSysPara->time[dev].iYear,0,sizeof(Common_Time));
				memset((uint8*)&chgChargerTime->min[dev],0,sizeof(uint32));
				if(TickOut((uint32 *)&chgChargerTime->itick[dev], DoorSys_CloseChkBatTick)){
					chgChargerTime->lastMin[dev] = 0;
				}
			}
		}
	}
	
	/*
	** 20210723
	** ��ע:���͵����zyģ����,�ױ�������ѹ������-->����ģ�鷴������,
	**	�����عػ�ģ�� -->ģ�鱨������ѹ������-->ת����-->�ػ�ģ��
	*/	
	if(zyChargerInfo[dev].zyGetItem.zyErr.bits.outputVolH != 0){
		if(zy_DoorSysPara->doorState[dev] != doorErr && zy_DoorSysPara->doorState[dev] != doorOptOverTime){
			zy_DoorSysPara->doorState[dev] = BatFull;
		}
		
		if(closeModuleCnt[dev] < 5){
			if(TickOut((uint32*)&closeModuleTick[dev],200) == true){
				TickOut((uint32*)&closeModuleTick[dev],0);
				/*set ZY Charger OnOff*/
				set_ZY_ChargerOnOff(false, dev);
				iParam[2] = dev;
				SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);	
				closeModuleCnt[dev]++;
			}	
		}

		/*��ע:����ذγ�,�ſ��Ը���"����ѹ����"��־λ*/
		if(zyChargerInfo[dev].zyGetItem.batVol < 350){/*��ز�����*/
			closeModuleCnt[dev] = 0;
			TickOut((uint32*)&closeModuleTick[dev],0);
			/*��zyģ��״̬��Ϣ*/
			memset((uint8*)&zyChargerInfo[dev].zyGetItem.zyErr.flag,0,sizeof(ZY_Err));
		}	
	}
}

/*
** zy Parse Charger Get State Info
*/
static void zy_Parse_ChargerGetStateInfo(uint8* data,uint8 datalen,uint8 dev){
	uint8 offset = 0;
	uint16 temp = 0;
	uint16 temp1 = 0;
	uint32 tempTrans = 0; 
	
	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	if((int16)temp > 0){
		temp1 = little_Big_Endian__Exchange(temp);
		tempTrans = temp1*ZY_Charger_BaseCurValue;
		zyChargerInfo[dev].zyGetItem.chgCur = (uint16)(tempTrans/4096);
	}else{
		zyChargerInfo[dev].zyGetItem.chgCur = 0;
	}
	offset += sizeof(uint16);
	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	temp1 = little_Big_Endian__Exchange(temp);
	tempTrans = temp1*ZY_Charger_BaseVolValue;
	zyChargerInfo[dev].zyGetItem.batVol = (uint16)(tempTrans/4096);
	offset += sizeof(uint16);
	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	temp1 = little_Big_Endian__Exchange(temp);
	tempTrans = temp1*ZY_Charger_BaseVolValue;	
	zyChargerInfo[dev].zyGetItem.outputVol = (uint16)(tempTrans/4096);
	offset += sizeof(uint16);

	offset += 4;/*����ƫ����*/

	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	/*
	**	20210723
	** ��ע:��ܵ����"��ѹ����"�������ģ�鷴�����ػ�,��������ѹ����֮��,��ֹ����״̬
	*/
	if(zyChargerInfo[dev].zyGetItem.zyErr.bits.outputVolH == 0){
		zyChargerInfo[dev].zyGetItem.zyErr.flag = little_Big_Endian__Exchange(temp);
	}
	offset += sizeof(uint16);	

	offset += 6;/*����ƫ����*/	

	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	zyChargerInfo[dev].zyGetItem.state = little_Big_Endian__Exchange(temp);
	offset += sizeof(uint16);	

	/*update door state info*/
	update_DoorStateInfo(dev);
}

/*
** zy Parse Charger Get Set Value 
*/
static void zy_ParseChargerGetSetValue(uint8* data,uint8 datalen,uint8 dev){
	uint8 offset = 0;
	uint16 temp = 0;
	uint16 compareTemp = 0;

	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	compareTemp =  little_Big_Endian__Exchange(temp);
	offset += sizeof(uint16);	
	/*compare vol*/
	if(compareTemp == zyChargerInfo[dev].zySetItem.setVol){
		zyChargerInfo[dev].zySetItem.volSetSuccFlag = true;
	}else{
		zyChargerInfo[dev].zySetItem.volSetSuccFlag = false;
	}

	offset += 4;/*����ƫ����*/

	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	compareTemp =  little_Big_Endian__Exchange(temp);
	offset += sizeof(uint16);	

	/*compare cur*/
	if(compareTemp == zyChargerInfo[dev].zySetItem.setCur){
		zyChargerInfo[dev].zySetItem.curSetSuccFlag = true;
	}else{
		zyChargerInfo[dev].zySetItem.curSetSuccFlag = false;
	}	
}

/*
** zy Parse Charger Get Set On Off
*/
static void zy_ParseChargerGetSetOnOff(uint8* data,uint8 datalen,uint8 dev){
	uint8 offset = 0;
	uint16 temp = 0;
	uint16 compareTemp = 0;

	memcpy((uint8*)&temp,(uint8*)&data[offset],sizeof(uint16));
	compareTemp =  little_Big_Endian__Exchange(temp);
	offset += sizeof(uint16);	

	/*compare OnOff*/
	if(compareTemp == zyChargerInfo[dev].zySetItem.moduleOnOff){
		zyChargerInfo[dev].zySetItem.onOffSetSuccFlag = true;/*���ػ�ָ��һ����*/
	}else{
		zyChargerInfo[dev].zySetItem.onOffSetSuccFlag = false;;/*���ػ�ָ�һ��*/
	}
}

/*
** zy Parse Charger GetInfo
*/
static void zy_Parse_ChargerGetInfo(uint8* data,uint8 datalen,uint8 dev){
	switch(datalen){
		case 20:/*�Ĵ�����ַ:400-409-->��ȡģ�����*/
			zy_Parse_ChargerGetStateInfo(data, datalen, dev);
			break;
		case 8:/*�Ĵ�����ַ:200-203-->��ȡ�趨ֵ:��ѹ/����*/
			zy_ParseChargerGetSetValue(data, datalen, dev);
			break;
		case 2:/*�Ĵ�����ַ:204-->���ػ�*/
			zy_ParseChargerGetSetOnOff(data, datalen, dev);
			break;
	}
}

/*
** zy_ParseChargerSetInfo
*/
static void zy_ParseChargerSetInfo(uint16 regAddr,uint8 dev){
	switch(regAddr){
		case set_ZY_Charger_OnOff_RegisterAddr:
			zyChargerInfo[dev].zySetItem.onOffSetSuccFlag = true;/*���ػ�ָ��һ����*/
			break;
	}
}

/*
** zy Parse Charger Info
*/
bool zy_ParseChargerInfo(void* pData,void* dev){
	ZY_Charger* zy_Charger = (ZY_Charger*)pData;

	if(zy_Charger == null){
		return false;
	}
	
	switch(zy_Charger->funCode){
		case get_ZY_Charger_FunCode:
			zy_Parse_ChargerGetInfo((uint8 *)&zy_Charger->data[0], zy_Charger->dataLen,  zy_Charger->addr);
			break;
		case set_ZY_Charger_FunCode:
			zy_ParseChargerSetInfo(zy_Charger->registerAddr,zy_Charger->addr);
			break;
	}

	return true;
}

/*
** zy State Charger Get Info
*/
bool zy_State_Charger_GetInfo(void* pData,uint8 dev){
	ZY_Charger* zy_Charger = (ZY_Charger*)pData;

	if(pData == null){
		return false;
	}

	zy_Charger->addr = dev;
	zy_Charger->funCode = get_ZY_Charger_FunCode;
	zy_Charger->registerAddr = get_State_ZY_Charger_RegisterAddr;
	zy_Charger->registerNum = get_State_ZY_Charger_RegisterNum;

	return true;
}

/*
** zy Charger Set Value Get
*/
bool zy_ChargerSetValueGet(void* pData,uint8 dev){
	ZY_Charger* zy_Charger = (ZY_Charger*)pData;

	if(pData == null){
		return false;
	}

	zy_Charger->addr = dev;
	zy_Charger->funCode = get_ZY_Charger_FunCode;
	zy_Charger->registerAddr = set_ZY_Charger_VoltCur_RegisterAddr;
	zy_Charger->registerNum = set_ZY_Charger_VolCur_RegisterNum;

	return true;	
}

/*
** zy Charger Set OnOff Get
*/
bool zy_ChargerSetOnOffGet(void* pData,uint8 dev){
	ZY_Charger* zy_Charger = (ZY_Charger*)pData;

	if(pData == null){
		return false;
	}

	zy_Charger->addr = dev;
	zy_Charger->funCode = get_ZY_Charger_FunCode;
	zy_Charger->registerAddr = set_ZY_Charger_OnOff_RegisterAddr;
	zy_Charger->registerNum = set_ZY_Charger_OnOff_RegisterNum;

	return true;		
}

/*
** set ZY Charger Vol Cur
*/
void set_ZY_Charger_VolCur(float vol,float cur,uint8 dev){
	zyChargerInfo[dev].zySetItem.setVol = (uint16)(vol*10);
	zyChargerInfo[dev].zySetItem.setCur = (uint16)(cur * 100);
}

/*
** zy Charger Set Volt Cur
*/
bool zy_Charger_SetVoltCur(void* pData,uint8 dev){
	ZY_Charger* zy_Charger = (ZY_Charger*)pData;
	uint8 offset = 0;
	uint16 temp = 0;

	if(pData == null){
		return false;
	}

	zy_Charger->addr = dev;
	zy_Charger->funCode = set_ZY_Charger_FunCode;
	zy_Charger->registerAddr = set_ZY_Charger_VoltCur_RegisterAddr;
	zy_Charger->registerNum = set_ZY_Charger_VolCur_RegisterNum;
	zy_Charger->dataLen = (uint8)(set_ZY_Charger_VolCur_RegisterNum * 2);
	/*copy data*/
	temp = little_Big_Endian__Exchange(zyChargerInfo[dev].zySetItem.setVol);
	memcpy((uint8*)&zy_Charger->data[offset],(uint8*)&temp,sizeof(uint16));
	offset += sizeof(uint16);
	memset((uint8*)&zy_Charger->data[offset],0,4);
	offset += 4;
	temp = little_Big_Endian__Exchange(zyChargerInfo[dev].zySetItem.setCur);
	memcpy((uint8*)&zy_Charger->data[offset],(uint8*)&temp,sizeof(uint16));
	offset += sizeof(uint16);

	return true;	
}

/*
** get ZY Charger OnOff Cmd Exe State 
*/
bool get_ZYChargerOnOffCmdExeState(uint8 dev){
	return zyChargerInfo[dev].zySetItem.onOffSetSuccFlag;
}

/*
** set ZY Charger OnOff
*/
void set_ZY_ChargerOnOff(bool onoff,uint8 dev){
	if(onoff == true){
		zyChargerInfo[dev].zySetItem.moduleOnOff = 1;/*����*/
	}else{
		zyChargerInfo[dev].zySetItem.moduleOnOff = 0;/*�ػ�*/
	}
	/*���ÿ��ػ����ñ�־*/
	zyChargerInfo[dev].zySetItem.onOffSetSuccFlag = false;
}

/*
** zy Charger Set OnOff
*/
bool zy_Charger_SetOnOff(void* pData,uint8 dev){
	ZY_Charger* zy_Charger = (ZY_Charger*)pData;
	uint16 temp = 0;

	if(pData == null){
		return false;
	}

	zy_Charger->addr = dev;
	zy_Charger->funCode = set_ZY_Charger_FunCode;
	zy_Charger->registerAddr = set_ZY_Charger_OnOff_RegisterAddr;
	zy_Charger->registerNum = set_ZY_Charger_OnOff_RegisterNum;
	zy_Charger->dataLen = (uint8)(set_ZY_Charger_OnOff_RegisterNum * 2);
	/*copy*/
	temp = little_Big_Endian__Exchange(zyChargerInfo[dev].zySetItem.moduleOnOff);
	memcpy((uint8*)&zy_Charger->data[0],(uint8*)&temp,sizeof(uint16));

	return true;
}

/*
** reset Cfg Charger Logic
*/
#if Debug_Info == 1
uint32 tickNum =0;
#endif
bool reset_CfgChargerLogic(void){
	static bool resetFlag = false;
	static uint8 circNum = 0;
	static uint8 deviceAddr = 0;
	ParamHS paramHS_Temp = get_HSVolCurLimit();
	uint32 iParam[3] = {0x01,D_ZY_CMD_Set_VoltCur,0};
	#if Debug_SinglePoint == 1
	uint8 deviceAddrNum = 1;
	#else 
	uint8 deviceAddrNum = ALLDOORNUM;
	#endif

	if(resetFlag == false){
		#if Debug_Info == 1
		tickNum = OSTimeGet();
		#endif
		for(circNum = 0;circNum < 3;circNum++){	
			/*HMI DW Voice Init  */
			HMI_DW_VoiceDeal();
			for(deviceAddr=0;deviceAddr < deviceAddrNum;deviceAddr++){
				if(zyChargerInfo[deviceAddr].zySetItem.volSetSuccFlag == false
					|| zyChargerInfo[deviceAddr].zySetItem.curSetSuccFlag == false){/*��ѹ����δ���óɹ�*/
					watchdogUpdate();
					set_ZY_Charger_VolCur(paramHS_Temp.volLimit, paramHS_Temp.curLimit, deviceAddr);
					iParam[1] = D_ZY_CMD_Set_VoltCur;/*���õ�ѹ����*/
					iParam[2] = deviceAddr;
					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
					while(SM_GetCmd(D_SM_TASKID_CARD,null)){/*��������Ƿ������*/
						watchdogUpdate();
						Sleep(2);
					}
				}
			}			

			if(zyChargerInfo[deviceAddr].zySetItem.volSetSuccFlag == false
				|| zyChargerInfo[deviceAddr].zySetItem.curSetSuccFlag == false){
				Sleep(60);/*�ȴ��ظ����ݽ������*/
			}
			
			for(deviceAddr=0;deviceAddr < deviceAddrNum;deviceAddr++){
				if(zyChargerInfo[deviceAddr].zySetItem.volSetSuccFlag == false
					|| zyChargerInfo[deviceAddr].zySetItem.curSetSuccFlag == false){/*��ѹ����δ���óɹ�*/
					watchdogUpdate();
					iParam[1] = D_ZY_CMD_GetSetValue;/*��ȡ��ѹ������������*/
					iParam[2] = deviceAddr;
					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
					while(SM_GetCmd(D_SM_TASKID_CARD,null)){/*��������Ƿ������*/
						watchdogUpdate();
						Sleep(2);
					}
				}
			}

			if(zyChargerInfo[deviceAddr].zySetItem.volSetSuccFlag == false
				|| zyChargerInfo[deviceAddr].zySetItem.curSetSuccFlag == false){
				Sleep(60);/*�ȴ��ظ����ݽ������*/
			}
		}

		for(circNum = 0;circNum < 3;circNum++){	
			/*HMI DW Voice Init  */
			HMI_DW_VoiceDeal();
			for(deviceAddr=0;deviceAddr < deviceAddrNum;deviceAddr++){
				if(zyChargerInfo[deviceAddr].zySetItem.onOffSetSuccFlag == false){/*���ÿ��ػ���־*/
					watchdogUpdate();
					if(zyChargerInfo[deviceAddr].zySetItem.resetBatOnline == true){/*��λ�������*/
						set_ZY_ChargerOnOff(true, deviceAddr);
					}else{
						set_ZY_ChargerOnOff(false, deviceAddr);
					}
					iParam[1] = D_ZY_CMD_Set_OnOff;/*���ÿ��ػ�*/
					iParam[2] = deviceAddr;
					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
					while(SM_GetCmd(D_SM_TASKID_CARD,null)){/*��������Ƿ������*/
						watchdogUpdate();
						Sleep(2);
					}				
				}
			}

			if(zyChargerInfo[deviceAddr].zySetItem.onOffSetSuccFlag == false){
				Sleep(80);/*�ȴ��ظ����ݽ������*/
			}
				/*��ע:����zy ë������--��ѯ�����Ĵ�����Ч,���޸ĳ�ȷ�ϳ��ģ���ѽӿڿ��ػ�ָ���*/
// 			for(deviceAddr=0;deviceAddr < deviceAddrNum;deviceAddr++){
// 				if(zyChargerInfo[deviceAddr].zySetItem.onOffSetSuccFlag == false){/*���ÿ��ػ���־*/
// 					watchdogUpdate();
// 					iParam[1] = D_ZY_CMD_GetSetOnOff;/*��ȡ���ÿ��ػ�״̬��Ϣ*/
// 					iParam[2] = deviceAddr;
// 					SM_SetCmd(D_SM_ZY_Charger, (uint32 *)&iParam[0]);
// 					while(SM_GetCmd(D_SM_TASKID_CARD,null)){/*��������Ƿ������*/
// 						watchdogUpdate();
// 						Sleep(2);
// 					}				
// 				}
// 			}

// 			if(zyChargerInfo[deviceAddr].zySetItem.onOffSetSuccFlag == false){
// 				Sleep(60);/*�ȴ��ظ����ݽ������*/
// 			}
		}
		resetFlag = true;
		#if Debug_Info == 1
		tickNum = OSTimeGet() - tickNum;
		#endif
		/*�������ʧЧ--�Ƿ����״̬*/
	}
	return resetFlag;
}

