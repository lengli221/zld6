#ifndef LCDDATA_DW_H
#define LCDDATA_DW_H

#include "type.h"

/*
** II������ҳ�����
*/
#define LCD_ADDR_WriteId								(uint16)(0x0084)
#define LCD_ADDR_Len_WriteId						(uint8)(2)
#define LCD_ADDR_ReadId								(uint16)(0x0014)
#define LCD_ADDR_Len_ReadId						(uint8)(1)
#define LCD_ADDR_WriteVoice							(uint16)(0x00A0)
#define LCD_ADDR_Len_WriteVoice					(uint8)(2)

/*
** App Ӧ�ò��ַ+�����"һ����"����ƫ�Ƶ�ַ:0x1000
*/
#define LCD_WriteTimeAddr								(uint16)0x1100
#define LCD_Write_Len_TimeAddr						(uint8)0x07

#define LCD_WriteStateIconAddr						(uint16)0x1110
#define LCD_Write_Len_StateIconAddr				(uint8)ALLDOORNUM

#define LCD_WriteNumAddr								(uint16)0x1120
#define LCD_Write_Len_NumAddr						(uint8)0x04

#define LCD_WriteAdminPwShowAddr					(uint16)0x1130
#define LCD_Write_Len_AdminPwShowAddr			(uint8)0x04

#define LCD_WriteAdminPwAsciiAddr					(uint16)0x1140
#define LCD_Write_Len_AdminPwAsciiAddr			(uint8)0x04

#define LCD_WriteAdminPwIconAddr					(uint16)0x1150
#define LCD_Write_Len_AdminPwIconAddr			(uint8)0x01

#define LCD_WriteSysSetTimeAddr						(uint16)0x1160
#define LCD_Write_Len_SysSetTimeAddr				(uint8)(0x07+0x05)/*(uint8)0x07*/

#define LCD_WriteConsumeWayAddr 					(uint16)0x1170
#define LCD_Write_Len_ConsumeWayAddr			(uint8)0x02/*0x01*/

#define LCD_WriteConsumeIconAddr					(uint16)0x1180
#define LCD_Write_Len_ConsumeIconAddr			(uint8)0x04/*0x02*/

#define LCD_WriteSingleConsumeAddr				(uint16)0x1190
#define LCD_Write_Len_SingleConsumeAddr			(uint8)0x01+DeviceAddr_MAX

#if Modify_Key_190826 == 1
#define LCD_WriteFullSetTimeAddr						(uint16)0x11A0
#define LCD_Write_Len_FullSetTimeAddr				(uint8)(0x01 + 0x01/*��ַ������*/ + sizeof(LCD_SysModifyKey)/sizeof(uint16))
#else
#define LCD_WriteFullSetTimeAddr						(uint16)0x11A0
#define LCD_Write_Len_FullSetTimeAddr				(uint8)0x01
#endif

#define LCD_WriteRecordStateAddr					(uint16)0x11B0
#define LCD_Write_Len_RecordStateAddr			sizeof(LCDRecordStateData)/sizeof(uint16)

#define LCD_WriteClearRecordAddr					(uint16)0x11C0
#define LCD_Write_Len_ClearRecordAddr			(uint8)0x01

#define LCD_WriteClearOneBtnOpenAddr				(uint16)0x11D0
#define LCD_Write_Len_ClearOneBtnOpenAddr		(uint8)0x01

#define LCD_WriteAppointDoorIdAddr					(uint16)0x11E0
#define LCD_Write_Len_AppointDoorIdAddr			(uint8)0x02

#define LCD_WriteSoftVersionAddr						(uint16)0x11F0
#define LCD_Write_Len_SoftVersionAddr				(uint8)0x01

#if Modify_PW_190822 == 1
#define LCD_WriteModifyPwShowAddr				(uint16)0x1420
#define LCD_Write_Len_ModifyPwShowAddr		(uint8)(sizeof(LCD_ModifyPw)/sizeof(uint16))
#define LCD_WriteModifyPwAsciiAddr					(uint16)0x1440
#define LCD_Write_Len_ModifyPwAsciiAddr			(uint8)(sizeof(LCD_ModifyPw)/sizeof(uint16))
#endif

#if Modify_Key_190826 == 1
#define LCD_WriteModifyKeyAddr						(uint16)0x1460
#define LCD_Write_Len_ModifyKeyAddr 				(uint8)(sizeof(LCD_ModifyCard)/sizeof(uint16))
#define LCD_WriteCancelModifyKeyAddr				(uint16)0x1480
#define LCD_Write_Len_CancelModifyKeyAddr		(uint8)(sizeof(LCD_CancelCardKey)/sizeof(uint16))
#define LCD_WriteKeyModeChoiceAddr				(uint16)0x14A0				
#define LCD_Write_Len_KeyModeChoiceAddr		(uint16)(sizeof(LCD_KeyModeChoice)/sizeof(uint16))
#endif

