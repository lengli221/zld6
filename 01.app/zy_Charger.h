#ifndef __ZY_CHARGER_H_
#define __ZY_CHARGER_H_

#include "type.h"

#define D_ZY_CMD_Get_State													0x01
#define D_ZY_CMD_Set_VoltCur												0x02
#define D_ZY_CMD_Set_OnOff													0x03
#define D_ZY_CMD_GetSetValue												0x04
#define D_ZY_CMD_GetSetOnOff												0x05

#define get_State_ZY_Charger_RegisterAddr								(uint16)400
#define get_State_ZY_Charger_RegisterNum								(uint16)10


#define set_ZY_Charger_VoltCur_RegisterAddr								(uint16)200
#define set_ZY_Charger_VolCur_RegisterNum								(uint16)4

#define set_ZY_Charger_OnOff_RegisterAddr								(uint16)204
#define set_ZY_Charger_OnOff_RegisterNum								(uint16)1

#define ZY_Charger_BaseCurValue											1000/*10A--�Ŵ�ϵ��:100*/
#define ZY_Charger_BaseVolValue												480/*48V--�Ŵ�ϵ��:10*/

/*
** ��Դ--������ṹ��
*/
typedef struct{
	uint8 addr;/*��ַ*/
	uint8 funCode;/*������*/
	uint16 registerAddr;/*�Ĵ�����ʼ��ַ*/
	uint16 registerNum;/*�Ĵ�������*/
	uint8 dataLen;/*�����ֽ�����*/
	uint8 data[256];/*������*/
}ZY_Charger;

/*
** ��Դ������
*/
typedef struct{
	uint16 setVol;/*���ú�ѹ��ѹ*/
	uint16 setCur;/*�����������*/
	uint16 moduleOnOff;/*ģ�鿪�ػ�����*/
	bool volSetSuccFlag;/*��ѹ�趨�ɹ���־*/
	bool curSetSuccFlag;/*�����趨�ɹ���־*/
	bool onOffSetSuccFlag;/*���ػ��趨�ɹ���־*/
	bool resetBatOnline;/*��λ������߱�־*/
}ZY_SetItem;

/*
** ��Դ������Ϣ
*/
typedef union{
	uint16 flag;
	struct{
		uint16 outputVolH:1;/*�����ѹ��*/
		uint16 outVolAvgH:1;/*�����ѹƽ��ֵ��*/
		uint16 batVolH:1;/*��ص�ѹ��*/
		uint16 batVolAvgH:1;/*��ص�ѹƽ��ֵ��*/
		uint16 batVolAvgL:1;/*��ص�ѹƽ��ֵ��*/
		uint16 batAT:1;/*��ط���*/
		uint16 batVolAb:1;/*��ص�ѹ�쳣*/
		uint16 outputOC:1;/*�������*/
		uint16 outputCurAvgH:1;/*�������ƽ��ֵ��*/
		uint16 chargerH:1;/*������¶ȹ���*/
		uint16 res:6;
	}bits;
}ZY_Err;

/*
** ��Դ��ȡ��
*/
typedef struct{
	uint16 chgCur;/*������*/
	uint16 batVol;/*��ص�ѹ*/
	uint16 outputVol;/*�����ѹ*/
	ZY_Err zyErr;/*��Դ������Ϣ*/
	uint16 state;/*״̬��Ϣ*/
}ZY_GetItem;

/*
** ��Դ--�������Ϣ
*/
typedef struct{
	ZY_SetItem zySetItem;/*��Դ������*/
	ZY_GetItem zyGetItem;/*��Դ��ȡ��*/
}ZY_Charger_Info;

/*
** init ZY Charger Para
*/
void init_ZYChargerPara(void);
/*
** zy Parse Charger Info
*/
bool zy_ParseChargerInfo(void* pData,void* dev);
/*
** zy State Charger Get Info
*/
bool zy_State_Charger_GetInfo(void* pData,uint8 dev);
/*
** zy Charger Set Value Get
*/
bool zy_ChargerSetValueGet(void* pData,uint8 dev);
/*
** zy Charger Set OnOff Get
*/
bool zy_ChargerSetOnOffGet(void* pData,uint8 dev);
/*
** set ZY Charger Vol Cur
*/
void set_ZY_Charger_VolCur(float vol,float cur,uint8 dev);
/*
** zy Charger Set Volt Cur
*/
bool zy_Charger_SetVoltCur(void* pData,uint8 dev);
/*
** get ZY Charger OnOff Cmd Exe State 
*/
bool get_ZYChargerOnOffCmdExeState(uint8 dev);
/*
** set ZY Charger OnOff
*/
void set_ZY_ChargerOnOff(bool onoff,uint8 dev);
/*
** zy Charger Set OnOff
*/
bool zy_Charger_SetOnOff(void* pData,uint8 dev);
/*
** reset Cfg Charger Logic
*/
bool reset_CfgChargerLogic(void);

#endif

