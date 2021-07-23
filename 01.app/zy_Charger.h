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

#define ZY_Charger_BaseCurValue											1000/*10A--放大系数:100*/
#define ZY_Charger_BaseVolValue												480/*48V--放大系数:10*/

/*
** 振源--充电器结构体
*/
typedef struct{
	uint8 addr;/*地址*/
	uint8 funCode;/*功能码*/
	uint16 registerAddr;/*寄存器起始地址*/
	uint16 registerNum;/*寄存器数量*/
	uint8 dataLen;/*数据字节数量*/
	uint8 data[256];/*数据项*/
}ZY_Charger;

/*
** 振源设置项
*/
typedef struct{
	uint16 setVol;/*设置恒压电压*/
	uint16 setCur;/*设置输出电流*/
	uint16 moduleOnOff;/*模块开关机命令*/
	bool volSetSuccFlag;/*电压设定成功标志*/
	bool curSetSuccFlag;/*电流设定成功标志*/
	bool onOffSetSuccFlag;/*开关机设定成功标志*/
	bool resetBatOnline;/*复位电池在线标志*/
}ZY_SetItem;

/*
** 振源故障信息
*/
typedef union{
	uint16 flag;
	struct{
		uint16 outputVolH:1;/*输出电压高*/
		uint16 outVolAvgH:1;/*输出电压平均值高*/
		uint16 batVolH:1;/*电池电压高*/
		uint16 batVolAvgH:1;/*电池电压平均值高*/
		uint16 batVolAvgL:1;/*电池电压平均值低*/
		uint16 batAT:1;/*电池反接*/
		uint16 batVolAb:1;/*电池电压异常*/
		uint16 outputOC:1;/*输出过流*/
		uint16 outputCurAvgH:1;/*输出电流平均值高*/
		uint16 chargerH:1;/*充电器温度过高*/
		uint16 res:6;
	}bits;
}ZY_Err;

/*
** 振源获取项
*/
typedef struct{
	uint16 chgCur;/*充电电流*/
	uint16 batVol;/*电池电压*/
	uint16 outputVol;/*输出电压*/
	ZY_Err zyErr;/*振源故障信息*/
	uint16 state;/*状态信息*/
}ZY_GetItem;

/*
** 振源--充电器信息
*/
typedef struct{
	ZY_SetItem zySetItem;/*振源设置项*/
	ZY_GetItem zyGetItem;/*振源获取项*/
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