/*
** ���õ�ѹ����
*/
#define LCD_WriteModuleVolCurAddr					(uint16)0x1500
#define LCD_WriteModuleVolCurlen					(uint8)0x03

/*����¼������ƫ�Ƶ�ַ����*/
#define LCD_WriteRecordAddr							(uint16)0x4000
#define LCD_Write_Len_RecordAddr					sizeof(LCDRecordData)/sizeof(uint16)

enum
{//Mainͼ��
	IdleIcon = (uint8)0x00,//����
	FullIcon = (uint8)0x01,//����
	IngIcon = (uint8)0x02,//�����
	ErrIcon = (uint8)0x03,//�쳣
	OverTimeIcon = (uint8)0x04,//��ʱ
	BatOnlineIcon = (uint8)0x05/*�������*/
};

enum
{//AdminPageͼ��
	CorrectPw = (uint16)0x00,//Ĭ��
	ErrPw = (uint16)0x01,//�������
	TwoPwErr = (uint16)0x02,//�������벻һ��
};

enum
{//���ѷ�ʽ
	Month_Card	= (uint16)0x01,//�¿�
	Consumer_Card = (uint16)0x02,//���ѿ�
};

enum
{//ѡ��Icon
	NoSelectedIcon = (uint16)0x00,//δѡ��
	SelectedIcon = (uint16)0x01,//ѡ��
};

#if Modify_Key_190826 == 1
enum
{
	Card_InitKey = 0x00,//ʹ������ĳ�ʼ��Կ
	Card_OriginalKey = 0x01,//ʹ�ó���ԭʼ��Կ
	Card_CabinrtKey = 0x02,//ʹ�ù�����Կ
	Card_NewInitKey = 0x03,//����Ƭ���³ɳ�ʼֵ
};

enum
{
	No_CardKeyIcon = 0x00,//����ʾ
	CardKeyIcon_Choice = 0x01,//ѡ���޸���Կģʽ
	CardKeyIcon_Opt = 0x02,//������޸���Կ
	CardKeyIcon_NoLegality = 0x03,//��Կ���Ϸ�
	CardKeyModifying = 0x04,//����Կ������
	CardKeyModifyFail = 0x05,//����Կ����ʧ��,��ȡ��,�������ó�ʼ��Կ
	CardKeyModifySucc = 0x06,//��Ƭ��Կ���ĳɹ�,��ȡ��
	CardKey_Cabinet_NoMatch = 0x07,//��Ƭ��ƥ��
};

enum
{
	CardKey_NoMode = 0x00,
	Cardkey_Mode_1 = 0x01,
	Cardkey_Mode_2 = 0x02,
	Cardkey_Mode_3 = 0x03,
};

#endif

typedef struct
{
	uint16 hour;
	uint16 min;
	uint16 sec;
	uint16 year[2];
	uint16 month;
	uint16 day;	
}LCDTime;

typedef struct
{
	uint16 stateIcon[ALLDOORNUM];//״̬ͼ��
	uint16 idleNum;//������
	uint16 ingNum;//����ʹ��
	uint16 WaterChkIcon;//ˮλ���
	uint16 cardComErr;//ˢ����
}LCDMainPage;

typedef struct
{
	uint16 show[4];
	uint16 Ascii[4];
	uint16 icon;
}LCDAdminPage;

typedef struct
{
	LCDTime time;
	uint16 consumeWay;//���ѷ�ʽ 
	uint16 consumeIcon[2];//����ͼ��
	uint16 singleConsum;//��������	����--100
	uint16 fullSetTime;//��������ʱ��--min ����
	uint16 oneBtnOpenDoor;//һ����������
	uint16 smokeCtr;//�����̸п���
	uint16 smokeIcon[2];//�̸�ͼ�� 
	uint16 deviceId[DeviceAddr_MAX];//�豸ID
}LCDSysParam;

/*
** ���õ�ѹ����
*/
typedef struct{
	uint16 vol;/*��ѹ*/
	uint16 cur;/*����*/
	uint16 ok;/*ȷ�ϰ�ť*/
}LcdCfgModule;

typedef struct
{
	uint16 no[4];
	uint16 cardNo[4];
	uint16 sTime[8];
	uint16 eTime[8];
	uint16 doorId;//����ID
	uint16 consumeWay[3];//�¿�/��������
	uint16 analysis[10];//ԭ�����
}LCDRecordData;

