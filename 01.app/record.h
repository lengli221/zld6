#ifndef __RECORD_H
#define __RECORD_H

#define RECORD_NO_MAX							5000//最大充电记录数

#define RECORD_FLASH_FLAG						(uint8)0xA5

enum
{/*Charge_Record.state声明*/ /*关于解释说明:增加说明针对中天二代版本*/
	Record_Init = (uint8)0x00,
	OpenDoorIdErr = (uint8)0x01,//--仓门故障
	OpenFullErr = (uint8)0x02,
	Normal_SaveBat = (uint8)0x03,//--开仓成功
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
	uint8 flashFlag;//flash存储标志
	uint8 state;//存储状态
	uint8 cardNo[8];//卡号
	uint8 doorId;//仓门ID
	Common_Time startTime;//开始时间
	Common_Time stopTime;//结束时间
	uint16 consumeWay;//消费方式
	uint8 sum;//数据校验
}Charge_Record;

uint16 ChargeGetRecordNo(void);
void chargeClearRecordNo(void);
 void ChargeRecordNoWrite(uint16 no);
 void ChargeRecordWritePreData(uint8 proc,uint8 state,uint16 consumeWay);
 void ChargeRecordWrite(uint8 proc,uint8 state,uint8 *cardNo,uint8 doorId);
 bool ChargeRecordRead(uint16 no,Charge_Record *record);
void ChargeRecordNoRead(void);
#endif

