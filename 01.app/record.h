#ifndef __RECORD_H
#define __RECORD_H

#define RECORD_NO_MAX							5000//������¼��

#define RECORD_FLASH_FLAG						(uint8)0xA5

enum
{/*Charge_Record.state����*/ /*���ڽ���˵��:����˵�������������汾*/
	Record_Init = (uint8)0x00,
	OpenDoorIdErr = (uint8)0x01,//--���Ź���
	OpenFullErr = (uint8)0x02,
	Normal_SaveBat = (uint8)0x03,//--���ֳɹ�
	No_SaveBat = (uint8)0x04,
	SaveAbnormalBat = (uint8)0x05,
	OverTimeSaveNormalBat = (uint8)0x06,
	OverTimeSaveAbnormalBat = (uint8)0x07,
	CloseDoorNoTakeBat = (uint8)0x08,
	CloseDoorTakeBat = (uint8)0x09,
	NoIdleDoor = (uint8)0x0A,
	NoFullDoor = (uint8)0x0B,
	Record_FullInit = (uint8)0x0C,
};

typedef struct
{
	uint8 flashFlag;//flash�洢��־
	uint8 state;//�洢״̬
	uint8 cardNo[8];//����
	uint8 doorId;//����ID
	Common_Time startTime;//��ʼʱ��
	Common_Time stopTime;//����ʱ��
	uint16 consumeWay;//���ѷ�ʽ
	uint8 sum;//����У��
}Charge_Record;

uint16 ChargeGetRecordNo(void);
void chargeClearRecordNo(void);
 void ChargeRecordNoWrite(uint16 no);
 void ChargeRecordWritePreData(uint8 proc,uint8 state,uint16 consumeWay);
 void ChargeRecordWrite(uint8 proc,uint8 state,uint8 *cardNo,uint8 doorId);
 bool ChargeRecordRead(uint16 no,Charge_Record *record);
void ChargeRecordNoRead(void);
#endif