typedef struct
{
	uint16 curpage[4];
	uint16 totalpage[6];
	uint16 lastbtn;
	uint16 nextbtn;
}LCDRecordStateData;

typedef struct
{
	uint16 btnOk;
}LCD_ClearRecord;

typedef struct
{
	uint16 appointId;
	uint16 ok;
}LCD_OneAppointPage;

typedef struct
{
	uint16 ctr;//���ư�
}LCD_SoftVersion;

typedef struct
{
	uint8 id[2];
}LCD_SCREENID;

typedef struct
{
	uint16 addr;
	uint16 *pData;
}LCD_DATAFRAME;

typedef struct
{
	uint8 addr;
	uint8 *pData;
}LCD_CTRLFRAME;

#if Modify_PW_190822 == 1	
typedef struct
{
	uint16 newPw[4];
	uint16 rePw[4];
	uint16 ok;
	uint16 icon;
}LCD_ModifyPw;
#endif

#if Modify_Key_190826 == 1
typedef struct
{
	uint16 cabinet[5];//�޸Ĺ�����Կ
}LCD_SysModifyKey;

typedef struct
{
	uint16 initKey[5];//��Ƭ��ʼ��Կ
	uint16 newKey[5];//��Ƭ�µ���Կ
	uint16 way;//���¿�Ƭ��Կ��ʽ
	uint16 ok;
	uint16 icon;//����״̬����
}LCD_ModifyCard;

typedef struct
{
	bool initFlag;
	bool newFlag;
}LCD_ModifyCardChange;

typedef struct
{
	uint16 step;//ȡ������Կ����
}LCD_CancelCardKey;

typedef struct
{
	uint16 init;
	uint16 re;
}LCD_KeyModeChoice;
#endif

bool LCD_SetData(void *pData,void *temp);
bool LCD_GetData(void *pData);
uint16 LCD_GetScreenID(void);
bool LCD_SetScreenID(uint16 screenId);
void LCD_GetScreenIdSet(uint8*pdata);
bool LCD_JumpScreen(void *pData, uint8 dev);
bool LCD_ReadScreenID(void *pData, uint8 dev);
bool LCD_SetTime(Common_Time time);
bool LCD_WriteTime(void *pData,uint8 dev);
void LCD_UpdateStateIcon(void);
bool LCD_WriteStateIcon(void *pData,uint8 dev);
void LCD_UpdateStateNum(void);
bool LCD_WriteStateNum(void *pData,uint8 dev);
void LCD_ClearAdminPwShow(void);
void LCD_SetAdminPwShowXX(void);
bool LCD_WriteAdminPwShow(void *pData,uint8 dev);
void LCD_ClearAdminPwAscii(void);
bool LCD_WriteAdminPwAscii(void *pData,uint8 dev);
void LCD_GetAdminPw(LCDAdminPage * adminPage);
bool LCD_GetBtnOk(void);
bool LCD_ReadAdminPwAscii(void *pData,uint8 dev);
void LCD_SetAdminPwIcon(uint16 icon);
bool LCD_WriteAdminPwIcon(void *pData,uint8 dev);
void LCD_UpdateSysTimeShow(void);
void LCD_UpdateSysSet(void);
void LCD_GetSysSetShow(LCDTime *time);
bool LCD_IsUpdateTime(void);
bool LCD_WriteSysParamTime(void *pData,uint8 dev);
bool LCD_ReadSysParamTime(void *pData,uint8 dev);
void LCD_ClearConsumeWay(void);
void Lcd_SetSmokeCtr(uint16 smokeCtr);
bool Lcd_IsUpdateSmokeCtr(void);
void LCD_UpdateSmokeCtrIcon(uint16 smokeCtr);
bool LCD_IsUpdateConsumeWay(void);
void LCD_SetConsumeWaySet(uint16 consumeWay);
bool LCD_WriteConsumeWay(void *pData,uint8 dev);
bool LCD_ReadConsumeWay(void *pData,uint8 dev);
void LCD_UpdateConsumeIcon(uint16 consumeWay);
bool LCD_WriteConsumeWayIcon(void *pData,uint8 dev);
void LCD_SetDeviceId(uint8*deviceId);
void LCD_GetDeviceId(uint8*deviceId);
bool LCD_IsDeviceIdChange(void);
bool LCD_IsDeviceIdLegality(void);
	
