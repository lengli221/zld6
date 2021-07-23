#ifndef DOOR_SYS_LOGIC_H
#define DOOR_SYS_LOGIC_H
#define DoorSys_CloseChkBatTick					(uint32)5000

#include "type.h"

/*ϵͳ����״̬*/
enum
{
	doorIdle = 0x00,//�ſ���(��ز�����������)
	/*��ش���*/
	BatCharging = (uint8)0x01,//����� 
	BatFull = (uint8)0x02,//����
	BatTurnAround = (uint8)0x03,//��ط���
	BatOverTempP = (uint8)0x04,//���±���
	BatOverVolP = (uint8)0x05,//��ѹ����
	BatOnline= (uint8)0x06,/*��ؽ���*/
	batErr = (uint8)0x07,/*��ع���*/
	chargerModuleComErr = 0xFC,/*�����ģ��ͨѶ�쳣*/
	doorOptOverTimeMid = (uint8)0xFD,//���ղֳ�ʱ�쳣�����м���--��ͨѶ���������
	doorOptOverTime = (uint8)0xFE,//���ղֳ�ʱ��ʱ�쳣����
	doorErr = 0xFF,//�����쳣����
};

enum
{
	Exiting_SaveBatOpt = 0x01,//���ر�־
	No_OptDoor = 0x02,//�޲��ſɲ�����־
	OptCnt_Succ = 0x03,//������Ա���ӳɹ���־
};

enum
{
	Process_Idle = 0x00,//���н���
	Process_SaveBat = 0x01,//����ҵ���߼�
	Process_SaveBatOverTime = 0x02,//����ҵ��֮��ʱ�쳣
	Process_ChkExchangeBat = 0x03,//��ѯ�ɻ���ز���ID
	Process_TakeFullBat = 0x04,//ȡ�����ҵ���߼�
};

enum
{
	WaterChk_Nomal = (uint16)0x0000,//ˮλ����
	WaterChk_Abnormal = (uint16)0x0001,//ˮλ�쳣
};

#pragma pack(1)
typedef struct
{
	uint8 card[8];//����
}CardData;

typedef struct
{
	uint8 doorId;//����״̬
}DoorData;

typedef struct
{
	bool flag;//�̱߳�־
	CardData cradData;//���������
	uint8 process;//����
	uint8 procId;//����ID
	DoorData doorData;//�����������
}ProcessSysCtr;

typedef struct
{
	uint8 doorState[ALLDOORNUM];//����״̬
	uint8 closeDoorAbnCnt[ALLDOORNUM];//�ز������������жϲ����쳣 
	Common_Time time[ALLDOORNUM];//��翪ʼʱ��
}DoorSysParam;

typedef struct
{
	bool flag;
	uint8 doorid;
}SysAppointId;//��ָ�����Ų���

typedef struct
{
	uint8 optCnt;//��������
	uint8 oneBtnOpenFlag;//һ����������
	SysAppointId appointId;
	uint16 waterChk;//ˮλ���
	uint16 cardComErr;//ˢ����ͨѶ�쳣
	bool sysDeviceErrFlag;//ϵͳ�豸�쳣--��ͣ,�̸й��ϲ�����ˢ��
	bool sysSmokeFlag;//�̸б�־
	bool sysEPOFlag;//��ͣ��־
}SysParam;

/*
** Abn Deal Logic
*/
typedef struct{
	uint32 Flag;/*�쳣�����־*/
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

