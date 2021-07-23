#ifndef HMI_DW_DATA_H
#define HMI_DW_DATA_H

#define HMI_PAGEDEEP_NUM								20//���ҳ��������20

#define HMI_DW_RECORD_NUM							10//һҳ��ʾ�ļ�¼��

#define HMI_DW_PAGE_MAIN								(uint8)0x00//��ҳ��
#define HMI_DW_PAGE_Swiping							(uint8)0x18//ˢ��ҳ��
#define HMI_DW_PAGE_Tasking							(uint8)0x19//ȡ��ҳ��
#define HMI_DW_PAGE_NoSwOpt							(uint8)0x14//�޷�����ҳ��
#define HMI_DW_PAGE_ExitSaveBatOpt					(uint8)0x15//���ڴ��ز���
#define HMI_DW_PAGE_AdminPwPage					(uint8)0x06//����Ա����ҳ��
#define HMI_DW_PAGE_SysSetPage						(uint8)0x13//ϵͳ����
#define HMI_DW_PAGE_chargeRecordPage				(uint8)0x02//����¼ҳ��
#define HMI_DW_PAGE_OpenIdleErr						(uint8)0x0F//���ղֹ���
#define HMI_DW_PAGE_OpenFullErr						(uint8)0x10//�����ֹ���
#define HMI_DW_PAGE_NoSaveBat						(uint8)0x17//δ������
#define HMI_DW_PAGE_SaveAbnBat						(uint8)0x1B//�����쳣���
#define HMI_DW_PAGE_OTSaveNorBat					(uint8)0x1C//��ʱ�����������
#define HMI_DW_PAGE_OTSaveAbnBat					(uint8)0x1E//��ʱ�����쳣���
#define HMI_DW_PAGE_NoFullBat							(uint8)0x1D//δȡ����
#define HMI_DW_PAGE_chargeRecordChkPage			(uint8)0x1F//�ͻ���ѯ����¼
#define HMI_DW_PAGE_ClearHistoryRecord				(uint8)0x0B//�������¼
#define HMI_DW_PAGE_OneBtnOpenDoor				(uint8)0x12//�����в���
#define HMI_DW_PAGE_AppointDoorId					(uint8)0x1A//��ָ����
#define HMI_DW_PAGE_SoftVersion						(uint8)0x04//����汾��

#define HMI_DW_PAGE_DeviceIdNoLegality				(uint8)0x2B//�豸ID�Ƿ���

#if Modify_PW_190822 == 1
#define HMI_DW_PAGE_ModifyPw_Sys					(uint8)0x07//�޸�ϵͳ����ҳ��
#define HMI_DW_PAGE_ModifyPw_Fun					(uint8)0x2C//�޸Ĺ�������ҳ��
#endif

#if Modify_Key_190826 == 1
#define HMI_DW_PAGE_Modify_Key						(uint8)0x2D//�޸Ŀ�Ƭ��Կ
#endif

enum
{
	cardCom_Normal = (uint16)0x0000,//ˢ��������
	cardCom_Abnormal = (uint16)0x0001,//ˢ�����쳣
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

/*---------------------------------������־------------------------------------------------*/
typedef union{
	uint32 flag;
	struct{
		uint32 idleDoorOpen:1;/*���в����Ѵ�*/
		uint32 fullDoorOpen:1;/*���ֲ����Ѵ�*/
		uint32 carding:1;/*ˢ����*/
		uint32 cardFinsh:1;/*ˢ�����*/
		uint32 comErr:1;/*ͨѶ�쳣*/
		uint32 doorNoClose:1;/*����δ��*/
		uint32 batNoSave:1;/*���δ����*/
		uint32 openIdleDoorFail:1;/*����ʧ��*/
		uint32 noIdleDoor:1;/*�޿��в�*/
		uint32 noExceBat:1;/*�޿��õ��*/
		uint32 openFullDoorFail:1;/*������ʧ��*/
		uint32 optBusy:1;/*������æ��*/
		uint32 saveErrBat:1;/*�����쳣���*/
		uint32 fullNoTake:1;/*����δȡ��*/
		uint32 saveBatOutTime:1;/*�����쳣*/
		uint32 swing:1;/*������*/
		uint32 swFail:1;/*����ʧ��*/
		uint32 res:15;/*����*/
	}bits;
}VoiceTriggerFlag;

/*
** HMI Vocie Info
*/
typedef struct{
	VoiceTriggerFlag triggerFlag;/*������־*/
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