void LCD_SetSingleConsume(uint16 singleConsume);
uint16 LCD_GetSingleConsume(void);
bool LCD_IsSingleConsumeChange(void);
bool LCD_ReadSingleConsume(void *pData,uint8 dev);
bool LCD_WriteSingleConsume(void *pData,uint8 dev);
void LCD_SetFullSetTime(uint16 fullSetTime);
uint16 LCD_GetFullSetTime(void);
bool LCD_IsFullSetTimeChange(void);
bool LCD_ReadFullSetTime(void *pData,uint8 dev);
bool LCD_WriteFullSetTime(void *pData,uint8 dev);
void LCD_ClearRecordData(void);
void LCD_SetRecordData(Charge_Record *record,uint16 no);
bool LCD_WriteRecordData(void *pData,uint8 dev);
void LCD_SetRecordState(uint16 cur,uint16 total,uint16 lastbtn,uint16 nextbtn);
void LCD_GetRecordBtn(uint16 *last,uint16 *next);
bool LCD_WriteRecordState(void *pData,uint8 dev);
bool LCD_ReadRecordState(void *pData,uint8 dev);
void LCD_ClearClearRecordBtnOk(void);
uint16 LCD_GetClearRecordBtnOk(void);
bool LCD_WriteClearRecord(void *pData,uint8 dev);
bool LCD_ReadClearRecord(void *pData,uint8 dev);
void LCD_ClearClearOneBtnOpenDoor(void);
uint16 LCD_GetClearOneBtnOpenDoor(void);
bool LCD_WriteClearOneBtnOpenDoor(void *pData,uint8 dev);
bool LCD_ReadClearOneBtnOpenDoor(void *pData,uint8 dev);
void LCD_ClearAppointDoorId(void);
void LCD_GetAppointDoorId(LCD_OneAppointPage* oneAppoint);
bool LCD_WriteAppointDoorId(void *pData,uint8 dev);
bool LCD_ReadAppointDoorId(void *pData,uint8 dev);
void LCD_SetSoftVersion(LCD_SoftVersion softVer);
bool LCD_WriteSoftVersion(void *pData,uint8 dev);

#if Modify_PW_190822  == 1
void LCD_ClearModifyPwData(LCD_ModifyPw *modifyPw);
void LCD_SetModifyPwShowData(LCD_ModifyPw modifyPw);
bool LCD_WriteModifyPwShow(void *pData,uint8 dev);
void LCD_SetModifyPwAsciiData(LCD_ModifyPw modifyPw);
void LCD_GetModifyPwAsciiData(LCD_ModifyPw *modifyPw);
bool LCD_WriteModifyPwAscii(void *pData,uint8 dev);
bool LCD_ReadModifyPwAscii(void *pData,uint8 dev);
void LCD_ChkPwAsciiModify(void);
#endif

#if Modify_Key_190826 == 1
LCD_SysModifyKey LCD_GetSysModifyKey(void);
void LCD_SetCabinetKey(ParamSysCabinet parSysCabinet);
bool LCD_IsKeyChange(void);
bool LCD_IsKeyLegality(void);

LCD_ModifyCardChange LCD_GetModifyCardChange(void);
void LCD_SetModifyCardChange(LCD_ModifyCardChange lModifyCardChange);

void LCD_SetModifyCardKey(LCD_ModifyCard lModifyCardKey);
LCD_ModifyCard LCD_GetModifyCardKey(void);
bool LCD_IsCardInitKeyChange(void);
bool LCD_IsCardInitKeyLegality(void);
bool LCD_IsCardNewKeyChange(void);
bool LCD_IsCardNewKeyLegality(void);
bool LCD_WriteModifyCardKey(void *pData,uint8 dev);
bool LCD_ReadModifyCardKey(void *pData,uint8 dev);

void LCD_SetCancelModifyCardKey(LCD_CancelCardKey lCanelCardKey);
LCD_CancelCardKey LCD_GetCancelModifyCardKey(void);
bool LCD_WriteCancelModifyCardKeyStep(void *pData,uint8 dev);
bool LCD_ReadCancelModifyCardKeyStep(void *pData,uint8 dev);

LCD_KeyModeChoice LCD_GetKeyModeChoice(void);
void LCD_SetKeyModeChoice(LCD_KeyModeChoice lKeyModeChoice);
bool LCD_WriteKeyModeChoice(void *pData,uint8 dev);

#endif

/*
** set Module Vol Cur To Lcd
*/
void set_ModuleVolCurToLcd(uint16 vol,uint16 cur);

/*
** chk Module Vol Cur Cfg Is Legal
**	@return:
**		-1:δ������ȷ�ϰ�����
**		0:�������óɹ�
**		1:����һ��,�����ó���
*/
int8 chk_ModuleVolCurCfgIsLegal(void);

/*
** ���õ�ѹ���� ��ַ:0x1500
*/
bool LCD_WriteModuleVolCur(void *pData,uint8 dev);

/*
** ���õ�ѹ���� ��ַ:0x1500
*/
bool LCD_ReadoduleVolCur(void *pData,uint8 dev);

#endif

